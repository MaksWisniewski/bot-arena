from pygame import Vector2

SCREEN_SIZE = (SCREEN_SIZE_X, SCREEN_SIZE_Y) = (1280, 720)
FRAMERATE = 60
TILE_SIZE = 64
CAMERA_OFFSET_MOVE_AREA = 0.1 # 0 -> 1 realtive to draw_screen_size
CAMERA_OFFSET_SPEED = 3
CAMERA_SMOOTH = 10
SPRITE_SIZE = 64
ANIMATION_LEN = 5
ANIMATION_NAMES = ('walk', 'fight')
ANIMATION_DIRECTIONS = ('top','right','bot')
ANIMATION_SPEED = 15 # frames per second
PARTICLE_DIRECTION_PRECISION = 100
STANDARD_PARTICLE_TIME = 10 # * 10
STANDARD_PARTICLE_SPEED = 3
STANDARD_PARTICLE_SIZE = 1
STANDARD_PARTICLE_ACC = 0.95
HP_BAR_SMOOTH = 5
MOUSE_TARGET_RADIUS = 30
FONT_SIZE = {'small': 8, 'medium': 10, 'large': 15}
FONT_COLOR = (255, 255, 255)
INFO_TAB_SHOW_TIME = 100
INFO_TAB_HIDE_SPEED = 3
INFO_TAB_SHOW_SMOOTH = 3
SHOW_REAL_POS = True
INFO_TAB_MARGIN = 5
MOVE_PRECISION = 10
PROJECTILE_SPEED = 4
ZOOM_INTERWAL = 0.1
MIN_ZOOM = 0.5
MAX_ZOOM = 4
HEALTH_BAR_SIZE = (TILE_SIZE // 2, TILE_SIZE // (2 * 6.4))
HEALTH_BAR_COLOR_BACK = (255, 0, 0)
HEALTH_BAR_COLOR_FRONT = (0, 255, 0)
OBSTACLE_TYPE_NUMBER = 41
PROJECTILE_SPEED = 5
DAMAGE_INFO_COLOR_MULT = 4
COLORS = {
    "brown": (87, 65, 47)
}
STATSBAR_BACKGROUND_COLOR = COLORS["brown"]

HEADER_BAR_SIZE = 0.036 * SCREEN_SIZE_Y