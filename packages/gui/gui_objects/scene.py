from .gui_object import GUIobject
from pygame import Surface, Vector2
from .window import Window
from pygame.event import Event
from pygame import MOUSEBUTTONDOWN, KEYDOWN, BUTTON_LEFT

class Scene(GUIobject):
    def __init__(self, sub_objects: list['GUIobject'], **kwargs):
        super().__init__(sub_objects, (0, 0), (1, 1), **kwargs)
        self.surf = None
        self.supported_events = [MOUSEBUTTONDOWN, KEYDOWN]

    def get_surf(self) -> Surface:
        return self.surf

    def render(self) -> Surface:
        surf = Surface(self.real_size)
        for object in self.sub_objects:
            surf.blit(object.render(), object.real_pos)
        self.surf = surf
        return surf
    
    def calc_pos(self, global_pos: tuple, screen_size: tuple) -> None:
        global_pos = Vector2(global_pos)
        screen_size = Vector2(screen_size)

        self.real_pos = Vector2(
            global_pos.x + screen_size.x * self.pos.x,
            global_pos.y + screen_size.y * self.pos.y
        )
        self.real_size = Vector2(
            screen_size.x * self.size.x,
            screen_size.y * self.size.y
        )
        self.global_pos = global_pos

        queue = [self]
        while queue != []:
            window = queue.pop(0)
            for object in window.sub_objects:
                object.real_pos = Vector2(
                    window.real_size.x * object.pos.x,
                    window.real_size.y * object.pos.y
                )
                object.real_size = Vector2(
                    window.real_size.x * object.size.x,
                    window.real_size.y * object.size.y
                )
                object.global_pos = window.global_pos + object.real_pos
                if isinstance(object, Window):
                    queue.append(object)

    def handle_event(self, event: Event) -> None:
        if event.type not in self.supported_events:
            return
        if event.type == MOUSEBUTTONDOWN:
            if event.button != BUTTON_LEFT:
                return
        for object in self.sub_objects:
            object.handle_event(event)

    def update(self, dt) -> None:
        for object in self.sub_objects:
            object.update(dt)