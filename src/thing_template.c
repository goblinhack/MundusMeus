/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file for license.
 */


#include "main.h"
#include "thing_tile.h"
#include "tile.h"

/*
 * Using static memory as these things never change once made.
 */
thing_template thing_templates_chunk[TP_MAX];

tree_root *thing_templates;
tree_root *thing_templates_create_order;

static uint8_t tp_init_done;
static void tp_destroy_internal(tpp t);

uint8_t tp_init (void)
{
    tp_init_done = true;

    return (true);
}

void tp_fini (void)
{
    if (tp_init_done) {
        tp_init_done = false;

        tree_destroy(&thing_templates,
            (tree_destroy_func)tp_destroy_internal);

        tree_destroy(&thing_templates_create_order,
            (tree_destroy_func)0);
    }
}

/*
 * Fill up the tile cache for future fast finds of tile types.
 */
static void tp_fill_cache (tpp t)
{
    thing_tilep thing_tile;
    int index;
    tilep tile;

    TREE_WALK_UNSAFE(t->tiles, thing_tile) {

        if (thing_tile->is_join_block) {
            index = IS_JOIN_BLOCK;
        } else if (thing_tile->is_join_t_1) {
            index = IS_JOIN_T_1;
        } else if (thing_tile->is_join_t_2) {
            index = IS_JOIN_T_2;
        } else if (thing_tile->is_join_t_3) {
            index = IS_JOIN_T_3;
        } else if (thing_tile->is_join_t90_1) {
            index = IS_JOIN_T90_1;
        } else if (thing_tile->is_join_t90_2) {
            index = IS_JOIN_T90_2;
        } else if (thing_tile->is_join_t90_3) {
            index = IS_JOIN_T90_3;
        } else if (thing_tile->is_join_t180_1) {
            index = IS_JOIN_T180_1;
        } else if (thing_tile->is_join_t180_2) {
            index = IS_JOIN_T180_2;
        } else if (thing_tile->is_join_t180_3) {
            index = IS_JOIN_T180_3;
        } else if (thing_tile->is_join_t270_1) {
            index = IS_JOIN_T270_1;
        } else if (thing_tile->is_join_t270_2) {
            index = IS_JOIN_T270_2;
        } else if (thing_tile->is_join_t270_3) {
            index = IS_JOIN_T270_3;
        } else if (thing_tile->is_join_block) {
            index = IS_JOIN_BLOCK;
        } else if (thing_tile->is_join_horiz) {
            index = IS_JOIN_HORIZ;
        } else if (thing_tile->is_join_vert) {
            index = IS_JOIN_VERT;
        } else if (thing_tile->is_join_node) {
            index = IS_JOIN_NODE;
        } else if (thing_tile->is_join_left) {
            index = IS_JOIN_LEFT;
        } else if (thing_tile->is_join_right) {
            index = IS_JOIN_RIGHT;
        } else if (thing_tile->is_join_top) {
            index = IS_JOIN_TOP;
        } else if (thing_tile->is_join_bot) {
            index = IS_JOIN_BOT;
        } else if (thing_tile->is_join_tl) {
            index = IS_JOIN_TL;
        } else if (thing_tile->is_join_tr) {
            index = IS_JOIN_TR;
        } else if (thing_tile->is_join_bl) {
            index = IS_JOIN_BL;
        } else if (thing_tile->is_join_br) {
            index = IS_JOIN_BR;
        } else if (thing_tile->is_join_t) {
            index = IS_JOIN_T;
        } else if (thing_tile->is_join_t90) {
            index = IS_JOIN_T90;
        } else if (thing_tile->is_join_t180) {
            index = IS_JOIN_T180;
        } else if (thing_tile->is_join_t270) {
            index = IS_JOIN_T270;
        } else if (thing_tile->is_join_x) {
            index = IS_JOIN_X;
        } else if (thing_tile->is_join_tl2) {
            index = IS_JOIN_TL2;
        } else if (thing_tile->is_join_tr2) {
            index = IS_JOIN_TR2;
        } else if (thing_tile->is_join_bl2) {
            index = IS_JOIN_BL2;
        } else if (thing_tile->is_join_br2) {
            index = IS_JOIN_BR2;
        } else if (thing_tile->is_join_x) {
            index = IS_JOIN_X;
        } else if (thing_tile->is_join_x1) {
            index = IS_JOIN_X1;
        } else if (thing_tile->is_join_x1_270) {
            index = IS_JOIN_X1_270;
        } else if (thing_tile->is_join_x1_180) {
            index = IS_JOIN_X1_180;
        } else if (thing_tile->is_join_x1_90) {
            index = IS_JOIN_X1_90;
        } else if (thing_tile->is_join_x2) {
            index = IS_JOIN_X2;
        } else if (thing_tile->is_join_x2_270) {
            index = IS_JOIN_X2_270;
        } else if (thing_tile->is_join_x2_180) {
            index = IS_JOIN_X2_180;
        } else if (thing_tile->is_join_x2_90) {
            index = IS_JOIN_X2_90;
        } else if (thing_tile->is_join_x3) {
            index = IS_JOIN_X3;
        } else if (thing_tile->is_join_x3_180) {
            index = IS_JOIN_X3_180;
        } else if (thing_tile->is_join_x4) {
            index = IS_JOIN_X4;
        } else if (thing_tile->is_join_x4_270) {
            index = IS_JOIN_X4_270;
        } else if (thing_tile->is_join_x4_180) {
            index = IS_JOIN_X4_180;
        } else if (thing_tile->is_join_x4_90) {
            index = IS_JOIN_X4_90;
        } else if (thing_tile->is_join_horiz2) {
            index = IS_JOIN_HORIZ2;
        } else if (thing_tile->is_join_vert2) {
            index = IS_JOIN_VERT2;
        } else {
            continue;
        }

        t->tilep_join[index][ t->tilep_join_count[index] ] = thing_tile;

        tile = tile_find(thing_tile_name(thing_tile));
        if (!tile) {
            ERR("no tile for join index %d for %s, tile name %s",
                index, t->short_name, thing_tile_name(thing_tile));
        }

        t->tilep_join_tile[index][ t->tilep_join_count[index] ] = tile;

        if (t->tilep_join_count[index]++ >= IS_JOIN_ALT_MAX) {
            ERR("too many tile join alternatives for %s", t->short_name);
        }
    }
}

tpp tp_load (int id, const char *name)
{
    tpp t;

    if (tp_find(name)) {
        ERR("thing template name [%s] already used", name);
    }

    if (!thing_templates) {
        thing_templates = tree_alloc(TREE_KEY_STRING, "TREE ROOT: thing");
    }

    if (!thing_templates_create_order) {
        thing_templates_create_order =
                        tree_alloc(TREE_KEY_INTEGER,
                                   "TREE ROOT: thing create order");
    }

    if (id >= TP_MAX - 1) {
        ERR("too many thing templates");
    }

    t = &thing_templates_chunk[id];
    newptr(t, "tpp");

    t->tree.key = dupstr(name, "TREE KEY: thing");

    t->tp_id = id;

    if (!tree_insert_static(thing_templates, &t->tree.node)) {
        ERR("thing template insert name [%s] failed", name);
    }

    static int create_order;

    t->tree2.key = create_order++;
    if (!tree_insert_static(thing_templates_create_order, &t->tree2.node)) {
        ERR("thing template insert create order [%s] failed", name);
    }

    tp_fill_cache(t);

    return (t);
}

static void tp_destroy_internal (tpp t)
{
    tree_destroy(&t->tiles, (tree_destroy_func)thing_tile_free);

    if (t->short_name) {
        myfree(t->short_name);
    }

    if (t->raw_name) {
        myfree(t->raw_name);
    }

    if (t->light_tint) {
        myfree(t->light_tint);
    }

    oldptr(t);
}

/*
 * Find an existing thing.
 */
tpp tp_find (const char *name)
{
    thing_template target;
    tpp result;

    if (!name) {
        ERR("no name for thing find");
    }

    // memset(&target, 0, sizeof(target)); intentional for speed
    target.tree.key = (char*) name;

    result = (TYPEOF(result)) tree_find(thing_templates, &target.tree.node);
    if (!result) {
        return (0);
    }

    return (result);
}

tpp tp_find_short_name (const char *name)
{
    tree_root *tree;
    tpp t;

    tree = thing_templates;

    TREE_WALK(tree, t) {
        if (!strcasecmp(name, t->short_name)) {
            return (t);
        }
    }

    ERR("did not find short template name \"%s\"", name);

    return (0);
}

/*
 * Given a string name, map to a thing template.
 */
tpp string2thing_template (const char **s)
{
    static char tmp[MAXSTR];
    static const char *eo_tmp = tmp + MAXSTR;
    const char *c = *s;
    char *t = tmp;

    while (t < eo_tmp) {
        if ((*c == '\0') || (*c == '$')) {
            break;
        }

        *t++ = *c++;
    }

    if (c == eo_tmp) {
        return (0);
    }

    *t++ = '\0';
    *s += (t - tmp);

    thing_template find;
    tpp target;

    memset(&find, 0, sizeof(find));
    find.tree.key = tmp;

    target = (TYPEOF(target)) tree_find(thing_templates, &find.tree.node);
    if (!target) {
        ERR("unknown thing [%s]", tmp);
    }

    return (target);
}

const char *tp_name (tpp t)
{
    return (t->tree.key);
}

const char *tp_short_name (tpp t)
{
    return (t->short_name);
}

const char *tp_raw_name (tpp t)
{
    return (t->raw_name);
}

const char *tp_light_tint (tpp t)
{
    return (t->light_tint);
}

color tp_light_color (tpp t)
{
    return (t->light_color);
}

int tp_get_z_depth (tpp t)
{
    return (t->z_depth);
}

int tp_get_z_order (tpp t)
{
    return (t->z_order);
}

int tp_get_speed (tpp t)
{
    return (t->speed);
}

double tp_get_light_radius (tpp t)
{
    return (t->light_radius);
}

double tp_get_scale (tpp t)
{
    if (!t->scale) {
        return (1.0);
    }

    return (t->scale);
}

tree_rootp tp_get_tiles (tpp t)
{
    return (t->tiles);
}

tilep tp_first_tile (tpp tp)
{
    tree_rootp thing_tiles;

    thing_tiles = tp_get_tiles(tp);
    if (!thing_tiles) {
        DIE("tp %s has no tiles", tp_name(tp));
    }

    thing_tilep thing_tile;

    /*
     * Get the first anim tile.
     */
    thing_tile = (TYPEOF(thing_tile)) tree_root_first(thing_tiles);

    if (!thing_tile) {
        DIE("tp %s has no tiles", tp_name(tp));
    }

    return (thing_tile->tile);
}
