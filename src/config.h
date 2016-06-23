/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#undef ENABLE_LEAKCHECK           // Memory leak check
#undef ENABLE_PTRCHECK           // Check validity of pointers too
#undef ENABLE_PTRCHECK_HISTORY   // Slower
#undef ENABLE_WID_PTRCHECK       // Check validity of pointers too
#undef ENABLE_THING_SANITY       // Check what things carry

#undef ENABLE_MAZE_DEBUG          // Print it as generating
#undef ENABLE_MAP_SANITY          // Extra map checks
#undef ENABLE_ASSERT              // DIE on errors
#undef ENABLE_TREE_SANITY         // Slow tree sanity
#undef ENABLE_DEBUG               // More debugs
#undef ENABLE_DEMARSHAL_DEBUG     // Demarshalling, prints structures
#undef ENABLE_MAP_DEBUG           // Debug AI by printing the map
#undef ENABLE_WID_DEBUG           // Widget debug
#undef ENABLE_CONSOLE

#undef ENABLE_REDRAW_LIGHT_SOURCES
#undef ENABLE_LIGHT_ALL_UNDER_LIGHT_SOURCE
#undef ENABLE_BLACK_AND_WHITE

#define TICKS_PER_SEC             100

/*
 * Handles map updates and timer ticks.
 */
#define LEVEL_TICK_DELAY_TENTHS   1

/*
 * Keep loading files simple and in the dir we expect.
 */
#undef ENABLE_DIR_TRAVERSAL_IN_WIDGETS

/*
 * Other windows are darker when popups appear
 */
#undef ENABLE_FADE_OUT_OTHER_WIDGETS_WHEN_FOCUS_LOCKED

/*
 * Retro text
 */
#define ENABLE_LARGE_TEXT_OUTLINE

/*
 * How much the wheel mouse moves.
 */
#define ENABLE_WHEEL_SCROLL_SPEED_SCALE         1.2
#define ENABLE_WHEEL_MAX_SCROLL_SPEED_SCALE     15

#define ENABLE_JOY_SCROLL_SPEED_SCALE           1.0
#define ENABLE_JOY_MAX_SCROLL_SPEED_SCALE       1.5

/*
 * Tooltip appears above mouse. I prefer it to zoom into the top
 */
#undef ENABLE_TOOLTIP_OVER_MOUSE

/*
 * Tries to batch calls to draw arrays for more speed.
 */
#define ENABLE_GL_BULK_DRAW_ARRAYS 

/*
 * So we don't hog all the CPU when not in video sync mode
 */
#define MAIN_LOOP_DELAY             10

/*
 * Max long string size
 */
#define MAXSTR                      1024

/*
 * Light sources
 */
#define MAX_LIGHT_RAYS              180
#define MAX_LIGHTS                  8000

/*
 * Screen sizes.
 */
#define DEFAULT_VIDEO_WIDTH         1008
#define DEFAULT_VIDEO_HEIGHT        672

/*
 * Map. How many tiles across and down.
 */
#define LEVELS_ACROSS               10
#define LEVELS_DOWN                 10
#define TEST_LEVEL                  101

/*
 * How many tiles per map
 */
#define JIGPIECE_WIDTH              10
#define JIGPIECE_HEIGHT             8

#define MAP_JIGSAW_PIECES_ACROSS    4
#define MAP_JIGSAW_PIECES_DOWN      4
#define MAP_BORDER                  4

#define MAP_WIDTH                   (((MAP_JIGSAW_PIECES_ACROSS)*JIGPIECE_WIDTH)+4)
#define MAP_HEIGHT                  (((MAP_JIGSAW_PIECES_DOWN)*JIGPIECE_HEIGHT)+4)

#define WORLD_WIDTH                 1024
#define WORLD_HEIGHT                1024

#define FLUID_RESOLUTION            8
#define FLUID_VISIBLE_SCALE         1.0
#define FLUID_WIDTH                 (MAP_WIDTH * FLUID_RESOLUTION)
#define FLUID_HEIGHT                (MAP_HEIGHT * FLUID_RESOLUTION)
#define FLUID_MAX_MASS              64
#define MAX_FLUID_TILES             64 /* tiles of animation */

/*
 * The number of tiles per screen.
 */
#define TILES_SCREEN_WIDTH          23
#define TILES_SCREEN_HEIGHT         14
#define TILES_ANIM_MAX              10000

/*
 * Torchlight flickering parameters.
 */
#define MAP_LIGHT_CANDLELIGHT_FLICKER_RATE      20

/*
 * Any more than this and you can peek over walls.
 */
#define MAP_LIGHT_CANDLELIGHT_FLICKER_RADIUS    20
#define MAP_LIGHT_CANDLELIGHT_RADIUS            30

#define MAP_THINGS_PER_CELL         250

/*
 * Tile pixel width.
 */
#define TILE_WIDTH                  64
#define TILE_HEIGHT                 64

/*
 * The width of wall blocks and floor tiles not counting shadow pixels
 */
#define TILE_PIX_WIDTH              24
#define TILE_PIX_HEIGHT             24

/*
 * The width including shadow pixels.
 */
#define TILE_PIX_WITH_SHADOW_WIDTH  TILE_WIDTH
#define TILE_PIX_WITH_SHADOW_HEIGHT TILE_HEIGHT

/*
 * Padding for tiles on the edge of the smooth scroll. Has to be quite wide so 
 * that we can see the light from other player when off screen.
 */
#define TILES_SCREEN_LIGHT_WIDTH_PAD      8
#define TILES_SCREEN_LIGHT_HEIGHT_PAD     8

/*
 * But for fluid we want it to be as few tiles as we can as we have to do a
 * lot of water rendering.
 */
#define TILES_SCREEN_FLUID_WIDTH_PAD      2
#define TILES_SCREEN_FLUID_HEIGHT_PAD     2

/*
 * Floor tiles occupy more space.
 */
#define TILE_FLOOR_SCALE                  2

/*
 * Console
 */
#define CONSOLE_CURSOR_COLOR        PURPLE
#define CONSOLE_CURSOR_OTHER_COLOR  PLUM
#define CONSOLE_TEXT_COLOR          GREEN
#define CONSOLE_HEIGHT              1000

/*
 * Fonts
 */
#undef ENABLE_GENERATE_TTF         // Edit RUNME to add -lSDL_ttf

#define FIXED_FONT                  "data/ttf/monaco.ttf"
#define VSMALL_FONT                 "data/ttf/monaco.ttf"
#define SMALL_FONT                  "data/ttf/tekton.ttf"
#define MED_FONT                    "data/ttf/tekton.ttf"
#define LARGE_FONT                  "data/ttf/tekton.ttf"
#define VLARGE_FONT                 "data/ttf/tekton.ttf"
#define VVLARGE_FONT                "data/ttf/tekton.ttf"

#define VSMALL_FONT_SIZE            10
#define SMALL_FONT_SIZE             12
#define FIXED_FONT_SIZE             14
#define MED_FONT_SIZE               20
#define LARGE_FONT_SIZE             20
#define VLARGE_FONT_SIZE            20
#define VVLARGE_FONT_SIZE           28
#define MAX_FONT_SIZE               60

/*
 * Messages.
 */
#define MAX_HISCORES                10
#define SMALL_STRING_LEN_MAX        30
#define PLAYER_MSG_MAX              100

/*
 * Maximum number of thing types. This cannot be exceeded without changing
 * the message format in tx updates.
 */
#define THING_MAX                   1024

/*
 * We will not spawn any monsters above this limit; to keep the game fast.
 */
#define THING_MAX_MONSTS_SPAWN_LIMIT 300

#ifdef ENABLE_THING_SANITY
#define THING_SANITY(level, t) thing_sanity(level, t)
#else
#define THING_SANITY(level, t)
#endif

/*
 * Thumb in air. Needs to be big enough so wrap arounds and id reuse is less 
 * common.
 */
#define MAX_THINGS_PER_LEVEL        0x1fff
#define THING_ID_RAND_MASK          0xffff0000
#define MAX_TIMERS_PER_LEVEL        1000

/*
 * Timers
 */
#define DELAY_TENTHS_THING_COLLISION_TEST                   1

/*
 * How often we look at the keyboards
 */
#define DELAY_THOUSANDTHS_PLAYER_POLL                       15

#define DELAY_LEVEL_END_HIDE                                (ONESEC * 1)
#define DELAY_LEVEL_END_DESTROY                             (ONESEC * 2)

enum {
    DMAP_MAP_NONE,
    DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_PASSABLE,
    DMAP_MAP_PLAYER_TARGET_TREAT_DOORS_AS_WALLS,
};

#define DMAP_MAP_MAX 8

void set_game_video_pix_width(int width);
int get_game_video_pix_width(void);
void set_game_video_pix_height(int height);
int get_game_video_pix_height(void);
void set_game_sound_volume(int sound_volume);
int get_game_sound_volume(void);
void set_game_music_volume(int music_volume);
int get_game_music_volume(void);
void set_game_display_sync(int display_sync);
int get_game_display_sync(void);
void set_game_full_screen(int full_screen);
int get_game_full_screen(void);
void set_game_fps_counter(int fps_counter);
int get_game_fps_counter(void);
void save_game_config(void);
