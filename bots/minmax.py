import sys, os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bot_package.bot import Bot
from packages.game_logic.game import Game
from packages.game_logic.actions import Wait


def negate_player(x):
    return "right" if x == "left" else "left" if x == "right" else ""

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

    def min_max_search(self, game: Game, depth: int, _player: str) -> float:
        def browse_moves(__maximizing_player:bool):
            """search"""
            value = float("-inf") if __maximizing_player else float("inf")
            choose_function = max if __maximizing_player else min

            legal_moves = game.get_legal_moves(_player)
            for move in legal_moves:
                game_cp = game.copy()

                if _player == 'left': game_cp.update(move, Wait("right"))
                else: game_cp.update(Wait("left"), move)

                value = choose_function(value, self.min_max_search(game_cp, depth - 1, negate_player(_player)))
            return value


        if depth == 0 or game.is_win() is not None:
            return self.evaluate(game)

        return browse_moves((_player == self.side))


    def evaluate(self, game) -> float:
        """
        Evaluation function to estimate game state
        """
        answer = 0


        # jesli wygranko to premiuj funkcje
        if game.is_win() is not None and 'Tie' not in game.is_win():
            am_i_left = ('left' == self.side)
            left_wins = 'Left win' in game.is_win()

            if am_i_left == left_wins:
                answer += 199999
            else:
                answer -= 199999


        # liczymy pionki
        


        return answer


    def simulate(self) -> str:
        """
        Performs MinMax search and returns the best move
        """
        game = Game(state=self.arena_properties)
        best_move = None
        best_value = float("-inf")
        legal_moves = game.get_legal_moves(self.side)

        for move in legal_moves:
            game_cp = game.copy()

            if self.side == 'left':
                game_cp.update(move, Wait("right"))
            else:
                game_cp.update(Wait("left"), move)

            value = self.min_max_search(game_cp, 3, negate_player(self.side))

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
