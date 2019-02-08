/*
 * Created on Fri Feb 01 2019
 *
 * Copyright (c) 2019 James Magahern <james@magahern.com>
 */

#pragma once

#include <boost/signals2.hpp>
#include <libsoup/soup.h>
#include <memory>
#include <sys/socket.h>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace boost::signals2;

class Remote
{
public:
    Remote();
    ~Remote();

    void start_listening();

    // Command signals
    boost::signals2::signal<void(bool)>  set_enabled;
    boost::signals2::signal<void(float)> set_temperature;
    
    struct State {
        bool  enabled;
        bool  heat_on;
        float current_temp;
        float target_temp;
    };
    boost::signals2::signal<void(State&)> refresh_state;

    bool get_online_status();

private:
    bool        _online = false;
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

