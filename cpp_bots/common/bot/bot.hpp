#pragma once

#include "../json.hpp"
#include "../side.hpp"

#include <string>

class Bot
{
public:
    virtual void preprocess();
    virtual std::string make_move();
    virtual void post_move_action();

    void run();

protected:
    void receive_game_properties();
    void receive_arena_properties();
    void receive_status();

    void send_message(const std::string&);

    int game_timeout;
    int move_timeout;
    int ready_timeout;

    Side side;

    std::string status;
    Json arena_properties;
};
