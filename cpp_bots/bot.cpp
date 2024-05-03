#include "bot.hpp"

#include <iostream>

void Bot::preprocess()
{
}

std::string Bot::make_move()
{
    return "W";
}

void Bot::run()
{
    receive_game_properties();
    receive_arena_properties();

    preprocess();
    send_message("READY");

    for (;;)
    {
        const auto move = make_move();
        send_message(move);

        receive_status();
        if (status == "END")
        {
            break;
        }

        receive_arena_properties();
    }
}

void Bot::receive_game_properties()
{
    std::string side_string;
    std::cin >> game_timeout >> move_timeout >> ready_timeout >> side_string;
    side = side_string == "left" ? Side::left : Side::right;
}

std::string read_line()
{
    std::string line{""};
    while (line.empty())
    {
        std::getline(std::cin, line);
    }

    return line;
}

void Bot::receive_arena_properties()
{
    // TODO: read json
    arena_properties = read_line();
}

void Bot::receive_status()
{
    status = read_line();
}

void Bot::send_message(const std::string& message)
{
    std::cout << message << '\n';
}
