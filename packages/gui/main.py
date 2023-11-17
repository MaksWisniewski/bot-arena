from pygame import init, WINDOWCLOSE, Vector2, MOUSEWHEEL
from pygame.display import set_mode as display_set_mode, update as display_update
from pygame.time import Clock
from pygame.event import peek as event_peek
from os import name as os_name

from ..game_logic.game import Game
from .game_render.log_interpreter import LogInterpreter

from .gui_object import Window, Button, GameRender
from .mouse import Mouse
from .const import SCREEN_SIZE, FRAMERATE, STANDARD_FRAMERATE

class Main:
    def __init__(self, log_name: str):
        init()
        self.__screen = display_set_mode(SCREEN_SIZE)
        self.__is_running = True
        self.__dt = 1
        self.__clock = Clock()
        self.__tick = 0
        self.__game = Game()
        self.__mouse = Mouse()

        path = "/".join([dir for dir in __file__.split('\\') 
            if dir != ''][:-1]) + "/../../logs/" + log_name
        if os_name == 'posix':
            path = "./logs/" + log_name

        self.__log_interpreter = LogInterpreter(path)

        self.__gui = Window([
            Window([
                Button((0.1, 0.1), (0.2, 0.2)),
                Button((0.5, 0.5), (0.2, 0.2)),
            ], (0, 0), (0.2, 1)),
            Window([
                GameRender((0, 0), (1, 1), self.__game)
            ], (0.2, 0), (0.8, 0.8)),
            Window([
                Button((0.1, 0.1), (0.2, 0.2)),
                Button((0.5, 0.5), (0.2, 0.2)),
            ], (0.2, 0.8), (1, 0.2)),
        ], (0, 0), (1, 1))

        self.__gui.calc_pos(Vector2(SCREEN_SIZE), Vector2(0, 0))
        
        while self.__is_running:
            self.__mouse.update()
            self.__is_running = not event_peek(WINDOWCLOSE)
            self.__tick += self.__dt
            if self.__tick > FRAMERATE:
                game_output = self.__game.update(*self.__log_interpreter.get_next_actions())
                self.__tick = 0
            self.__screen_update()
 
    def __screen_update(self):
        '''Refreshes screen and update frame clock.
        '''
        self.__dt = self.__clock.tick(FRAMERATE) * STANDARD_FRAMERATE / 1000
        self.__screen.blit(self.__gui.get_surface(self.__dt, self.__mouse), (0, 0))
        display_update()

    def set_game_speed(self, value: float) -> None:
        '''Game speed setter.

        Value must be greater than 0 and lower or equal FRAMERATE.
        '''

        if value <= 0:
            raise Exception('Game speed must be greater than zero!')
        elif value > FRAMERATE:
            raise Exception('Game speed cannot be greater than FRAMERATE!')
        self.__game_speed = value

    def get_game_speed(self) -> float:
        '''Game speed getter.
        '''
        return self.__game_speed

if __name__ == "__main__":
    Main()