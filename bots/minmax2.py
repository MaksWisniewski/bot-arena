import sys, os
import numpy as np

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from bot_package.bot import Bot
from packages.game_logic.game import Game
from packages.game_logic.actions import Wait

from packages.simulator.serializer import Serializer


def negate_player(x: str):
    return "right" if x == "left" else "left" if x == "right" else ""


class MinMax_Bot(Bot):
    def preprocess(self) -> None:
        self.opponent_side = negate_player(self.side)


    def min_max_search(self, game: Game, depth: int, _player: str) -> float:
        def browse_moves(__maximizing_player: bool):
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

        return browse_moves(_player == self.side)


    def evaluate(self, game: Game) -> float:
        """
        Evaluation function to estimate game state
        """
        answer = 0

        # jesli wygranko to premiuj funkcje
        if game.is_win() is not None and 'Tie' not in game.is_win():
            am_i_left = 'left' == self.side
            left_wins = 'Left win' in game.is_win()

            if am_i_left == left_wins:
                answer += 199999
            else:
                answer -= 199999

        turrets = game.get_turrets()
        farms = game.get_farms()
        soldiers = game.get_soldiers()

        answer += len(farms[self.side]) * 7
        answer += len(soldiers[self.side]) * 3

        answer -= len(farms[self.opponent_side]) * 7
        answer -= len(soldiers[self.opponent_side]) * 3

        my_turrets = turrets[self.side]
        opponent_turrets = turrets[self.opponent_side]

        end = 0;
        start = game.get_path()[-1][0]

        if self.side == "left":
            end = game.get_path()[-1][0]
            start = 0
            # print(f"LEWY pozycja końca ścieżki: {end}", file=sys.stderr)
        for my_turret in my_turrets:
            distance = abs(my_turret.cords[0] - end) + abs(my_turret.cords[1] - end)
            answer -= distance

        for opponent_turret in opponent_turrets:
            distance = abs(opponent_turret.cords[0] - start) + abs(opponent_turret.cords[1] - start)
            answer -= distance



        return answer


    def simulate(self, _depth = 3) -> str:
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

            value = self.min_max_search(game_cp, _depth, self.opponent_side)

            if value > best_value:
                best_value = value
                best_move = move

        print(f"MINMAX: best move: {best_move}", file=sys.stderr)
        return best_move


    def make_move(self) -> str:
        # perform simulation and chose the best move
        return self.simulate()


    def post_move_action(self) -> None:
        return super().post_move_action()


while True:
    MinMax_Bot().run()
