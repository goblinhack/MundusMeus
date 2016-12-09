/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */


#include "main.h"
#include "init_fn.h"
#include "init_fn_private.h"

tree_rootp init_fns;

void action_init_fns_destroy (tree_rootp *root)
{
    init_fnp p;

    if (!root) {
        ERR("no init_fns to destroy");
    }

    TREE_WALK((*root), p) {
        action_init_fn_destroy(root, p);
        if (!*root) {
            return;
        }
    }

    if (*root) {
        tree_destroy(root, 0);
    }
}

static void action_init_fn_free (init_fnp p)
{
    if (p->logname) {
        myfree(p->logname);
    }

    myfree(p);
}

void action_init_fn_destroy (tree_rootp *t, init_fnp p)
{
    if (!t) {
        return;
    }

    if (!tree_remove(*t, &p->tree.node)) {
        ERR("init destroy name failed");
    }

    action_init_fn_free(p);

    if (!tree_root_size(*t)) {
        tree_destroy(t, 0);
    }
}

init_fnp action_init_fn_create (tree_rootp *root,
                                action_init_fn_callback callback,
                                void *context,
                                const char *name)
{
    static uint32_t tiebreak;
    init *t;

    if (!*root) {
        *root = tree_alloc(TREE_KEY_INTEGER, "TREE ROOT: init_fns");
    }

    t = (TYPEOF(t)) myzalloc(sizeof(*t), "TREE NODE: init");

    t->context = context;
    t->callback = callback;
    t->tree.key = tiebreak++;

    if (!tree_insert(*root, &t->tree.node)) {
        ERR("collect init failed");
    }

    return (t);
}

uint8_t action_init_fns_tick (tree_rootp *root)
{
    init *t;

    if (!root) {
        return (false);
    }

    t = (TYPEOF(t)) tree_root_first(*root);
    if (!t) {
        return (false);
    }

    //
    // Too noisy
    //
//    INIT_FN_LOG(t, " ");

    if (!((t->callback)(t->context))) {
        ERR("init function failed");
    }

    tree_remove_found_node(*root, &t->tree.node);

    action_init_fn_free(t);

    if (!tree_root_size(*root)) {
        tree_destroy(root, 0);
    }

    return (true);
}
