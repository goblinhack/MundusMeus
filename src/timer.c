/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "main.h"
#include "timer.h"
#include "timer_private.h"
#include "time_util.h"

/*
 * Various global timers.
 */
tree_rootp level_timers;

void action_timers_destroy (tree_rootp *root)
{
    if (!*root) {
        return;
    }

    verify(*root);

    timerp p;

    TREE_WALK((*root), p) {
        action_timer_destroy(root, p);
        if (!*root) {
            return;
        }
    }

    if (*root) {
        tree_destroy(root, 0);
    }
}

static void action_timer_free (timerp p)
{
    verify(p);

    LOG("%p: Final free of action timer", p);

    if (p->destroy_callback) {
        (p->destroy_callback)(p->context);
    }

    myfree(p);
}

void action_timer_destroy (tree_rootp *t, timerp p)
{
    verify(p);

    LOG("%p: Destroy action timer destroyed", p);

    if (!t) {
        return;
    }

    verify(*t);

    tree_rootp tree = *t;

    if (!tree) {
        ERR("no timer tree");
    }

    if (!tree_remove(tree, &p->tree.node)) {
        ERR("timer remove failed");
    }

    action_timer_free(p);

    if (!tree_root_size(tree)) {
        tree_destroy(t, 0);
    }
}

timerp action_timer_create (tree_rootp *root,
                            action_timer_callback callback,
                            action_timer_destroy_callback destroy_callback,
                            void *context,
                            const char *name,
                            uint32_t duration_m_s,
                            uint32_t jitter)
{
    static uint32_t tiebreak;
    timer *t;

    if (!*root) {
        *root = tree_alloc(TREE_KEY_TWO_INTEGER, "TREE ROOT: timers");
    }

    verify(*root);

    t = (TYPEOF(t)) myzalloc(sizeof(*t), "TREE NODE: timer");

    t->expires_when = time_get_time_ms() + duration_m_s;
    if (jitter) {
        t->expires_when += myrand() % jitter;
    }

    t->context = context;
    t->callback = callback;
    t->destroy_callback = destroy_callback;
    t->tree.key2 = t->expires_when;
    t->tree.key3 = tiebreak++;

    if (!tree_insert(*root, &t->tree.node)) {
        ERR("Action collect timer failed");
    }

    LOG("%p: Action timer (%s) created, fires in %3.2f secs",
        t,
        name,
        ((float)(t->expires_when - time_get_time_ms())) /
        (float)ONESEC);

    return (t);
}

void action_timers_tick (tree_rootp *root)
{
    timer *t;

    if (!*root) {
        return;
    }

    for (;;) {
        verify(*root);

        t = (TYPEOF(t)) tree_root_first(*root);
        if (!t) {
            return;
        }

        verify(t);

        if (t->expires_when > time_get_time_ms()) {
            return;
        }

        (t->callback)(t->context);

        if (!*root) {
            /*
             * Destroyed in callback?
             */
            break;
        }

        verify(t);
        verify(*root);

        tree_remove_found_node(*root, &t->tree.node);

        action_timer_free(t);
    }
}
