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

    // Call this to start listening on the TCP socket and open a connection to the command and
    // control server. Returns immediately.
    void start_listening(std::string server_url);

    // Returns `true` if we have heard from the server recently
    bool get_online_status();

    /** Command Signals **/
    // Attach to this signal for when a remote command is received to enable/disable monitoring
    boost::signals2::signal<void(bool)>  set_enabled;

    // Attach for target temperature changes
    boost::signals2::signal<void(float)> set_temperature;

    struct State {
        bool  enabled;
        bool  heat_on;
        float current_temp;
        float target_temp;
    };

    // This signal is sent when the state needs to be refreshed/synced between the controller and
    // the remote
    boost::signals2::signal<void(State&)> refresh_state;

private:
    bool        _online = false;
    bool        _listening = false;
    int         _socket_fd = -1;

    std::thread _socket_listener_thread;
    std::thread _http_polling_thread;

    std::string                  _server_url;
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

