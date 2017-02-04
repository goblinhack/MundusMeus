/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "glapi.h"
#include "wid_notify.h"
#include "time_util.h"
#include "init_fn.h"
#include "ttf.h"
#include "slre.h"
#include "command.h"
#include "term.h"
#include "string_util.h"
#include "wid_game_map.h"
#include "player.h"
#include "python.h"

#if defined WIN32 || defined __CYGWIN__
#endif

#ifndef SDL_BUTTON_WHEELLEFT
#define SDL_BUTTON_WHEELLEFT 6
#endif

#ifndef SDL_BUTTON_WHEELRIGHT
#define SDL_BUTTON_WHEELRIGHT 7
#endif

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
#undef ENABLE_SDL_WINDOW
#else /* } { */
#define ENABLE_SDL_WINDOW
#endif /* } */

#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED /* { */
#define ENABLE_SDL_WINDOW
#endif /* } */

#ifdef DISABLE_SDL_WINDOW
#undef ENABLE_SDL_WINDOW
#endif

#ifdef ENABLE_SDL_WINDOW /* { */
#ifndef sdl_uikitkeyboard_h
#define sdl_uikitkeyboard_h
/*
 * For some reason, the header file is not installed.
 */
extern DECLSPEC int32_t SDLCALL SDL_iPhoneKeyboardShow(SDL_Window * window);
extern DECLSPEC int32_t SDLCALL SDL_iPhoneKeyboardHide(SDL_Window * window);
extern DECLSPEC SDL_bool SDLCALL SDL_iPhoneKeyboardIsShown(SDL_Window * window);
extern DECLSPEC int32_t SDLCALL SDL_iPhoneKeyboardToggle(SDL_Window * window);
#endif
#endif /* } */

static int sdl_get_mouse(void);
static void sdl_screenshot_(void);
static int sdl_do_screenshot;

uint8_t sdl_main_loop_running;
uint8_t sdl_shift_held;
int32_t sdl_init_video;
uint32_t mouse_down;
int32_t mouse_x;
int32_t mouse_y;

int32_t sdl_left_fire;
int32_t sdl_right_fire;

int32_t sdl_joy1_right;
int32_t sdl_joy1_left;
int32_t sdl_joy1_down;
int32_t sdl_joy1_up;

int32_t sdl_joy2_right;
int32_t sdl_joy2_left;
int32_t sdl_joy2_down;
int32_t sdl_joy2_up;

uint8_t sdl_joy_buttons[SDL_MAX_BUTTONS];

static SDL_Joystick *joy;
#if (SDL_MAJOR_VERSION == 2) /* { */
static SDL_Haptic *haptic;
#endif /* } */

int *sdl_joy_axes;
int sdl_joy_deadzone = 8000;
int joy_index;
int joy_naxes;
int joy_buttons;
int joy_balls;

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
#endif /* } */

#ifdef ENABLE_SDL_WINDOW /* { */
SDL_Window *window; /* Our window handle */
SDL_GLContext context; /* Our opengl context handle */
#endif /* } */

void sdl_fini (void)
{
    SDL_ShowCursor(0);
    SDL_ShowCursor(1);

    if (sdl_init_video) {
        sdl_init_video = 0;
        SDL_VideoQuit();
    }

#ifdef ENABLE_SDL_WINDOW /* { */
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
#endif /* } */

    SDL_Quit();
}

#if 0
static inline uint8_t sdl_find_video_size (int32_t w, int32_t h)
{
    static uint8_t first = true;
    int32_t i;

    first = true;

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
    SDL_Rect **modes;

    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes(0, SDL_FULLSCREEN|SDL_HWSURFACE);

    /* Check if there are any modes available */
    if (modes == (SDL_Rect**)0) {
        DIE("No modes available!");
    }

    /* Check if our resolution is restricted */
    if (modes == (SDL_Rect**)-1) {
        return (true);
    }

    for (i=0; modes[i]; ++i) {
        if (first) {
            DBG("SDL video   : %dx%d available (full screen)",
                    modes[i]->w, modes[i]->h);
        }

        if ((modes[i]->w == w) && (modes[i]->h == h)) {
            LOG("SDL video   : %dx%d available (full screen)", w, h);
            return (true);
        }
    }
#else /* } { */
    for (i = 0; i < SDL_GetNumDisplayModes(0); ++i) {

        SDL_DisplayMode mode;

        SDL_GetDisplayMode(0, i, &mode);

        if (first) {
            DBG("SDL video   : %dx%d available (full screen)", mode.w, mode.h);
        }

        if ((mode.w == w) && (mode.h == h)) {
            LOG("SDL video   : %dx%d available (full screen)", w, h);
            return (true);
        }
    }
#endif /* } */

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes(0, 0);

    /* Check if there are any modes available */
    if (modes == (SDL_Rect**)0) {
        DIE("No modes available!");
    }

    /* Check if our resolution is restricted */
    if (modes == (SDL_Rect**)-1) {
        return (true);
    }

    for (i=0; modes[i]; ++i) {
        if (first) {
            DBG("SDL video   : %dx%d available (any mode)",
                    modes[i]->w, modes[i]->h);
        }

        if ((modes[i]->w == w) && (modes[i]->h == h)) {
            LOG("SDL video   : %dx%d available (any mode)", w, h);
            return (true);
        }
    }
#else /* } { */
    for (i = 0; i < SDL_GetNumDisplayModes(0); ++i) {

        SDL_DisplayMode mode;

        SDL_GetDisplayMode(0, i, &mode);

        if (first) {
            DBG("SDL video   : %dx%d available (any mode)", mode.w, mode.h);
        }

        if ((mode.w == w) && (mode.h == h)) {
            LOG("SDL video   : %dx%d available (any mode)", w, h);
            return (true);
        }
    }
#endif /* } */

    DBG("SDL video   : %dx%d not available", w, h);

    first = false;

    return (false);
}
#endif

static inline void sdl_list_video_size (void)
{
    int32_t i;

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
    SDL_Rect **modes;

    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes(0, 0);

    /* Check if there are any modes available */
    if (modes == (SDL_Rect**)0) {
        return;
    }

    /* Check if our resolution is restricted */
    if (modes == (SDL_Rect**)-1) {
        return;
    }

    for (i=0; modes[i]; ++i) {
        LOG("SDL video   : %dx%d available",
            modes[i]->w, modes[i]->h);
    }
#else /* } { */
    for (i = 0; i < SDL_GetNumDisplayModes(0); ++i) {

        SDL_DisplayMode mode;

        SDL_GetDisplayMode(0, i, &mode);

        LOG("SDL video   : %dx%d available", mode.w, mode.h);
    }
#endif /* } */
}

void sdl_joy_rumble (float strength, uint32_t ms)
{
#if (SDL_MAJOR_VERSION == 2) /* { */
    if (!haptic) {
        return;
    }

    SDL_HapticRumblePlay(haptic, strength, ms);
#endif /* } */
}

static void sdl_init_rumble (void)
{
#if (SDL_MAJOR_VERSION == 2) /* { */
    if (!haptic) {
        haptic = SDL_HapticOpenFromJoystick(joy);
        if (!haptic) {
            LOG("Couldn't initialize SDL rumble: %s", SDL_GetError());
            SDL_ClearError();
            return;
        }
    }

    if (!SDL_HapticRumbleSupported(haptic)) {
        LOG("No SDL rumble support: %s", SDL_GetError());
        SDL_ClearError();
        return;
    }

    if (SDL_HapticRumbleInit(haptic) != 0) {
        LOG("SDL rumble nit failed: %s", SDL_GetError());
        SDL_ClearError();
        return;
    }
#endif /* } */

    LOG("Opened Haptic for joy index %d", joy_index);
}

static void sdl_init_joystick (void)
{
#if (SDL_MAJOR_VERSION == 2) /* { */
    SDL_GameController *controller = NULL;

    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

    SDL_InitSubSystem(SDL_INIT_HAPTIC);
#endif /* } */

    SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    joy_index = 0;
#if (SDL_MAJOR_VERSION == 2) /* { */
    for (joy_index = 0;
         joy_index < SDL_NumJoysticks(); ++joy_index) {

        if (SDL_IsGameController(joy_index)) {
            controller = SDL_GameControllerOpen(joy_index);
            if (controller) {
                LOG("Found gamecontroller");
                break;
            } else {
                WARN("Could not open gamecontroller %i: %s",
                    joy_index, SDL_GetError());
                SDL_ClearError();
            }
        }
    }

    if (!controller) {
        LOG("No found gamecontroller");
        return;
    }
#endif /* } */

    joy = SDL_JoystickOpen(joy_index);
    if (joy) {
        LOG("Opened Joystick %d", joy_index);
#if (SDL_MAJOR_VERSION == 2) /* { */
        LOG("Name: %s", SDL_JoystickNameForIndex(0));
#endif /* } */
        LOG("Number of Axes: %d", SDL_JoystickNumAxes(joy));
        LOG("Number of Buttons: %d", SDL_JoystickNumButtons(joy));
        LOG("Number of Balls: %d", SDL_JoystickNumBalls(joy));

        joy_naxes = SDL_JoystickNumAxes(joy);
        joy_buttons = SDL_JoystickNumButtons(joy);
        joy_balls = SDL_JoystickNumBalls(joy);
    } else {
        WARN("Couldn't open Joystick 0");
    }
}

#if 0
static int
get_screensize (int *w, int *h)
{
    SDL_SysWMinfo info;

    SDL_VERSION(&info.version);

#if SDL_MAJOR_VERSION == 2 /* { */
    if (SDL_GetWMInfo(&info) > 0) {
#else
#endif
#if defined WIN32 || defined __CYGWIN__
        HWND top;
        RECT top_rect;

        HWND current;

        current = info.window;
        top = GetDesktopWindow();

        GetWindowRect(top, &top_rect);

        *w = top_rect.right - top_rect.left;
        *h = top_rect.bottom - top_rect.top;

        LOG("Win32 window size " + tostring(*w) + "x" + tostring(*h));

        return (true);
#else
        if (info.subsystem == SDL_SYSWM_X11) {
            info.info.x11.lock_func();
            *w = DisplayWidth(info.info.x11.display,
                DefaultScreen(info.info.x11.display));
            *h = DisplayHeight(info.info.x11.display,
                DefaultScreen(info.info.x11.display));
            info.info.x11.unlock_func();

            if (gh_global::trace_mode) {
                *w = 800;
                *h = 600;
            }

            LOG("Unix window size " + tostring(*w) + "x" + tostring(*h));
            return (true);
        } else {
            *w = 1024;
            *h = 800;
            LOG("Default screensize for unix");
            return (true);
        }
#endif // unix
    } else {
        //
        // MacOS hits here - no idea why. Need help from nice MacOS folks.
        //
        *w = 1024;
        *h = 800;
        LOG("Default screensize as SDL_GetWMInfo failed");
        return (false);
    }
}
#endif

uint8_t sdl_init (void)
{
    gl_ext_init();

    int32_t VIDEO_WIDTH;
    int32_t VIDEO_HEIGHT;
    int32_t value;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        SDL_MSG_BOX("SDL_Init failed %s", SDL_GetError());
        DIE("SDL_Init failed %s", SDL_GetError());
    }

#if SDL_MAJOR_VERSION == 2 /* { */
    if (SDL_VideoInit(0) != 0) {
        SDL_MSG_BOX("SDL_VideoInit failed %s", SDL_GetError());
        DIE("SDL_VideoInit failed %s", SDL_GetError());
    }
#endif /* } */

    sdl_init_joystick();
    sdl_init_rumble();

    sdl_init_video = 1;

    LOG("SDL version : %u.%u", SDL_MAJOR_VERSION, SDL_MINOR_VERSION);

    sdl_list_video_size();

#if 0
    uint8_t medium_size = sdl_find_video_size(
                            MEDIUM_VIDEO_WIDTH,
                            MEDIUM_VIDEO_HEIGHT);

    uint8_t default_size = sdl_find_video_size(
                            DEFAULT_VIDEO_WIDTH,
                            DEFAULT_VIDEO_HEIGHT);

#endif
    /*
     * If we have a saved setting, use that.
     */
    if (game.video_pix_width && game.video_pix_height) {
        VIDEO_WIDTH = game.video_pix_width;
        VIDEO_HEIGHT = game.video_pix_height;
    /*
     * Else guess.
     */
    } else {
#if SDL_MAJOR_VERSION == 2 /* { */
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);
        game.video_pix_width = mode.w;
        game.video_pix_height = mode.h;
#else
        const SDL_VideoInfo *info = SDL_GetVideoInfo();
        game.video_pix_width = info->current_w;
        game.video_pix_height = info->current_h;
#endif
        VIDEO_WIDTH = game.video_pix_width;
        VIDEO_HEIGHT = game.video_pix_height;
#if 0
    } else if (get_screensize(&game.video_pix_width,
                              &game.video_pix_height)) {
        VIDEO_WIDTH = game.video_pix_width;
        VIDEO_HEIGHT = game.video_pix_height;
    } else if (medium_size) {
        VIDEO_WIDTH = MEDIUM_VIDEO_WIDTH;
        VIDEO_HEIGHT = MEDIUM_VIDEO_HEIGHT;
    } else {
        VIDEO_WIDTH = DEFAULT_VIDEO_WIDTH;
        VIDEO_HEIGHT = DEFAULT_VIDEO_HEIGHT;
#endif
    }

    /*
     * Ortho fixup.
     */
    gl_ortho_set(VIDEO_WIDTH, VIDEO_HEIGHT);

    LOG("SDL video   : %dx%d",
        game.video_pix_width, game.video_pix_height);

    LOG("Ortho video : %dx%d",
        game.video_gl_width, game.video_gl_height);

    LOG("X/Y scale   : %f, %f",
        game.xscale, game.yscale);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /*
     * Don't use this. It seemed to mess up graphics on FireGL.
     *
     */
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    uint32_t video_flags;

#ifndef ENABLE_SDL_WINDOW /* { */
        LOG("SDL mode    : video");

        video_flags = SDL_OPENGL | SDL_NOFRAME;

        if (game.full_screen) {
            video_flags |= SDL_FULLSCREEN;
        }

        if (SDL_SetVideoMode(VIDEO_WIDTH, VIDEO_HEIGHT,
                            0 /* bpp */, video_flags) == 0) {

            game.video_pix_width = 0;
            game.video_pix_height = 0;

            config_save();

            ERR("Couldn't set display %ux%u: %s",
                VIDEO_WIDTH, VIDEO_HEIGHT,
                SDL_GetError());
            SDL_ClearError();
        }

#   else /* } { */
        LOG("SDL mode    : window");

        video_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS ;

        if (game.full_screen) {
            video_flags |= SDL_WINDOW_FULLSCREEN;
        }

        window = SDL_CreateWindow("mundusmeus",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                VIDEO_WIDTH, VIDEO_HEIGHT,
                                video_flags);
        if (!window) {
            game.video_pix_width = 0;
            game.video_pix_height = 0;

            config_save();

            SDL_MSG_BOX("Couldn't set windowed display %ux%u: %s",
                        VIDEO_WIDTH, VIDEO_HEIGHT,
                        SDL_GetError());

            DIE("Couldn't set windowed display %ux%u: %s",
                VIDEO_WIDTH, VIDEO_HEIGHT,
                SDL_GetError());
        }

        context = SDL_GL_CreateContext(window);
        if (!context) {
            SDL_MSG_BOX("SDL_GL_CreateContext failed %s", SDL_GetError());
            SDL_ClearError();
        }

        if (SDL_GL_MakeCurrent(window, context) < 0) {
            SDL_MSG_BOX("SDL_GL_MakeCurrent failed %s", SDL_GetError());
            SDL_ClearError();
        }

#endif /* } */

    SDL_ClearError();

#if SDL_MAJOR_VERSION == 1 /* { */
    SDL_WM_SetCaption("mundusmeus", "mundusmeus");
#else
    SDL_SetWindowTitle(window, "mundusmeus");
#endif /* } */

    LOG("GL Vendor   : %s", glGetString(GL_VENDOR));
    LOG("GL Renderer : %s", glGetString(GL_RENDERER));
    LOG("GL Version  : %s", glGetString(GL_VERSION));
    LOG("GL Exts     : %s", glGetString(GL_EXTENSIONS));

    SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
    DBG("Red         : %d", value);

    SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
    DBG("Green       : %d", value);

    SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
    DBG("Blue        : %d", value);

    SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
    DBG("Depth       : %d", value);

    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
    DBG("Doub Buffer : %d", value);

    SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
    DBG("Hw Accel    : %d", value);

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
    DBG("Vsync       : %d", SDL_GL_GetSwapInterval());
#endif /* } */

#if SDL_MAJOR_VERSION == 1 /* { */
    SDL_EnableKeyRepeat(150, 50);
#endif /* } */

    return (true);
}

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
static int32_t sdl_filter_events (void *userdata, SDL_Event *event)
{
    switch (event->type) {
        /* This is important!  Queue it if we want to quit. */
        case SDL_QUIT:
            return (1);

        /* Mouse and keyboard events go to threads */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEWHEEL:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
        case SDL_CONTROLLERAXISMOTION:
        case SDL_JOYAXISMOTION:               /* Joystick axis motion */
        case SDL_JOYBALLMOTION:               /* Joystick trackball motion */
        case SDL_JOYHATMOTION:                /* Joystick hat position change */
        case SDL_JOYBUTTONDOWN:               /* Joystick button pressed */
        case SDL_JOYBUTTONUP:                 /* Joystick button released */
            return (1);

        /* Drop all other events */
        default:
            return (0);
    }
}
#endif /* } */

static void print_bar (char *tmp, int pos, int len)
{
    int i;

    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "[");

    for (i = 0; i < len; ++i) {
        if (i == pos) {
            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "#");
        } else {
            snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), " ");
        }
    }
    snprintf(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp), "]");
}

static void sdl_event (SDL_Event * event)
{
    SDL_KEYSYM *key;

    switch (event->type) {
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2 /* { */
    case SDL_WINDOWEVENT:
        switch (event->window.event) {
        case SDL_WINDOWEVENT_SHOWN:
            DBG("Window %d shown", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_HIDDEN:
            DBG("Window %d hidden", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            DBG("Window %d exposed", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MOVED:
            DBG("Window %d moved to %d,%d",
                event->window.windowID, event->window.data1,
                event->window.data2);
            break;
        case SDL_WINDOWEVENT_RESIZED:
            DBG("Window %d resized to %dx%d",
                event->window.windowID, event->window.data1,
                event->window.data2);
            break;
        case SDL_WINDOWEVENT_MINIMIZED:
            DBG("Window %d minimized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_MAXIMIZED:
            DBG("Window %d maximized", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_RESTORED:
            DBG("Window %d restored", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_ENTER:
            DBG("Mouse entered window %d", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            DBG("Mouse left window %d", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            DBG("Window %d gained keyboard focus", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            DBG("Window %d lost keyboard focus", event->window.windowID);
            break;
        case SDL_WINDOWEVENT_CLOSE:
            DBG("Window %d closed", event->window.windowID);
            break;
        default:
            DBG("Window %d got unknown event %d",
                event->window.windowID, event->window.event);
            break;
        }
        break;
#endif /* } */

    case SDL_KEYDOWN:
        DBG("Keyboard: key pressed keycode 0x%08X = %s",
            event->key.keysym.sym,
            SDL_GetKeyName(event->key.keysym.sym));

            key = &event->key.keysym;

            wid_key_down(key, mouse_x, mouse_y);

            sdl_shift_held = (key->mod & KMOD_SHIFT) ? 1 : 0;
        break;

    case SDL_KEYUP:
        DBG("Keyboard: key released keycode 0x%08X = %s",
            event->key.keysym.sym,
            SDL_GetKeyName(event->key.keysym.sym));

            key = &event->key.keysym;

            wid_key_up(key, mouse_x, mouse_y);

            sdl_shift_held = (key->mod & KMOD_SHIFT) ? 1 : 0;
        break;

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
    case SDL_TEXTINPUT:
        DBG("Keyboard: text input \"%s\" in window %d",
            event->text.text, event->text.windowID);
        break;
#endif /* } */

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
    case SDL_MOUSEWHEEL: {
        DBG("Mouse: wheel scrolled %d in x and %d in y in window %d",
            event->wheel.x, event->wheel.y, event->wheel.windowID);

        sdl_get_mouse();

        static double accel = 1.0;

        {
            static uint32_t ts;

            if (time_have_x_tenths_passed_since(5, ts)) {
                accel = 1.0;
            } else {
                accel *= ENABLE_WHEEL_SCROLL_SPEED_SCALE;

                if (accel > ENABLE_WHEEL_MAX_SCROLL_SPEED_SCALE) {
                    accel = ENABLE_WHEEL_MAX_SCROLL_SPEED_SCALE;
                }
            }

            ts = time_get_time_ms_cached();
        }

        double wheel_x = event->wheel.x;
        double wheel_y = event->wheel.y;

        wheel_x *= accel;
        wheel_y *= accel;

        /*
         * Negative wheel x so side scrolls seem natural. Could just be
         * a dumb macos thing to ifdef?
         */
        wid_mouse_visible = 1;
        wid_mouse_motion(mouse_x, mouse_y, 0, 0, -wheel_x, wheel_y);
        break;
    }
#endif /* } */

    case SDL_MOUSEMOTION:
        mouse_down = sdl_get_mouse();

        DBG("Mouse: moved to %d,%d (%d,%d) state %d",
            event->motion.x, event->motion.y,
            event->motion.xrel, event->motion.yrel, mouse_down);

        wid_mouse_visible = 1;
        wid_mouse_motion(mouse_x, mouse_y,
                         event->motion.xrel, event->motion.yrel,
                         0, 0);
        break;

    case SDL_MOUSEBUTTONDOWN:
        mouse_down = sdl_get_mouse();

        DBG("Mouse DOWN: button %d pressed at %d,%d state %x",
            event->button.button, event->button.x, event->button.y,
            mouse_down);

        wid_mouse_visible = 1;
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */
        if (event->button.button == SDL_BUTTON_WHEELUP) {
            DBG("  wheel up");
            wid_mouse_motion(mouse_x, mouse_y, 0, 0, 0, 10);
            break;
        } else if (event->button.button == SDL_BUTTON_WHEELDOWN) {
            DBG("  wheel down");
            wid_mouse_motion(mouse_x, mouse_y, 0, 0, 0, -10);
            break;
        } else if (event->button.button == SDL_BUTTON_WHEELLEFT) {
            DBG("  wheel left");
            wid_mouse_motion(mouse_x, mouse_y, 0, 0, -10, 0);
            break;
        } else if (event->button.button == SDL_BUTTON_WHEELRIGHT) {
            DBG("  wheel right");
            wid_mouse_motion(mouse_x, mouse_y, 0, 0, 10, 0);
            break;
        }
#endif /* } */

        wid_mouse_down(event->button.button, mouse_x, mouse_y);
        break;

    case SDL_MOUSEBUTTONUP:
        mouse_down = sdl_get_mouse();

        DBG("Mouse UP: button %d released at %d,%d state %d",
            event->button.button, event->button.x, event->button.y,
            mouse_down);

        wid_mouse_up(event->button.button, mouse_x, mouse_y);
        break;

    case SDL_JOYAXISMOTION: {
        DBG("Joystick %d: axis %d moved by %d",
            event->jaxis.which, event->jaxis.axis, event->jaxis.value);

        int axis = event->jaxis.axis;
        int value = event->jaxis.value;

        if (!sdl_joy_axes) {
            sdl_joy_axes = (int*) myzalloc(sizeof(int) * joy_naxes, "joy axes");
        }

        sdl_joy_axes[axis] = value;

        sdl_left_fire = false;
        sdl_right_fire = false;

        if (sdl_joy_axes[2] > sdl_joy_deadzone) {
            DBG("left fire");
            sdl_left_fire = true;
            sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE] = 1;
        } else {
            sdl_joy_buttons[SDL_JOY_BUTTON_LEFT_FIRE] = 0;
        }


        if (sdl_joy_axes[5] > sdl_joy_deadzone) {
            DBG("right fire");
            sdl_right_fire = true;
            sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE] = 1;
        } else {
            sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT_FIRE] = 0;
        }

        if (sdl_right_fire || sdl_left_fire) {
            sdl_get_mouse();
            wid_joy_button(mouse_x, mouse_y);
        }

        break;
    }

    case SDL_JOYBALLMOTION:
        DBG("Joystick %d: ball %d moved by %d,%d",
            event->jball.which, event->jball.ball, event->jball.xrel,
            event->jball.yrel);
        break;

    case SDL_JOYHATMOTION:
        DBG("Joystick %d: hat %d moved to ", event->jhat.which,
            event->jhat.hat);

        switch (event->jhat.value) {
        case SDL_HAT_CENTERED:
            break;
        case SDL_HAT_UP:
            DBG("UP");
            sdl_joy2_up = true;
            break;
        case SDL_HAT_RIGHTUP:
            DBG("RIGHTUP");
            sdl_joy2_right = true;
            sdl_joy2_up = true;
            break;
        case SDL_HAT_RIGHT:
            DBG("RIGHT");
            sdl_joy2_right = true;
            break;
        case SDL_HAT_RIGHTDOWN:
            DBG("RIGHTDOWN");
            sdl_joy2_right = true;
            sdl_joy2_down = true;
            break;
        case SDL_HAT_DOWN:
            DBG("DOWN");
            sdl_joy2_down = true;
            break;
        case SDL_HAT_LEFTDOWN:
            DBG("LEFTDOWN");
            sdl_joy2_left = true;
            sdl_joy2_down = true;
            break;
        case SDL_HAT_LEFT:
            DBG("LEFT");
            sdl_joy2_left = true;
            break;
        case SDL_HAT_LEFTUP:
            sdl_joy2_left = true;
            sdl_joy2_up = true;
            DBG("LEFTUP");
            break;
        default:
            DBG("UNKNOWN");
            break;
        }
        break;

    case SDL_JOYBUTTONDOWN:
        DBG("Joystick %d: button %d pressed",
            event->jbutton.which, event->jbutton.button);
        sdl_joy_buttons[event->jbutton.button] = 1;
        sdl_get_mouse();
        wid_joy_button(mouse_x, mouse_y);
        break;

    case SDL_JOYBUTTONUP:
        DBG("Joystick %d: button %d released",
            event->jbutton.which, event->jbutton.button);
        sdl_joy_buttons[event->jbutton.button] = 0;
        break;

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
    case SDL_CLIPBOARDUPDATE:
        DBG("Clipboard updated");
        break;
#endif /* } */

    case SDL_QUIT:
        SDL_ShowCursor(1);

        DIE1("Quit requested");
        break;

    case SDL_USEREVENT:
        DBG("User event %d", event->user.code);
        break;

    default:
        DBG("Unknown event %d", event->type);
        break;
    }
}

static int sdl_get_mouse (void)
{
    if (!wid_mouse_visible) {
        return (0);
    }

    int x, y;
    int button = SDL_GetMouseState(&x, &y);

    if (!x && !y) {
        return (button);
    }

    mouse_x = x;
    mouse_y = y;

    mouse_x *= game.xscale;
    mouse_y *= game.yscale;

    return (button);
}

void sdl_mouse_center (void)
{
    int x, y;

    x = game.video_pix_width / 2;
    y = game.video_pix_height / 2;

    sdl_mouse_warp(x, y);
}

void sdl_mouse_warp (int32_t x, int32_t y)
{
    int32_t border = 10;

    if (x <= 0) {
        x = border;
    } else if (x >= game.video_pix_width - border) {
        x = game.video_pix_width - border;
    }
    if (y <= 0) {
        y = border;
    } else if (y >= game.video_pix_height - border) {
        y = game.video_pix_height - border;
    }

#if (SDL_MAJOR_VERSION == 2)
    SDL_WarpMouseInWindow(window, x, y);
#else
    SDL_WarpMouse(x, y);
#endif

    mouse_x = x;
    mouse_y = y;
}

static void sdl_tick (void)
{
    sdl_left_fire = false;
    sdl_left_fire = true;

    sdl_joy1_right = false;
    sdl_joy1_left = false;
    sdl_joy1_down = false;
    sdl_joy1_up = false;

    sdl_joy2_right = false;
    sdl_joy2_left = false;
    sdl_joy2_down = false;
    sdl_joy2_up = false;

    if (!sdl_joy_axes) {
        return;
    }

    sdl_get_mouse();

    /*
     * Right stick
     */
    if (sdl_joy_axes[3] > sdl_joy_deadzone) {
        DBG("right stick, right");
        sdl_joy1_right = true;

        sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT]++;
        wid_joy_button(mouse_x, mouse_y);
        sdl_joy_buttons[SDL_JOY_BUTTON_RIGHT]--;
    }

    if (sdl_joy_axes[3] < -sdl_joy_deadzone) {
        DBG("right stick, left");
        sdl_joy1_left = true;

        sdl_joy_buttons[SDL_JOY_BUTTON_LEFT]++;
        wid_joy_button(mouse_x, mouse_y);
        sdl_joy_buttons[SDL_JOY_BUTTON_LEFT]--;
    }

    if (sdl_joy_axes[4] > sdl_joy_deadzone) {
        DBG("right stick, down");
        sdl_joy1_down = true;

        sdl_joy_buttons[SDL_JOY_BUTTON_DOWN]++;
        wid_joy_button(mouse_x, mouse_y);
        sdl_joy_buttons[SDL_JOY_BUTTON_DOWN]--;
    }

    if (sdl_joy_axes[4] < -sdl_joy_deadzone) {
        DBG("right stick, up");
        sdl_joy1_up = true;

        sdl_joy_buttons[SDL_JOY_BUTTON_UP]++;
        wid_joy_button(mouse_x, mouse_y);
        sdl_joy_buttons[SDL_JOY_BUTTON_UP]--;
    }

    /*
     * Left stick
     */
    int mx = 0;
    int my = 0;

    if (sdl_joy_axes[0] > sdl_joy_deadzone) {
        DBG("left stick, right");
        sdl_joy2_right = true;
        mx = 1;
    }

    if (sdl_joy_axes[0] < -sdl_joy_deadzone) {
        DBG("left stick, left");
        sdl_joy2_left = true;
        mx = -1;
    }

    if (sdl_joy_axes[1] > sdl_joy_deadzone) {
        DBG("left stick, down");
        sdl_joy2_down = true;
        my = 1;
    }

    if (sdl_joy_axes[1] < -sdl_joy_deadzone) {
        DBG("left stick, up");
        sdl_joy2_up = true;
        my = -1;
    }

    static double accel = 1.0;
    static uint32_t ts;

    if (time_have_x_tenths_passed_since(5, ts)) {
        accel = 1.0;
    }

    if ((mx != 0) || (my != 0)) {
        ts = time_get_time_ms_cached();

        accel *= ENABLE_JOY_SCROLL_SPEED_SCALE;

        if (accel > ENABLE_JOY_MAX_SCROLL_SPEED_SCALE) {
            accel = ENABLE_JOY_MAX_SCROLL_SPEED_SCALE;
        }

        double x = mouse_x + ((double)mx * accel);
        double y = mouse_y + ((double)my * accel);

        if (x < 0) {
            x = 0;
        }

        if (y < 0) {
            y = 0;
        }

        if (x > game.video_pix_width - 1) {
            x = game.video_pix_width - 1;
        }

        if (y > game.video_pix_height - 1) {
            y = game.video_pix_height - 1;
        }

        if (wid_mouse_visible) {
            sdl_mouse_warp(x, y);
        }
    }

    if (debug_enabled) {
        char tmp[MAXSTR];
        *tmp = 0;

        int i;
        for (i = 0; i < joy_naxes; ++i) {
            int len = 80 - 20;

            *tmp = 0;
            snprintf(tmp + strlen(tmp),
                        sizeof(tmp) - strlen(tmp),
                        "  %2d: %6d  ", i, sdl_joy_axes[i]);

            print_bar(tmp, (sdl_joy_axes[i] + 32767) * (len-1) / 65534, len);
            DBG("%s", tmp);
        }
    }
}

uint8_t sdl_is_dungeon_way_downing (void)
{
    return (!sdl_main_loop_running);
}

void sdl_exit (void)
{
    if (!sdl_main_loop_running) {
        return;
    }

    LOG("Main loop is exiting...");

    SDL_ShowCursor(1);

    sdl_main_loop_running = false;
}

/*
 * User has entered a command, run it
 */
uint8_t fps_enable (tokens_t *tokens, void *context)
{
    char *s = tokens->args[2];

    if (!s || (*s == '\0')) {
        game.fps_counter = true;
    } else {
        game.fps_counter = strtol(s, 0, 10) ? 1 : 0;
    }

    DBG("FPS mode set to %u", game.fps_counter);

    return (true);
}

/*
 * User has entered a command, run it
 */
uint8_t sdl_user_exit (tokens_t *tokens, void *context)
{
    sdl_exit();

    return (true);
}

/*
 * Main loop
 */
void sdl_loop (void)
{
    SDL_Event events[10];
    int32_t found;
    int32_t i;
    uint16_t frames = 0;

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
    SDL_SetEventFilter(sdl_filter_events, 0);
#endif /* } */

    glEnable(GL_TEXTURE_2D);

    /*
     * Wait for events
     */
    int32_t timestamp_then = time_get_time_ms();
    int32_t timestamp_then2 = timestamp_then;

    sdl_main_loop_running = true;

    glClearColor(BLACK.r, BLACK.g, BLACK.b, 1.0f);

    /*
     * Turn on syncing for the intro screen.
     */
#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */

    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

#else /* } { */

    SDL_GL_SetSwapInterval(1);

#endif /* } */

    uint8_t init_done = false;

    while (!init_done) {

        /*
         * Clear the screen
         */
#if 0
        glClear(GL_COLOR_BUFFER_BIT);
#endif

        time_update_time_milli();

        /*
         * Splash screen loading bar for init functions.
         */
        init_done = !action_init_fns_tick(&init_fns);

#if 0
        sdl_splashscreen_update();

        /*
         * Let widgets move.
         */
        wid_tick_all();

        /*
         * Let widgets move.
         */
        wid_move_all();

        /*
         * Display windows.
         */
        wid_display_all();

        /*
         * Flip
         */
#ifdef ENABLE_SDL_WINDOW /* { */
        SDL_GL_SwapWindow(window);
#else /* } { */
        SDL_GL_SwapBuffers();
#endif /* } */
#endif
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    /*
     * Don't use this. It seemed to mess up graphics on FireGL.
     *
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
     */

#if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION == 2 /* { */

        if (game.display_sync) {
            SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
        } else {
            SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 0);
        }

#else /* } { */

        if (game.display_sync) {
            SDL_GL_SetSwapInterval(1);
        } else {
            SDL_GL_SetSwapInterval(0);
        }

#endif /* } */

    for (;;) {
        /*
         * Clear the screen
         */
        glClear(GL_COLOR_BUFFER_BIT);

        frames++;

        /*
         * Reset joystick handling before we poll and update.
         */
        sdl_tick();

        /*
         * Do processing of some things, like reading the keyboard or doing
         * stuff with widgets only occasionally if we do not need to.
         */
        int32_t timestamp_now = time_update_time_milli();

        if ((timestamp_now - timestamp_then > 10)) {
            if (sdl_do_screenshot) {
                sdl_do_screenshot = 0;
                sdl_screenshot_();
            }

            /*
             * Give up some CPU to allow events to arrive and time for the GPU
             * to process the above.
             */
            timestamp_then = timestamp_now;

            /*
             * Clean up dead widgets.
             */
            wid_gc_all();

            /*
             * Read events
             */
            SDL_PumpEvents();

#if (SDL_MAJOR_VERSION == 2) || \
        (SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION > 2) /* { */
            found = SDL_PeepEvents(events,
                                    ARRAY_SIZE(events),
                                    SDL_GETEVENT,
                                    SDL_QUIT,
                                    SDL_LASTEVENT);
#else /* } { */
            found = SDL_PeepEvents(events,
                                    ARRAY_SIZE(events),
                                    SDL_GETEVENT,
                                    SDL_QUITMASK|
                                    SDL_MOUSEEVENTMASK|
                                    /*
                                    * Seems not to be in SDL 1.2.14
                                    SDL_MOUSEWHEELMASK|
                                    */
                                    SDL_MOUSEMOTIONMASK|
                                    SDL_MOUSEBUTTONDOWNMASK|
                                    SDL_MOUSEBUTTONUPMASK|
                                    SDL_KEYDOWNMASK|
                                    SDL_KEYUPMASK);
#endif /* } */

            for (i = 0; i < found; ++i) {
                sdl_event(&events[i]);
            }

            /*
             * SDL doesn't seem to like an immediate center. beachball hang if
             * we do this.
             */
            static int first = 0;
            if (first) {
                first = 0;
                sdl_mouse_center();
                SDL_ShowCursor(0);
                wid_mouse_motion(mouse_x, mouse_y, 0, 0, 0, 0);
            }

            if (enable_console) {
                console_tick();
            }

            if (!sdl_main_loop_running) {
                break;
            }
        }

        if (game.need_tick) {
            game.need_tick = false;
            py_call_void_module_void("hooks", "hook_game_tick");
        }

        levelp level = &game.level;

        level = &game.level;
        if (level) {
            /*
             * Move the window so it scrolls as the player widget moves.
             */
            wid_game_map_scroll_adjust(level, 1);
        }

        /*
         * Display windows.
         */
        wid_display_all();

        /*
         * FPS counter.
         */
        {
            static char fps_text[10] = {0};

            /*
             * Very occasional.
             */
            if (timestamp_now - timestamp_then2 >= 1000) {

                timestamp_then2 = timestamp_now;

                /*
                 * User notifications.
                 */
                wid_notify_tick();

                /*
                 * FPS
                 */
                if (game.fps_counter) {
                    /*
                     * Update FPS counter.
                     */
                    snprintf(fps_text, sizeof(fps_text), "%u FPS", frames);

                    frames = 0;
                }
            }

            /*
             * FPS
             */
            if (game.fps_counter) {
                glcolor(YELLOW);

                ttf_puts(small_font, fps_text,
                         0.90 * (double) game.video_pix_width,
                         0.97 * (double) game.video_pix_height,
                         1.0, 1.0, true);
            }
        }

        if (wid_tooltip_string) {
            color c = PURPLE4;
            c.a = 100;
            glcolor(c);
            glBindTexture(GL_TEXTURE_2D, 0);

            gl_blitquad(
                     0.00 * (double) game.video_pix_width,
                     0.97 * (double) game.video_pix_height,
                     1.00 * (double) game.video_pix_width,
                     1.00 * (double) game.video_pix_height);

            glcolor(WHITE);
            ttf_puts(small_font, wid_tooltip_string,
                     0.00 * (double) game.video_pix_width,
                     0.97 * (double) game.video_pix_height,
                     1.0, 1.0, false);
        }

        blit_flush();

        SDL_Delay(game.sdl_delay);

        time_get_time_ms();

        /*
         * Flip
         */
        {
#ifdef ENABLE_SDL_WINDOW /* { */
            SDL_GL_SwapWindow(window);
#else /* } { */
            SDL_GL_SwapBuffers();
#endif /* } */
        }
    }

    SDL_ShowCursor(1);
}

void sdl_show_keyboard (void)
{
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED /* { */
    SDL_iPhoneKeyboardShow(window);
#endif /* } */
}

void sdl_hide_keyboard (void)
{
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED /* { */
    SDL_iPhoneKeyboardHide(window);
#endif /* } */
}

void sdl_screenshot (void)
{
    sdl_do_screenshot = 1;
}

static void sdl_screenshot_ (void)
{
    FILE *fp;
    int w = game.video_pix_width;
    int h = game.video_pix_height;

    static int count;
    char *filename = dynprintf("screenshot.%d.ppm", ++count);
    unsigned char * pixels = (unsigned char *)
                    mymalloc(w*h*4, "screenshot"); // 4 bytes for RGBA

    fp = fopen(filename, "w");

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK_LEFT);

    glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    fprintf(fp, "P6\n%d %d\n255\n",w,h);

    int i, j;

    for (j=h-1;j>=0;j--) {
        for (i=0;i<w;i++) {
            fputc(pixels[3*j*w+3*i+0], fp);
            fputc(pixels[3*j*w+3*i+1], fp);
            fputc(pixels[3*j*w+3*i+2], fp);
        }
    }

    CON("screenshot: %s", filename);

    fclose(fp);
    myfree(filename);
    myfree(pixels);
}
