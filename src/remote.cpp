/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#include "remote.h"
#include "runloop.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include <string.h> // for strerror
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

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
    };
}

Remote::~Remote()
{
    close(_socket_fd);
    _listening = false;
    _listener_thread.join();
}

void Remote::main_loop()
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

void Remote::start_listening()
{
    _listening = true;
    _listener_thread = std::thread(&Remote::main_loop, this);
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

