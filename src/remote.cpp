/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "remote.h"
#include "runloop.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <iostream>
#include <string.h> // for strerror
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

// TODO: Don't hardcode this?
static const std::string k_server_url = "http://localhost:43001";

#define CMD_BIND(command_fn) \
    std::bind(&command_fn, this, std::placeholders::_1)

Remote::Remote()
{
    uid_t uid = getuid();
    std::string prog_path = "/run/user/" + std::to_string(uid) + "/chipotherm";

    do {
        int result = 0;

        struct stat pp_stat;
        if (stat(prog_path.c_str(), &pp_stat) != 0 || !S_ISDIR(pp_stat.st_mode)) {
            mkdir(prog_path.c_str(), 0744);
        }

        // Create UNIX socket
        std::string fifo_path = prog_path + "/socket";
        if (stat(fifo_path.c_str(), &pp_stat) == 0) {
            // Remove if socket already exists
            result = unlink(fifo_path.c_str());
            if (result != 0) {
                fprintf(stderr, "Error removing socket: %s (%d)\n", fifo_path.c_str(), result);
                break;
            }
        }

        _socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (_socket_fd < 0) {
            fprintf(stderr, "Error opening socket\n");
            break;
        }

        // Configure socket
        struct sockaddr_un addr;
        bzero(&addr, sizeof(sockaddr_un));

        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, fifo_path.data(), sizeof(addr.sun_path) - 1);

        // Bind to socket
        result = bind(_socket_fd, (struct sockaddr *)&addr, sizeof(addr));
        if (result < 0) {
            fprintf(stderr, "Error binding to socket: %s\n", strerror(errno));
            break;
        }

        // Start listening
        result = listen(_socket_fd, 3);
        if (result < 0) {
            fprintf(stderr, "Error listening to socket\n");
            break;
        }
    } while (0);

    // Build command map
    _command_map = {
        { "set_enabled", CMD_BIND(Remote::cmd_set_enabled) },
        { "set_temperature", CMD_BIND(Remote::cmd_set_temperature) },
        { "handle_server_cmd", CMD_BIND(Remote::cmd_handle_server_cmd) },
    };

    // Setup soup session
    SoupSession *session = soup_session_new();
    _http_session = std::shared_ptr<SoupSession>(session, g_object_unref);
}

Remote::~Remote()
{
    close(_socket_fd);
    _listening = false;
    _http_polling_thread.join();
    _socket_listener_thread.join();
}

void Remote::socket_listener_main()
{
    std::cout << "listening to commands..." << std::endl;

    ssize_t read_b = 0;
    const unsigned MAX_LEN = 2048;
    std::vector<char> buffer(MAX_LEN);
    std::string command_str;
    while (_listening) {
        command_str.clear();

        // Accept connection
        int sock_in = accept(_socket_fd, nullptr, nullptr);
        if (sock_in < 0) {
            fprintf(stderr, "Error accepting connection: %s\n", strerror(errno));
            _listening = false;
            break;
        }

        // Read command to buffer
        while ( (read_b = read(sock_in, &buffer[0], 1024)) > 0 ) {
            command_str.append(buffer.cbegin(), buffer.cend());
        }

        if (command_str.length() > 0) {
            boost::trim(command_str);
            process_command(command_str);
        } else {
            fprintf(stderr, "Got nothing back from client\n");
        }

        close(sock_in);
    }
}

bool Remote::get_online_status()
{
    return _online;
}

void Remote::start_listening()
{
    _listening = true;
    _socket_listener_thread = std::thread(&Remote::socket_listener_main, this);
    _http_polling_thread = std::thread(&Remote::http_polling_main, this);

    // TODO: would be nice if this sent an updated state right away, but the current
    // state hasn't been populated yet, so this needs to happen later. Maybe introduce
    // a Runloop::schedule_after_delay or something
    //send_update_state();
}

void Remote::process_command(const std::string command)
{
    std::vector<std::string> parts;
    boost::split(parts, command, boost::is_space(), boost::token_compress_on);

    std::string &command_name = parts[0];
    auto search = _command_map.find(command_name);
    if (search != _command_map.end()) {
        _command_map[command_name](std::vector<std::string>(parts.begin() + 1, parts.end()));
    } else {
        fprintf(stderr, "Unrecognized command: %s\n", parts[0].c_str());
    }
}

/*
 * Commands
 */

static inline bool parse_bool(std::string str)
{
    // Allow to be a little fuzzy
    return str == "true" || str == "1";
}

Remote::CommandFn Remote::cmd_set_enabled(const CommandArgs args)
{
    bool enabled = parse_bool(args[0]);
    Runloop::main_runloop().schedule_task([=]() {
        set_enabled(enabled);
    });
}

Remote::CommandFn Remote::cmd_set_temperature(const CommandArgs args)
{
    float temperature = std::stof(args[0]);
    Runloop::main_runloop().schedule_task([=]() {
        set_temperature(temperature);
    });
}

Remote::CommandFn Remote::cmd_handle_server_cmd(const CommandArgs args)
{
    // Handle "refresh" command
    if (args[0] == "refresh") {
        send_update_state();
        return;
    }

    // Otherwise, assume this is a setState command
    if (args.size() != 2) {
        fprintf(stderr, "Got server command that wasn't in the correct format\n");
        return;
    }

    bool enabled = args[0] == "enabled";
    float temperature = std::stof(args[1]);
    Runloop::main_runloop().schedule_task([=]() {
        set_temperature(temperature);
        set_enabled(enabled);
    });
}

/*
 * Server communication
 */

void Remote::http_polling_main()
{
    _online = true;

    while (_listening) {
        SoupMessage *msg = soup_message_new("GET", (k_server_url + "/poll").c_str());
        guint status = soup_session_send_message(_http_session.get(), msg);
        if (status == 200) {
            _online = true;

            // Parse response body
            GBytes *response_bytes = soup_buffer_get_as_bytes(
                soup_message_body_flatten(msg->response_body)
            );

            if (response_bytes == nullptr) {
                fprintf(stderr, "Error getting response buffer\n");
                continue;
            }

            gsize size = 0;
            gconstpointer data = g_bytes_get_data(response_bytes, &size);

            if (data == nullptr) {
                fprintf(stderr, "Error getting response buffer as bytes\n");
                continue;
            }

            std::string response_str((const char *)data, size);
            process_command("handle_server_cmd " + response_str);
        } else {
            fprintf(stderr, "Got status %d from the polling thread\n", status);
            switch (status) {
                case SOUP_STATUS_CANT_CONNECT:
                    _online = false;
                    fprintf(stderr, "Unable to connect to command & control server\n");
                    fprintf(stderr, "Trying again in 15 seconds\n");

                    // Wait a little while, then try again
                    std::this_thread::sleep_for(std::chrono::seconds(15));
                    continue;
                case SOUP_STATUS_IO_ERROR:
                    // Probably just timed out, go and try again
                    continue;
                default:
                    continue;
            }
        }

        g_object_unref(msg);
    }
}

void Remote::send_update_state()
{
    using boost::property_tree::ptree;
    using boost::property_tree::write_json;

    // TODO: this should probably happen on the main thread?
    Remote::State state = refresh_state();

    ptree state_tree;
    state_tree.put("CurrentTemperature", state.current_temp);
    state_tree.put("TargetTemperature", state.target_temp);
    state_tree.put("Enabled", state.enabled);
    state_tree.put("HeatOn", state.heat_on);

    std::ostringstream buf;
    write_json(buf, state_tree, true);

    SoupMessage *msg = soup_message_new("POST", (k_server_url + "/updateState").c_str());
    soup_message_set_request(
        msg, 
        "application/text",
        SOUP_MEMORY_COPY, 
        buf.str().c_str(), 
        buf.str().length()
    );

    guint status = soup_session_send_message(_http_session.get(), msg);
    if (status != 200) {
        fprintf(stderr, "Error sending updateState: status %u\n", status);
    }

    g_object_unref(msg);
}

