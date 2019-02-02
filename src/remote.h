/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <sigc++/sigc++.h>
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
    std::string _fifo_path;

    void main_loop();
    void process_command(const std::string whole_command);

    // Commands
    using CommandFn = void; // decorator for readability
    using arguments_t = std::vector<std::string>;
    std::unordered_map<std::string, std::function<CommandFn(const arguments_t)>> _command_map;

    CommandFn cmd_set_enabled(const arguments_t args);
    CommandFn cmd_set_temperature(const arguments_t args);

};

