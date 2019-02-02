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
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define CMD_BIND(command_fn) \
    std::bind(&command_fn, this, std::placeholders::_1)

Remote::Remote()
{
    uid_t uid = getuid();
    std::string prog_path = "/run/user/" + std::to_string(uid) + "/chipotherm";

    struct stat pp_stat;
    if (stat(prog_path.c_str(), &pp_stat) != 0 || !S_ISDIR(pp_stat.st_mode)) {
        mkdir(prog_path.c_str(), 0744);
    }

    std::string fifo_path = prog_path + "/fifo";
    if (stat(fifo_path.c_str(), &pp_stat) != 0) {
        int result = mkfifo(fifo_path.c_str(), 0744);
        if (result != 0) {
            fprintf(stderr, "Error creating fifo: %s (%d)\n", fifo_path.c_str(), result);
        }
    }

    _fifo_path = fifo_path;

    // Build command map
    _command_map = {
        { "set_enabled", CMD_BIND(Remote::cmd_set_enabled) },
        { "set_temperature", CMD_BIND(Remote::cmd_set_temperature) },
    };
}

Remote::~Remote()
{
    _listening = false;
    _listener_thread.join();
}

void Remote::main_loop()
{
    std::cout << "listening to commands..." << std::endl;

    char *line = nullptr;
    size_t len = 0;
    ssize_t read_b = 0;

    while (_listening) {
        FILE *cmd_in = fopen(_fifo_path.c_str(), "r");
        if (!cmd_in) {
            fprintf(stderr, "Error opening FIFO: %s\n", strerror(errno));
            _listening = false;
            break;
        }

        while ( (read_b = getline(&line, &len, cmd_in)) != -1 ){
            std::string command_str(line);
            boost::trim(command_str);

            process_command(command_str);
        }

        fclose(cmd_in);
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

Remote::CommandFn Remote::cmd_set_enabled(const arguments_t args)
{
    bool enabled = parse_bool(args[0]);
    Runloop::main_runloop().schedule_task([=]() {
        set_enabled(enabled);
    });
}

Remote::CommandFn Remote::cmd_set_temperature(const arguments_t args)
{
    float temperature = std::stof(args[0]);
    Runloop::main_runloop().schedule_task([=]() {
        set_temperature(temperature);
    });
}

