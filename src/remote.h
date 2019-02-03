/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <libsoup/soup.h>
#include <memory>
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
    
    using StatePair = std::pair<bool, float>;
    sigc::signal<StatePair(void)> refresh_state;

private:
    bool        _listening = false;
    int         _socket_fd = -1;

    std::thread _socket_listener_thread;
    std::thread _http_polling_thread;

    std::shared_ptr<SoupSession> _http_session;

    void http_polling_main();
    void socket_listener_main();

    void process_command(const std::string whole_command);

    // Server calls
    void send_update_state();

    // Commands
    using CommandFn   = void; // decorator for readability
    using CommandArgs = std::vector<std::string>;
    std::unordered_map<std::string, std::function<CommandFn(const CommandArgs)>> _command_map;

    CommandFn cmd_set_enabled(const CommandArgs args);
    CommandFn cmd_set_temperature(const CommandArgs args);
    CommandFn cmd_handle_server_cmd(const CommandArgs args);

};

