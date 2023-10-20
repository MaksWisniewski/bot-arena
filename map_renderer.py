from pygame import Surface
from pygame import Vector2
from pygame import Color
from random import choice

from const import MAP_SIZE_PX, TILE_SIZE, MAP_SIZE_X, MAP_SIZE_Y

from map import Map

class MapRenderer:
    def __init__(self) -> None:
        self.map_texture = Surface(MAP_SIZE_PX)

    def render(self, assets: dict, map: Map) -> Surface:
        '''Drawing map tiles.
        '''

        TEXTURE_NAMES = {
            'path':  'tile_path',
            'farm': 'tile_farm'
        }

        filled_cords = []

        for tile_code, cords in map.structures.items():
            if tile_code != 'obstacles':
                for cord in cords:
                    self.map_texture.blit(
                        assets['tiles'][TEXTURE_NAMES[tile_code]], cord * TILE_SIZE)
                    filled_cords.append(cord)

        grass_cords = [
            Vector2(x, y) 
            for x in range(MAP_SIZE_X)
              for y in range(MAP_SIZE_Y)
                if not Vector2(x, y) in filled_cords
        ]
        
        for cord in grass_cords:
            self.map_texture.blit(
                assets['tiles']['tile_grass_' + self.__get_grass_turn(cord, map)], cord * TILE_SIZE)

        obstacle_cords = list(sorted(map.structures['obstacles'], key = lambda v: v.y))
        obstacles_to_render = []

        while obstacle_cords:
            render_cord, texture_name, covered_cords = self.__render_obstacle(obstacle_cords[0], assets, obstacle_cords)
            obstacles_to_render.append((render_cord, texture_name))
            for cord in covered_cords:
                obstacle_cords.remove(cord)
        
        for render_cord, texture_name in sorted(obstacles_to_render, key = lambda x: x[0].y):
            self.map_texture.blit(assets['obstacles'][texture_name], render_cord)

        return self.map_texture
    
    def __group_tiles(self, cord: Vector2, other_tiles: list[Vector2]) -> list[Vector2]:
        
        def recursive(actual_cord: Vector2, group: list[Vector2]) -> list[Vector2]:
            new_tiles = [v for v in self.__get_neighbouring_tiles(actual_cord, other_tiles) if v not in group]
            all_tiles = group + new_tiles
            for tile in new_tiles:
                all_tiles.extend(v for v in recursive(tile, all_tiles) if v not in all_tiles)
        
            return all_tiles

        tiles = recursive(cord, []) 
        return tiles
    
    def __get_tile_group_size(self, group: list[Vector2]) -> Vector2:
        max_x = max(group, key = lambda v: v.x).x
        max_y = max(group, key = lambda v: v.y).y
        min_x = min(group, key = lambda v: v.x).x
        min_y = min(group, key = lambda v: v.y).y
        return Vector2(max_x - min_x + 1, max_y - min_y + 1)

    def __get_tile_group_bottom_center(self, group: list[Vector2]) -> Vector2:
        max_x = max(group, key = lambda v: v.x).x
        min_x = min(group, key = lambda v: v.x).x
        max_y = max(group, key = lambda v: v.y).y 
        return Vector2(min_x + (max_x - min_x + 1) / 2, max_y + 1)
        
    def __render_obstacle(self, cord: Vector2, assets: dict, obstacles: list[Vector2]) -> list[Vector2]:
        
        tiles = self.__group_tiles(cord, obstacles)
        tiles_size = self.__get_tile_group_size(tiles) * TILE_SIZE
        size = Vector2(MAP_SIZE_PX) 
        while not (size.x <= tiles_size.x and size.y <= tiles_size.y):
            name = choice(list(assets['obstacles']))
            texture = assets['obstacles'][name]
            size = Vector2(texture.get_size())
        
        bottom_center = self.__get_tile_group_bottom_center(tiles) * TILE_SIZE

        render_pos = Vector2(
            int(bottom_center.x) - size.x // 2,
            int(bottom_center.y - size.y),
        )

        covered_top_left = Vector2(bottom_center.x - size.x // 2, bottom_center.y - size.y)
        covered_bottom_right = Vector2(bottom_center.x + size.x // 2, bottom_center.y)

        covered_cords = [
            cord for cord in tiles if 
                covered_top_left.x // TILE_SIZE - 3 <= cord.x <= covered_bottom_right.x // TILE_SIZE + 3 and \
                covered_top_left.y // TILE_SIZE - 3 <= cord.y <= covered_bottom_right.y // TILE_SIZE + 3
        ]

        return render_pos, name, covered_cords if covered_cords else tiles

    def __get_neighbouring_tiles(self, cord: Vector2, other_tiles: list[Vector2], **kwargs) -> list[Vector2]:
        r = ((x, y) for x in range(-1, 2) for y in range(-1, 2)) \
            if 'diagonal' in kwargs else ((1, 0), (-1, 0), (0, 1), (0, -1), (0, 0))

        return [
            Vector2(x, y) + (cord if 'only_offset' not in kwargs else Vector2(0, 0))
                for x, y in r if Vector2(x, y) + cord in other_tiles
        ]

    def __get_grass_turn(self, cord: Vector2, map: Map) -> str:
        '''Returns name of grass turn based on neighboring tiles.
        '''

        is_path = self.__get_neighbouring_tiles(cord, map.structures['path'], only_offset = True, diagonal = True)
        
        if not is_path:
            return 'center'

        turn = 'top' if (0, -1) in is_path else 'bot' if (0, 1) in is_path else ''
        turn += 'left' if (-1, 0) in is_path else 'right' if (1, 0) in is_path else ''

        if not turn:
            turn += ([v for key, v in {
                (-1, -1):   'd1',
                (1, -1):    'd2',
                (1, 1):     'd3',
                (-1, 1):    'd4',
            }.items() if key in is_path] + [''])[0]

        return turn