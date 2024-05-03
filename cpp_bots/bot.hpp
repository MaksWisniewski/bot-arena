#pragma once

#include <nlohmann/json.hpp>
#include <string>

using Json = nlohmann::json;

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

    void send_message(const std::string& message);

    int game_timeout;
    int move_timeout;
    int ready_timeout;

    enum class Side : bool
    {
        left,
        right
    } side;

    std::string status;
    Json arena_properties;
};
