/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */

#include <libgen.h>
#include <unistd.h>
#include <SDL.h>
#include "slre.h"
#include <time.h>

#include "glapi.h"
#include "wid_console.h"
#include "command.h"
#include "ttf.h"
#include "font.h"
#include "math_util.h"
#include "tex.h"
#include "music.h"
#include "sound.h"
#include "tile.h"
#include "thing.h"
#include "wid.h"
#include "wid_intro_about.h"
#include "wid_game_over.h"
#include "wid_intro_settings.h"
#include "wid_hiscore.h"
#include "wid_intro.h"
#include "wid_game_map.h"
#include "string_util.h"
#include "dir.h"
#include "file.h"
#include "level.h"
#include "resource.h"
#include "init_fn.h"
#include "ramdisk.h"
#include "term.h"
#include "mzip_lib.h"
#include "map_jigsaw.h"
#include "thing_template.h"
#include "wid_editor.h"
#include "player.h"

static char **ARGV;
char *EXEC_FULL_PATH_AND_NAME;
char *EXEC_DIR;
char *DATA_PATH;
char *LEVELS_PATH;
char *TTF_PATH;
char *GFX_PATH;
int debug;

FILE *LOG_STDOUT;
FILE *LOG_STDERR;

uint8_t quitting;
uint8_t opt_seed = 1;

void quit (void)
{
    if (quitting) {
        return;
    }

    quitting = true;

    /*
     * Save the player name.
     */
    config_save();

    hiscore_save();

    sdl_exit();

    wid_game_map_fini();

    level_fini();
    player_fini();
    tp_fini();
    thing_fini();
    map_jigsaw_fini();

    wid_console_fini();
    wid_intro_fini();
    wid_intro_about_fini();
    wid_game_over_fini();
    wid_intro_settings_fini();
    wid_hiscore_fini();

    command_fini();

    wid_fini();
    ttf_fini();
    font_fini();
    tex_fini();
    music_fini();
    sound_fini();
    tile_fini();
    sdl_fini();
    config_fini();
    enum_fmt_destroy();
    enum_font_destroy();
    blit_fini();
    miniz_fini();

    if (EXEC_FULL_PATH_AND_NAME) {
        myfree(EXEC_FULL_PATH_AND_NAME);
        EXEC_FULL_PATH_AND_NAME = 0;
    }

    if (DATA_PATH) {
        myfree(DATA_PATH);
        DATA_PATH = 0;
    }

    if (LEVELS_PATH) {
        myfree(LEVELS_PATH);
        LEVELS_PATH = 0;
    }

    if (TTF_PATH) {
        myfree(TTF_PATH);
        TTF_PATH = 0;
    }

    if (GFX_PATH) {
        myfree(GFX_PATH);
        GFX_PATH = 0;
    }

#ifdef ENABLE_LEAKCHECK
    if (!croaked) {
        ptrcheck_fini();
    }
#endif

    term_fini();
}

void restart (void)
{
    char *args[] = { 0, 0 };
    char *executable = ARGV[0];

    LOG("Run %s", executable);

    args[0] = executable;

#ifdef _WIN32
    execve(executable, (const char *const *) args, 0);
#else
    execve(executable, (char *const *) args, 0);
#endif
}

void die (void)
{
    quit();

    LOG("Bye, error exit");
    fprintf(MY_STDERR, "exit(1) error\n");

    exit(1);
}

/*
 * Find the binary we are running.
 */
static void find_executable (void)
{
    char *parent_dir = 0;
    char *curr_dir = 0;
    char *exec_name = 0;
    char *exec_expanded_name = 0;
    char *path = 0;
    char *tmp;

    exec_name = mybasename(ARGV[0], __FUNCTION__);

    /*
     * Get the current directory, ending in a single /
     */
    curr_dir = dynprintf("%s" DSEP, dir_dot());
    tmp = strsub(curr_dir, DSEP DSEP, DSEP);
    myfree(curr_dir);
    curr_dir = tmp;

    /*
     * Get the parent directory, ending in a single /
     */
    parent_dir = dynprintf("%s" DSEP, dir_dotdot(dir_dot()));
    tmp = strsub(parent_dir, DSEP DSEP, DSEP);
    myfree(parent_dir);
    parent_dir = tmp;

    /*
     * Get rid of ../ from the program name and replace with the path.
     */
    exec_expanded_name = dupstr(ARGV[0], __FUNCTION__);
    if (*exec_expanded_name == '.') {
        tmp = strsub(exec_expanded_name, ".." DSEP, parent_dir);
        myfree(exec_expanded_name);
        exec_expanded_name = tmp;
    }

    /*
     * Get rid of ./ from the program name.
     */
    if (*exec_expanded_name == '.') {
        tmp = strsub(exec_expanded_name, "." DSEP, "");
        myfree(exec_expanded_name);
        exec_expanded_name = tmp;
    }

    /*
     * Get rid of any // from th path
     */
    tmp = strsub(exec_expanded_name, DSEP DSEP, DSEP);
    myfree(exec_expanded_name);
    exec_expanded_name = tmp;

    /*
     * Look in the simplest case first.
     */
    EXEC_FULL_PATH_AND_NAME = dynprintf("%s%s", curr_dir, exec_name);
    if (file_exists(EXEC_FULL_PATH_AND_NAME)) {
        EXEC_DIR = dupstr(curr_dir, "exec dir 1");
        goto cleanup;
    }

    myfree(EXEC_FULL_PATH_AND_NAME);

    /*
     * Try the parent dir.
     */
    EXEC_FULL_PATH_AND_NAME = dynprintf("%s%s", parent_dir, exec_name);
    if (file_exists(EXEC_FULL_PATH_AND_NAME)) {
        EXEC_DIR = dupstr(parent_dir, "exec dir 2");
        goto cleanup;
    }

    myfree(EXEC_FULL_PATH_AND_NAME);

    /*
     * Try the PATH.
     */
    path = getenv("PATH");
    if (path) {
        char *dir = 0;

        path = dupstr(path, "path");

        for (dir = strtok(path, ":"); dir; dir = strtok(0, ":")) {
            EXEC_FULL_PATH_AND_NAME = dynprintf("%s" DSEP "%s", dir, exec_name);
            if (file_exists(EXEC_FULL_PATH_AND_NAME)) {
                EXEC_DIR = dynprintf("%s" DSEP, dir);
                goto cleanup;
            }

            myfree(EXEC_FULL_PATH_AND_NAME);
        }

        myfree(path);
        path = 0;
    }

    EXEC_FULL_PATH_AND_NAME = dupstr(exec_expanded_name, "full path");
    EXEC_DIR = dupstr(dirname(exec_expanded_name), "exec dir");

cleanup:
    DBG("Exec name   : \"%s\"", exec_name);
    DBG("Parent dir  : \"%s\"", parent_dir);
    DBG("Curr dir    : \"%s\"", curr_dir);
    DBG("Full name   : \"%s\"", exec_expanded_name);

    if (path) {
        myfree(path);
    }

    if (exec_name) {
        myfree(exec_name);
    }

    if (exec_expanded_name) {
        myfree(exec_expanded_name);
    }

    if (parent_dir) {
        myfree(parent_dir);
    }
}

/*
 * Find all installed file locations.
 */
static void find_exec_dir (void)
{
    char *tmp;

    find_executable();

    /*
     * Make sure the exec dir ends in a /
     */
    tmp = dynprintf("%s" DSEP, EXEC_DIR);
    EXEC_DIR = strsub(tmp, DSEP DSEP, DSEP);
    myfree(tmp);
}

/*
 * Hunt down the data/ dir.
 */
static void find_data_dir (void)
{
    DATA_PATH = dynprintf("%sdata" DSEP, EXEC_DIR);
    if (dir_exists(DATA_PATH)) {
        return;
    }

    myfree(DATA_PATH);

    DATA_PATH = dupstr(EXEC_DIR, __FUNCTION__);
}

/*
 * Hunt down the level/ dir.
 */
static void find_level_dir (void)
{
    LEVELS_PATH = dynprintf("%sdata" DSEP "levels" DSEP, EXEC_DIR);
    if (dir_exists(LEVELS_PATH)) {
        return;
    }

    myfree(LEVELS_PATH);

    LEVELS_PATH = dupstr(EXEC_DIR, __FUNCTION__);
}

/*
 * Hunt down the ttf/ dir.
 */
static void find_ttf_dir (void)
{
    TTF_PATH = dynprintf("%sdata" DSEP "ttf" DSEP, EXEC_DIR);
    if (dir_exists(TTF_PATH)) {
        return;
    }

    myfree(TTF_PATH);

    TTF_PATH = dupstr(EXEC_DIR, __FUNCTION__);
}

/*
 * Hunt down the gfx/ dir.
 */
static void find_gfx_dir (void)
{
    GFX_PATH = dynprintf("%sdata" DSEP "gfx" DSEP, EXEC_DIR);
    if (dir_exists(GFX_PATH)) {
        return;
    }

    myfree(GFX_PATH);

    GFX_PATH = dupstr(EXEC_DIR, __FUNCTION__);
}

/*
 * Find all installed file locations.
 */
static void find_file_locations (void)
{
    find_exec_dir();
    find_data_dir();
    find_level_dir();
    find_ttf_dir();
    find_gfx_dir();

    DBG("Level path  : \"%s\"", LEVELS_PATH);
    DBG("Gfx path    : \"%s\"", GFX_PATH);
    DBG("Font path   : \"%s\"", TTF_PATH);
}

static void usage (void)
{
    static int whinged;

    if (whinged) {
        return;
    }
    whinged = true;

    CON("MundusMeus, options:");
    CON(" ");
    CON("        --seed        dungeon random seed");
    CON("        -seed");
    CON(" ");
    CON("Written by goblinhack@gmail.com");
}

static void parse_args (int32_t argc, char *argv[])
{
    int32_t i;

    LOG("Greetings");

    /*
     * Parse format args
     */
    CON("program: %s", argv[0]);
    for (i = 1; i < argc; i++) {
        CON("  arg: \"%s\"", argv[i]);
    }

    for (i = 1; i < argc; i++) {

        /*
         * -seed
         */
        if (!strcasecmp(argv[i], "--seed") ||
            !strcasecmp(argv[i], "-seed") ||
            !strcasecmp(argv[i], "-s")) {

            opt_seed = atoi(argv[i + 1]);
            i++;
            continue;
        }

        /*
         * Bad argument.
         */
        if (argv[i][0] == '-') {
            usage();
            WARN("unknown format argument, %s", argv[i]);
            continue;
        }

        usage();
        WARN("unknown format argument, %s", argv[i]);
    }
}

int32_t main (int32_t argc, char *argv[])
{
    mysrand(time(0));

    LOG_STDOUT = fopen("stdout.txt", "w+");
    LOG_STDERR = fopen("stderr.txt", "w+");

    ramdisk_init();

    ARGV = argv;

    dospath2unix(ARGV[0]);

    parse_args(argc, argv);

    term_init();

    math_init();

    color_init();

    find_file_locations();

    if (!config_init()) {
	ERR("Config init");
    }

    if (!config_load()) {
	ERR("Config read");
    }

    if (!sdl_init()) {
	ERR("SDL init");
    }

    if (!tex_init()) {
	ERR("tex init");
    }

    if (!music_init()) {
	ERR("music init");
    }

    if (!sound_init()) {
	ERR("sound init");
    }

    if (!tile_init()) {
	ERR("tile init");
    }

    if (!ttf_init()) {
	ERR("TTF init");
    }

    if (!font_init()) {
	ERR("Font init");
    }

    if (!wid_init()) {
	ERR("wid init");
    }

    if (!wid_hiscore_init()) {
	ERR("wid hiscore init");
    }

#ifdef ENABLE_LEAKCHECK
    ptrcheck_leak_snapshot();
#endif

    if (!resource_init()) {
	ERR("resource init");
    }

    if (!wid_console_init()) {
	ERR("wid_console init");
    }

    if (!command_init()) {
	ERR("command init");
    }

#ifdef ENABLE_LEAKCHECK
    if (!ptrcheck_init()) {
	ERR("ptrcheck init");
    }
#endif

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)level_init,
                          0, "level_init");

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)player_init,
                          0, "player_init");

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)tp_init,
                          0, "tp_init");

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)thing_init,
                          0, "thing_init");

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)wid_intro_about_init,
                          0, "wid_intro_about_init");

    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)wid_intro_settings_init,
                          0, "wid_intro_settings_init");

    action_init_fn_create(&init_fns,
                        (action_init_fn_callback)wid_intro_init,
                        0, "wid_intro_init");

    gl_enter_2d_mode();
    sdl_loop();
    gl_leave_2d_mode();

    quit();

    LOG("Bye, ok exit");
    fprintf(MY_STDOUT, "Exited\n");

    return (0);
}
