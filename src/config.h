/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#define ENABLE_LEAKCHECK           // Memory leak check
#define ENABLE_PTRCHECK           // Check validity of pointers too
#define ENABLE_PTRCHECK_HISTORY   // Slower
#define ENABLE_WID_PTRCHECK       // Check validity of pointers too
#define ENABLE_THING_SANITY       // Check what things carry

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
#define MAP_WIDTH                   256
#define MAP_HEIGHT                  256

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

#define VSMALL_FONT_SIZE            14
#define SMALL_FONT_SIZE             16
#define FIXED_FONT_SIZE             16
#define MED_FONT_SIZE               20
#define LARGE_FONT_SIZE             24
#define VLARGE_FONT_SIZE            32
#define VVLARGE_FONT_SIZE           36
#define MAX_FONT_SIZE               60

/*
 * Messages.
 */
#define SMALL_STRING_LEN_MAX        30

/*
 * Maximum number of thing types. This cannot be exceeded without changing
 * the message format in tx updates.
 */
#define TP_MAX                      10000

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
