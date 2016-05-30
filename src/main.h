/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "config.h"

/*
 * Enhanced random number generator.
 */
#include "pcg_basic.h"
#define myrand() pcg32_random()
#define mysrand(a) pcg32_srandom(a,a)
//#define myrand() rand()
//#define mysrand(a) srand(a)

#ifdef _WIN32
#include <windows.h>

static inline void uSleep (int waitTime) 
{
    __int64 time1 = 0, time2 = 0, freq = 0;

    QueryPerformanceCounter((LARGE_INTEGER *) &time1);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);

    do {
        QueryPerformanceCounter((LARGE_INTEGER *) &time2);
    } while((time2-time1) < waitTime);
}
#else
#define uSleep usleep
#endif

/*
 * Misc
 */
#undef ONESEC
#define ONESEC                          1000

#ifndef true
#define true                            1
#endif

#ifndef false
#define false                           0
#endif

#ifndef max
#define max(a,b)                        (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)                        (((a) < (b)) ? (a) : (b))
#endif

#define swap(a,b) {                                 \
            typeof (a) c;                           \
            c = a;                                  \
            a = b;                                  \
            b = c;                                  \
        }

/*
 * Simple array routines
 */
#define ARRAY_SIZE(_array_)             (sizeof(_array_)/sizeof(_array_[0]))

#define FOR_ALL_IN_ARRAY(iterator, _array_)                                 \
    typeof(&_array_[0]) iterator;                                           \
    for ((iterator) = (_array_);                                            \
         (iterator) < ((_array_) + ARRAY_SIZE(_array_));                    \
         (iterator)++)

#define FOR_NEXT_IN_ARRAY(iterator, _array_)                                \
    for ((iterator)++;                                                      \
         (iterator) < ((_array_) + ARRAY_SIZE(_array_));                    \
         (iterator)++)

/*
 * GCC extension for offset
 */
#ifdef __GNUC__
#if defined(__GNUC__) && __GNUC__ > 3
#define STRUCT_OFFSET(STRUCT, MEMBER) __builtin_offsetof(STRUCT, MEMBER)
#else
#define STRUCT_OFFSET(STRUCT, MEMBER) \
                     ((size_t) ( (char *)&((st *)0)->m - (char *)0 ))
#endif
#else
#define STRUCT_OFFSET(STRUCT, MEMBER) \
                     ((size_t) ( (char *)&((st *)0)->m - (char *)0 ))
#endif

#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

/*
 * Types
 */
#ifndef __int8_t_defined
#ifndef _INT8_T
#define _INT8_T
typedef signed char          int8_t;
#endif /*_INT8_T */

#ifndef _INT16_T
#define _INT16_T
typedef short                int16_t;
#endif /* _INT16_T */

#ifndef _INT32_T
#define _INT32_T
typedef int                  int32_t;
#endif /* _INT32_T */

#ifndef _INT64_T
#define _INT64_T
typedef long long            int64_t;
#endif /* _INT64_T */

#ifndef _UINT8_T
#define _UINT8_T
typedef unsigned char        uint8_t;
#endif /*_UINT8_T */

#ifndef _UINT16_T
#define _UINT16_T
typedef unsigned short       uint16_t;
#endif /* _UINT16_T */

#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned int         uint32_t;
#endif /* _UINT32_T */

#ifndef _UINT64_T
#define _UINT64_T
typedef unsigned long long   uint64_t;
#endif /* _UINT64_T */
#endif

/*
 * msys functions seem to accept either / or \ so we don't need to worry.
 */
#define DSEP "/"
#define DCHAR '/'

#if defined(__MINGW32__) || defined(__linux__)
/*
 * SDL provides SDL_strlcat so linking will work. This just fixes the
 * warnings.
 */
uint32_t strlcpy(char *dst, const char *src, uint32_t maxlen);
uint32_t strlcat(char *dst, const char *src, uint32_t maxlen);
#endif

/*
 * Globals:
 */
typedef struct net_ *netp;
typedef struct font *fontp;
typedef struct wid_console *wid_consolep;
typedef struct game_ *gamep;
typedef struct wid_ *widp;
typedef struct tree_root_ *tree_rootp;
typedef struct tex_ *texp;
typedef struct music_ *musicp;
typedef struct sound_ *soundp;
typedef struct tile_ *tilep;
typedef struct thing_ *thingp;
typedef struct action_timer_ *timerp;
typedef struct action_init_fn_ *init_fnp;
typedef struct tp_ *tpp;
typedef struct tpp_data_ *tpp_data;
typedef struct thing_tile_ *thing_tilep;
typedef struct tokens_t_ *tokenp;
typedef struct tree_demarshal_ *demarshal_p;
typedef struct tree_marshal_ *marshal_p;
typedef struct tokens_t_ *tokensp;
typedef struct level_t_ *levelp;
typedef struct map_t_ *mapp;

extern thingp player;
extern fontp vsmall_font;
extern fontp fixed_font;
extern fontp small_font;
extern fontp med_font;
extern fontp large_font;
extern fontp vlarge_font;
extern fontp vvlarge_font;
extern wid_consolep wid_console;
extern gamep config;

typedef uint8_t (*tp_is_callback)(tpp);

/*
 * log.c
 */
#define DIE(args...)                                                          \
    DYING("Died at %s:%s():%u", __FILE__, __FUNCTION__, __LINE__);            \
    CROAK(args);                                                              \
    exit(1);

extern uint8_t croaked;

#ifdef ENABLE_ASSERT
#define ASSERT(x)                                                             \
    if (!(x)) {                                                               \
        DIE("Failed assert");                                                 \
    }
#else
#define ASSERT(x)
#endif

void CROAK(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void DYING(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void LOG(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void LOGS(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void WARN(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void CON(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

enum {
    INFO,
    WARNING,
    CRITICAL,
    POPUP,
    OVER_THING,
    SOUND,
    SOUND_GLOBAL,
};

/*
 * Bloody windows uses MSG hence MESG
 */
void MESG(uint32_t level, const char *fmt, ...) 
    __attribute__ ((format (printf, 2, 3)));

void SDL_MSG_BOX(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void MSG_BOX(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void ERR(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
void DBG(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));

void MSG_SHOUT_AT(uint32_t level,
                  thingp,
                  double x,
                  double y,
                  const char *fmt, ...)
                  __attribute__ ((format (printf, 5, 6)));

void THING_LOG(thingp, const char *fmt, ...)
                     __attribute__ ((format (printf, 2, 3)));
void THING_CON(thingp, const char *fmt, ...)
                     __attribute__ ((format (printf, 2, 3)));
void THING_ERR(thingp, const char *fmt, ...)
                     __attribute__ ((format (printf, 2, 3)));
void THING_DBG(thingp, const char *fmt, ...)
                     __attribute__ ((format (printf, 2, 3)));
void LEVEL_LOG(levelp, const char *fmt, ...)
                     __attribute__ ((format (printf, 2, 3)));

#ifdef ENABLE_WID_DEBUG
void WID_LOG(widp, const char *fmt, ...) \
                     __attribute__ ((format (printf, 2, 3)));

void WID_DBG(widp, const char *fmt, ...) \
                     __attribute__ ((format (printf, 2, 3)));
#else
#define WID_DBG(...)
#define WID_LOG(...)
#endif

extern uint8_t debug_enabled;
uint8_t debug_enable(tokensp, void *context);
uint8_t fps_enable(tokensp, void *context);

/*
 * util.c
 */
void *myzalloc_(uint32_t size, const char *what, const char *func,
                const char *file, const uint32_t line);

void *mymalloc_(uint32_t size, const char *what, const char *func,
                const char *file, const uint32_t line);

void *myrealloc_(void *ptr, uint32_t size, const char *what, const char *func,
                 const char *file, const uint32_t line);


void myfree_(void *ptr, const char *func, const char *file,
             const uint32_t line);

char *dupstr_(const char *in, const char *what, const char *func,
              const char *file, const uint32_t line);

#include "ptrcheck.h"

/*
 * point.c
 */
typedef struct {
    double x;
    double y;
} fpoint;

typedef struct {
    int32_t x;
    int32_t y;
} point;

typedef struct {
    double x;
    double y;
    double z;
} fpoint3d;

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} point3d;

typedef struct {
    int32_t width;
    int32_t height;
} size;

typedef struct {
    double width;
    double height;
} fsize;

/*
 * main.c
 */
extern FILE *LOG_STDOUT;
extern FILE *LOG_STDERR;

#define MY_STDOUT (LOG_STDOUT ? LOG_STDOUT : stdout)
#define MY_STDERR (LOG_STDERR ? LOG_STDERR : stderr)

extern char *EXEC_FULL_PATH_AND_NAME;
extern char *EXEC_DIR;
extern char *DATA_PATH;
extern char *LEVELS_PATH;
extern char *GFX_PATH;
extern char *TTF_PATH;
extern uint8_t quitting;

void quit(void);
void restart(void);
void die(void);
void tree_debug_test(void);
uint8_t miniz_test(int32_t argc, char *argv[]);
uint8_t mzip_file_test(int32_t argc, char *argv[]);
uint8_t dir_test(int32_t argc, char *argv[]);
uint8_t string_test(int32_t argc, char *argv[]);
uint8_t marshal_test(int32_t argc, char *argv[]);
uint8_t enum_test(int32_t argc, char *argv[]);
uint8_t ptrcheck_test(int32_t argc, char *argv[]);

/*
 * config.c
 */
void config_fini(void);
uint8_t config_init(void);
uint8_t config_save(void);
uint8_t config_load(void);

/*
 * color.c
 */
typedef struct {
    uint8_t r, g, b, a;
} color;

/*
 * mazegen.c
 */
int32_t map_jigsaw_test(int32_t argc, char **argv);

/*
 * main.c
 */
extern uint8_t opt_seed;

extern int debug;

#include "color.h"
#include "level.h"

struct game_ {
    level_t level;

    /*
     * Current level.
     */
    uint32_t level_no;

    /*
     * Current game tick.
     */
    uint32_t tick;

    widp wid_map;
    widp wid_grid;
    widp wid_horiz_scroll;
    widp wid_vert_scroll;

    widp wid_score_text;
    widp wid_score_textbox;

    widp wid_gold_text;
    widp wid_gold_textbox;

    widp wid_hp_text;
    widp wid_hp_icon;
    widp wid_drown_icon[10];

    widp wid_torches_icon;
    widp wid_torches_text;

    widp wid_ropes_text;
    widp wid_ropes_icon;

    widp wid_bombs_text;
    widp wid_bombs_icon;

    widp wid_keys_text;
    widp wid_keys_icon;

    int32_t video_pix_width;
    int32_t video_pix_height;
    int32_t video_gl_width;
    int32_t video_gl_height;
    float xscale;
    float yscale;
    int32_t sound_volume;
    int32_t music_volume;
    int32_t display_sync;
    int32_t full_screen;
    int32_t fps_counter;

    double tile_width;
    double tile_height;

    double last_playery;
    double last_playerx;

    uint8_t game_over:1;
    uint8_t level_is_being_loaded:1;
};

extern struct game_ game;

static inline levelp thing_levelp (uint32_t id) 
{
    levelp level = &game.level;

    verify(level);

    return(level);
}

static inline thingp id_to_thing (uint32_t id) 
{
    uint32_t index = id & ~(THING_ID_RAND_MASK);

    if (index >= MAX_THINGS_PER_LEVEL) {
        DIE("overflow in looking up IDs, id %08X, index %u", id, index);
    }

    levelp level = &game.level;
    verify(level);

    thingp t = &level->things[index];
    verify(t);

    if (!t->thing_id) {
        return (0);
    }

    if (t->thing_id != id) {
        DIE("thing id %08X magic mismatch %u vs %u", index, id, t->thing_id);
    }

    return (t);
}

#define SOUND_MAX 20

