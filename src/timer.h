/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */
typedef void (*action_timer_callback)(void *);
typedef void (*action_timer_destroy_callback)(void *);
#pragma once

void action_timers_destroy(tree_rootp *root);
void action_timer_destroy(tree_rootp *t, timerp p);
timerp action_timer_create(tree_rootp *root,
                    action_timer_callback,
                    action_timer_destroy_callback,
                    void *context,
                    const char *name,
                    uint32_t duration_m_s,
                    uint32_t jitter);
void action_timers_tick(tree_rootp *root);
