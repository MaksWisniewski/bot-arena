#include "node.hpp"

void MCTSNode::update()
{
}

std::string MCTSNode::get_best_move()
{
    return "S swordsman";
}

bool MCTSNode::simulate()
{
    return false;
}

int MCTSNode::expand()
{
    return 0;
}

double MCTSNode::score()
{
    // TODO: implement better score function
    return games_played == 0 ? 0 : games_won / games_played;
}

bool MCTSNode::is_leaf()
{
    return children.empty();
}
