# Function to determine the winner of a game based on the player's state
def determine(players_state: dict[int, bool], player_instances: dict):
    if not players_state[player_instances[0]] and not players_state[player_instances[1]]:
        return 'TIE'
    elif not players_state[player_instances[0]]:
        return 1
    elif not players_state[player_instances[1]]:
        return 0
