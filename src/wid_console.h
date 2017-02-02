/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

void wid_console_fini(void);
uint8_t wid_console_init(void);
void wid_console_log(const char *s);
void wid_console_hello(void);

/*
 * Global widgets.
 */
extern widp wid_console_input_line;

extern widp wid_console_window;
extern widp wid_console_vert_scroll;
#ifdef CONSOLE_HORIZ_SCROLL
extern widp wid_console_horiz_scroll;
#endif
extern widp wid_console_container;

uint8_t wid_console_receive_input(widp w, const SDL_KEYSYM *key);
