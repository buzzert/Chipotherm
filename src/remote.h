/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <sigc++/sigc++.h>
#include <sys/socket.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class Remote
{
public:
    Remote();
    ~Remote();

    void start_listening();

    // Command signals
    sigc::signal<void(bool)>  set_enabled;
    sigc::signal<void(float)> set_temperature;

private:
    bool        _listening = false;
    std::thread _listener_thread;
    int         _socket_fd = -1;

    void main_loop();
    void process_command(const std::string whole_command);

    // Commands
    using CommandFn   = void; // decorator for readability
    using CommandArgs = std::vector<std::string>;
    std::unordered_map<std::string, std::function<CommandFn(const CommandArgs)>> _command_map;

    CommandFn cmd_set_enabled(const CommandArgs args);
    CommandFn cmd_set_temperature(const CommandArgs args);

};

