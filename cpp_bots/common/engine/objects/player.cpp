#include "player.hpp"

Player::Player(const Json& json) :
    gold{json["gold"]},
    income{json["income"]}
{
    for (auto& farm : json["buildings"]["farms"])
    {
        farms.push_back(Building{farm});
    }
    for (auto& turret : json["buildings"]["turrets"])
    {
        turrets.push_back(Building{turret});
    }
    for (auto& soldier : json["units"])
    {
        soldiers.push_back(soldier);
    }
}
