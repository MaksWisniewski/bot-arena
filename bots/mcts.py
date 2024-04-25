import sys, os
import numpy as np

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bot_package.bot import Bot
from packages.game_logic.game import Game
from packages.game_logic.actions import str_to_action

class MCTS_Node:
    """
    Stores information about MCTS tree node

    Attributes:
    - `games_played` - total amount of simulations that used this node
    - `games_won` - number of simulations in which our player won
    - `children` - dictionary that maps moves to corresponding child nodes

    Methods:
    - `score()`
    - `is_leaf()`
    """

    def __init__(self):
        self.games_played = 0
        self.games_won = 0
        self.children = {}


    def score(self) -> float:
        """
        Returns score of the node calculated as a ratio of `games_won` and `games_played`
        or `0` if `games_played` is `0`

        Might need to be changed in future so that frequently visited nodes have higher score
        """
        return 0 if self.games_played == 0 else self.games_won / self.games_played


    def is_leaf(self) -> bool:
        return not bool(self.children)


    def get_best_move(self) -> str:
        """
        Returns best move based on children scores
        """
        if self.is_leaf():
            return None

        probabilities = np.array([np.exp(self.children[move].score()) for move in self.children.keys()])
        probabilities /= probabilities.sum()

        return str(np.random.choice(list(self.children.keys()), p=probabilities))


class MCTS_Bot(Bot):
    def preprocess(self) -> None:
        self.MCTS_root = MCTS_Node()
        self.opponent_side = 'right' if self.side == 'left' else 'left'

        print(self.side, file=sys.stderr)
        print(self.arena_properties['arena'], file=sys.stderr)


    def simulate(self) -> str:
        """
        Performs simulation and returns best move
        """
        # DONE:
        # - create Game (need to add method for creating Game object from json state)
        # - go to leaf of the tree, choose nodes based on their scores
        # - get legal moves
        # - perform simulation (random game) for each of moves from previous step and create new nodes
        # - update node stats on the path to root
        # - return best move based on corresponding node's score
        # TODO:
        # - !!! find out what's so slow and optimize it !!!
        # - rethink what should opponent's move be when choosing next node
        # - rethink if the tree should be reinitialized before each move or if some nodes can be reused (for now they are reused)
        # - filter legal moves (on large map there are lot of them - some should not be considered)
        # - add heuristic for game state evaluation -> to evaluate unfinished simulations
        # - improve score function (to prefer nodes that are visited most often)
        # - use move timeout to choose number of simulations
        # - maybe smarter move selection in simulations ?

        game = Game(state=self.arena_properties)

        def update_game(my_move, opponent_move):
            if self.side == 'left':
                game.update(action_left=my_move,
                            action_right=opponent_move)
            else:
                game.update(action_left=opponent_move,
                            action_right=my_move)

        MAX_GAME_LENGTH = 50
        NUM_OF_SIMULATIONS = 5

        for _ in range(NUM_OF_SIMULATIONS):
            # go to leaf
            node = self.MCTS_root
            path_to_root = [node]
            while (not node.is_leaf()):
                my_move = node.get_best_move()
                update_game(my_move=str_to_action(my_move, self.side),
                            opponent_move=game.get_random_move(self.opponent_side))
                node = node.children[my_move]
                path_to_root.append(node)

            # get legal moves
            legal_moves = game.get_legal_moves(self.side)
            # at most 10 random moves
            considered_moves = np.array(legal_moves)[np.random.choice(len(legal_moves), min(len(legal_moves), 10), replace=False)]
            node.children = {str(move): MCTS_Node() for move in considered_moves}

            # perform simulation for each move
            for move in node.children.keys():
                child_node = node.children[move]
                child_node.games_played += 1

                game_copy = game.copy()
                for _ in range(MAX_GAME_LENGTH):
                    move_result = game_copy.update(game_copy.get_random_move('left'),
                                                   game_copy.get_random_move('right'))
                    if 'Left win' in move_result:
                        child_node.games_won += int(self.side == 'left')
                        break
                    elif 'Right win' in move_result:
                        child_node.games_won += int(self.side == 'right')
                        break
                    elif "Tie" in move_result:
                        break

            games_played = len(node.children.keys())
            games_won = sum(node.children[move].games_won for move in node.children.keys())

            # propagate update of node values
            for node in path_to_root:
                node.games_played += games_played
                node.games_won += games_won

        best_move = self.MCTS_root.get_best_move()
        self.MCTS_root = self.MCTS_root.children[best_move]
        return best_move


    def make_move(self) -> str:
        # perform simulation and chose the best move
        best_move = self.simulate()
        print('chosen move: ', best_move, file=sys.stderr)
        return best_move


    def post_move_action(self) -> None:
        return super().post_move_action()


while True:
    MCTS_Bot().run()
