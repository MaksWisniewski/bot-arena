import sys, os, random, json

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bot_package.bot import Bot
from bot_package.move import Move
from packages.game_logic.game import Game
from packages.simulator.serializer import Serializer

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
        return bool(self.children)


class MCTS_Bot(Bot):
    def preprocess(self) -> None:
        self.MCTS_root = MCTS_Node()


    def simulate(self) -> str:
        """
        Performs simulation and returns best move
        """
        # DONE:
        # - create Game (need to add method for creating Game object from json state)
        # TODO:
        # - go to leaf of the tree, choose nodes based on their scores
        #   (best or better - random with probabilities proportional to scores)
        # - get legal moves (and probably filter out some of them - like building towers far from the path)
        # - perform simulation (random game) for each of moves from previous step and create new nodes
        # - update node stats on the path to root
        # - return best move based on corresponding node's score (best or random with distribution based on scores)

        game = Game(state=self.arena_properties)

        serialized_game = Serializer.get_json(game)

        print('self.arena_properties:\n', json.dumps(self.arena_properties), file=sys.stderr)
        print('serialized:\n', serialized_game, file=sys.stderr)
        print('Equal' if json.dumps(self.arena_properties) == serialized_game else 'Not equal', file=sys.stderr)

        return Move.Spawn('archer')


    def make_move(self) -> str:
        # perform simulation and chose the best move
        return self.simulate()


    def post_move_action(self) -> None:
        return super().post_move_action()


while True:
    MCTS_Bot().run()
