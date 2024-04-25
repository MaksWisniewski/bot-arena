import sys, os, random, json

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bot_package.bot import Bot
from bot_package.move import Move
from packages.game_logic.game import Game
from packages.simulator.serializer import Serializer

class MinMax_Node:
    """
    Stores information about MinMax tree node

    Attributes:
    - `games_played` - total amount of simulations that used this node
    - `games_won` - number of simulations in which our player won
    - `children` - dictionary that maps moves to corresponding child nodes

    Methods:
    - `is_leaf()`
    """

    def __init__(self):
        self.games_played = 0
        self.games_won = 0
        self.children = {}


    def is_leaf(self) -> bool:
        return bool(self.children)


class MinMax_Bot(Bot):
    def preprocess(self) -> None:
        self.MinMax_root = MinMax_Node()


    def min_max_search(self, game, depth: int, maximizing_player: bool) -> float:
        """
        Implements the MinMax search algorithm
        """
        if depth == 0 or game.is_game_over():
            return self.evaluate(game)  # Evaluation function to estimate game state

        if maximizing_player:
            value = float("-inf")
            legal_moves = game.get_legal_moves()

            for move in legal_moves:
                game.make_move(move)
                value = max(value, self.min_max_search(game, depth - 1, False))
                game.undo_last_move()
            return value
        else:
            value = float("inf")
            legal_moves = game.get_legal_moves()

            for move in legal_moves:
                game.make_move(move)
                value = min(value, self.min_max_search(game, depth - 1, True))
                game.undo_last_move()
            return value


    def evaluate(self, game) -> float:
        """
        Evaluation function to estimate game state
        """
        pass


    def simulate(self) -> str:
        """
        Performs MinMax search and returns the best move
        """
        game = Game(state=self.arena_properties)
        best_move = None
        best_value = float("-inf")
        legal_moves = game.get_legal_moves()

        for move in legal_moves:
            game_cp = game.copy()

            if self.side == 'left':
                game_cp.make_move(move, Wait("right"))
            else:
                game_cp.make_move(Wait("left"), move)

            value = self.min_max_search(game_cp, depth=3, maximizing_player=False)

            if value > best_value:
                best_value = value
                best_move = move

        return best_move


    def make_move(self) -> str:
        # perform simulation and chose the best move
        return self.simulate()


    def post_move_action(self) -> None:
        return super().post_move_action()


while True:
    MinMax_Bot().run()
