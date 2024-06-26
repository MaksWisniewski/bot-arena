from .stats import COST
from .stats import START_GOLD, PASSIVE_GOLD, FARM_GOLD
from .stats import SOLDIERS_STATS

from .actions import Action, BuildAction
from .actions import Wait, BuildTurret, BuildFarm, SpawnSoldier

from .objects.map import Map
from .objects.turrets import Turrets, Turret
from .objects.soldiers import Soldiers, Soldier
from .objects.farms import Farms, Farm

import os, random

from packages import MAPS_DIRECTORY

ErrorCode = {
    -5: 'Too many troops',
    -4: 'Wrong build place',
    -3: 'Same build place',
    -2: 'Wrong action',
    -1: 'Not enough gold',
    0: 'OK',
    1: 'Left win',
    2: 'Right win',
    3: 'Tie'
}

class Game:
    def __init__(self, map_path = None, state = None) -> None:
        map_path = os.path.join(MAPS_DIRECTORY, map_path) if map_path is not None else None

        self._map = Map(map_path)

        self.turrets = {
            'left': Turrets(self._map.path),
            'right': Turrets(self._map.path)
        }

        self.soldiers  = {
            'left': Soldiers('left', self._map.path),
            'right': Soldiers('right', self._map.path)
        }

        self.farms = {
            'left': Farms(self._map.path),
            'right': Farms(self._map.path)
        }

        self.gold = {
            'left': START_GOLD,
            'right': START_GOLD
        }

        self.income = {
            'left': PASSIVE_GOLD,
            'right': PASSIVE_GOLD
        }

        if state is not None:
            self.__load_state(state)

        self.action_left = Wait('left')
        self.action_right = Wait('right')

    def __load_state(self, state) -> None:
        self._map.load_from_dict(state['arena'])

        players = state['players']
        for player in players:
            buildings = players[player]['buildings']
            for turret_cords in buildings['turrets']:
                self.turrets[player].spawn(turret_cords)

            for farm_cords in buildings['farms']:
                self.farms[player].spawn(farm_cords)

            for unit_stats in players[player]['units']:
                self.soldiers[player].spawn_with_stats(unit_stats)

            self.gold[player] = players[player]['gold']

            self.income[player] = players[player]['income']


    def __update_soldiers(self) -> None:
        self.soldiers['left'].fight(self.soldiers['right'])
        self.soldiers['right'].fight(self.soldiers['left'])

        self.soldiers['left'].move()
        self.soldiers['right'].move()

    def __shoot_turrets(self) -> None:
        self.turrets['left'].shoot(self.soldiers['right'])
        self.turrets['right'].shoot(self.soldiers['left'])

    def __handle_actions_error(self) -> tuple[int, int]:
        def check_build_place(action: Action) -> int:
            if isinstance(action, BuildTurret) and self.gold[action.side] < COST['turret']:
                return -1
            if isinstance(action, BuildFarm) and self.gold[action.side] < COST['farm']:
                return -1
            if action.cords[0] < 0 or action.cords[0] >= self._map.MAP_SIZE_X or action.cords[1] < 0 or action.cords[1] >= self._map.MAP_SIZE_Y:
                return -4

            wrong_build_places = [
                self._map.obstacles,
                self._map.path,
                self.turrets['left'],
                self.turrets['right'],
                self.farms['left'],
                self.farms['right']
                ]

            for place in wrong_build_places:
                if action.cords in place:
                    return -4

            return 0

        def check_spawn_soldier(action: Action) -> int:
            soldier_name = action.name
            if self.gold[action.side] < SOLDIERS_STATS[soldier_name]['cost']:
                return -1
            if not self.soldiers[action.side].can_spawn():
                return -5
            return 0

        left_error = check_build_place(self.action_left) if isinstance(self.action_left, BuildAction) else 0
        right_error = check_build_place(self.action_right) if isinstance(self.action_right, BuildAction) else 0

        # if both can build and same build place
        if (left_error, right_error) == (0, 0) and isinstance(self.action_left, BuildAction) and isinstance(self.action_right, BuildAction):
            if self.action_left.cords == self.action_right.cords:
                return (-3, -3)

        left_error = check_spawn_soldier(self.action_left) if isinstance(self.action_left, SpawnSoldier) else left_error
        right_error = check_spawn_soldier(self.action_right) if isinstance(self.action_right, SpawnSoldier) else right_error

        return (left_error, right_error)

    def __execute_actions(self) -> None:
        def build(action: Action) -> None:
            if isinstance(action, BuildTurret):
                self.gold[action.side] -= COST['turret']
                self.turrets[action.side].spawn(action.cords)
                return
            if isinstance(action, BuildFarm):
                self.gold[action.side] -= COST['farm']
                self.farms[action.side].spawn(action.cords)
                return

        def spawn(action: Action) -> None:
            soldier_name = action.name
            self.gold[action.side] -= SOLDIERS_STATS[soldier_name]['cost']
            self.soldiers[action.side].spawn(soldier_name)

        action_to_function = {
            BuildTurret: build,
            BuildFarm: build,
            SpawnSoldier: spawn,
            Wait: lambda action: None
        }

        action_to_function[self.action_left.__class__](self.action_left)
        action_to_function[self.action_right.__class__](self.action_right)

    def __is_win(self) -> tuple[bool, bool]:
        left_win = self.soldiers['left'].is_win
        right_win = self.soldiers['right'].is_win

        if left_win and right_win:
            return (ErrorCode[3], ErrorCode[3])
        if left_win:
            return (ErrorCode[1], ErrorCode[1])
        if right_win:
            return (ErrorCode[2], ErrorCode[2])

        return None

    def is_win(self):
        return self.__is_win()

    def update(self, action_left: Action, action_right: Action) -> tuple[str, str]:
        self.__update_soldiers()
        self.__shoot_turrets()
        self.soldiers['left'].clear_dead()
        self.soldiers['right'].clear_dead()

        self.action_left = action_left
        self.action_right = action_right
        Error = self.__handle_actions_error()
        self.action_left = Wait('left') if Error[0] else self.action_left
        self.action_right = Wait('right') if Error[1] else self.action_right

        self.__execute_actions()

        self.gold['left'] += self.income['left']
        self.gold['right'] += self.income['right']

        self.income['left'] = len(self.farms['left']) * FARM_GOLD + PASSIVE_GOLD
        self.income['right'] = len(self.farms['right']) * FARM_GOLD + PASSIVE_GOLD

        WinLog = self.__is_win()
        if WinLog:
            self.update = lambda action_left, action_right: self.__is_win()
            return WinLog

        return (ErrorCode[Error[0]], ErrorCode[Error[1]])

    def get_path(self) -> list[tuple[int, int]]:
        return self._map.path

    def get_obstacles(self) -> list[tuple[int, int]]:
        return self._map.obstacles.obstacles

    def get_map_size(self) -> tuple[int, int]:
        return (self._map.MAP_SIZE_X, self._map.MAP_SIZE_Y)

    def get_empty_cells(self) -> list[tuple[int, int]]:
        path_cells = self.get_path()
        obstacle_cells = [tuple(obstacle.cords) for obstacle in self.get_obstacles()]
        non_empty_cells = obstacle_cells + path_cells

        farms = self.get_farms()
        turrets = self.get_turrets()
        for player in ['left', 'right']:
            non_empty_cells += [tuple(farm.get_coordinates()) for farm in farms[player]]
            non_empty_cells += [tuple(turret.get_coordinates()) for turret in turrets[player]]

        size_x, size_y = self.get_map_size()

        return [(x,y) for x in range(size_x) for y in range(size_y) if (x,y) not in non_empty_cells]

    def get_soldiers(self) -> dict[str, list[Soldier]]:
        return {
            'left': self.soldiers['left'].soldiers,
            'right': self.soldiers['right'].soldiers
        }

    def get_turrets(self) -> dict[str, list[Turret]]:
        return {
            'left': self.turrets["left"].turrets,
            'right': self.turrets["right"].turrets
        }

    def get_farms(self) -> dict[str, list[Farm]]:
        return {
            'left': self.farms['left'].farms,
            'right': self.farms['right'].farms
        }

    def get_gold(self) -> dict[str, int]:
        return self.gold.copy()

    def get_income(self) -> dict[str, int]:
        return self.income.copy()

    def get_legal_moves(self, player: str) -> list[Action]:
        moves = [Wait(player)]

        # spawn soldier
        for soldier_type in SOLDIERS_STATS:
            if SOLDIERS_STATS[soldier_type]['cost'] <= self.gold[player]:
                moves.append(SpawnSoldier(player, soldier_type))

        # build
        for building_type in COST:
            if COST[building_type] <= self.gold[player]:
                ActionClass = BuildFarm if building_type == 'farm' else BuildTurret
                moves += [ActionClass(player, x, y) for x, y in self.get_empty_cells()]

        return moves

    def get_random_move(self, player: str) -> Action:
        return random.choice(self.get_legal_moves(player))

    def generate_random_map(self, map_path, size_x = 10, size_y = 10) -> None:
        self._map.generate_random_map(map_path, size_x, size_y)

    def display(self) -> None:
        for turret in self.turrets['left'].turrets:
            print(turret.cords, end=' ')
        print()

        for i in range(self._map.MAP_SIZE_Y):
            for j in range(self._map.MAP_SIZE_X):
                try:
                    if (j, i) in self.soldiers['left']:
                        print('l', end='')
                    elif (j, i) in self.soldiers['right']:
                        print('r', end='')
                    elif (j, i) in self._map.path:
                        print('_', end='')
                    elif (j, i) in self.turrets['left']:
                        print('L', end='')
                    elif (j, i) in self.turrets['right']:
                        print('R', end='')
                    else:
                        print('.', end='')
                except:
                    if (j, i) in self._map.path:
                        print('O', end='')
                    else:
                        print('.', end='')

            print()

    def copy(self):
        game = Game()
        game._map = self._map.copy()
        game.turrets = {
            'left': self.turrets['left'].copy(),
            'right': self.turrets['right'].copy()
        }
        game.soldiers = {
            'left': self.soldiers['left'].copy(),
            'right': self.soldiers['right'].copy()
        }
        game.farms = {
            'left': self.farms['left'].copy(),
            'right': self.farms['right'].copy()
        }
        game.gold = self.gold.copy()
        game.income = self.income.copy()
        game.action_left = self.action_left
        game.action_right = self.action_right
        return game
