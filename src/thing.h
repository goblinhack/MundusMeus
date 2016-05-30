/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the LICENSE file.
 */

#pragma once

#include "tree.h"
#include "wid.h"
#include "thing_template.h"
#include <stdlib.h>
#include <math.h>

extern int things_total;
extern int things_total;
extern int monst_things_total;
extern int monst_things_total;

uint8_t thing_init(void);
void thing_fini(void);
void thing_map_sanity(levelp level);
void thing_sanity(levelp, thingp);
void thing_map_dump(void);
thingp thing_new(levelp,
                 const char *name, 
                 double x, double y);
thingp thing_change_level(levelp old_level,
                          levelp new_level,
                          thingp o);
void thing_reinit(levelp, thingp, double x, double y);
void thing_restarted(levelp, thingp t);
void thing_destroy(levelp, thingp, const char *why);
void thing_destroy_in(levelp level, thingp t, int32_t delay);
void thing_remove_hooks(levelp level, thingp t);
int thing_tick_all(levelp level);
void thing_animate_all(levelp level);
void thing_tick_player_slow_all(levelp level,
                                int force);
uint8_t thing_mob_spawn(levelp, thingp);
thingp thing_mob_spawn_on_death(levelp, thingp);
void thing_wake(levelp, thingp t);
void thing_dead(levelp, thingp, thingp killer,
                const char *fmt, ...) __attribute__ ((format (printf, 4, 5)));
int thing_hit(levelp, thingp, thingp hitter, uint32_t damage);
void thing_hide(levelp, thingp);
thingp thing_owner(levelp, thingp t);
uint8_t thing_is_visible(levelp, thingp);
void thing_leave_level(levelp, thingp);
void thing_join_level(levelp, thingp);
void thing_visible(levelp, thingp);
void things_level_destroyed(levelp, uint8_t keep_player);
int thing_is_player_or_owned_by_player(levelp, thingp t);
void demarshal_thing(demarshal_p ctx, thingp);
void marshal_thing(marshal_p ctx, thingp);
void thing_templates_marshal(marshal_p out);
void things_marshal(marshal_p out);
void thing_set_wid(levelp, thingp, widp);
void thing_map_remove(levelp, thingp t);
void thing_map_add(levelp, thingp t, int32_t x, int32_t y);
widp thing_wid(thingp);
const char *thing_name(thingp);
const char *thing_logname(thingp);
const char *thing_short_name(thingp);
uint8_t thing_z_depth(thingp);
uint8_t thing_z_order(thingp);
tree_rootp thing_tile_tiles(thingp);
void thing_animate(levelp, thingp);
thingp thing_hit_solid_obstacle(levelp, thingp t, double nx, double ny);
thingp thing_hit_fall_obstacle(levelp, thingp t, double nx, double ny);
thingp thing_hit_any_obstacle(levelp, thingp t, double nx, double ny);
typedef thingp (*thing_hit_obstacle_fn)(levelp level, thingp t, 
                                        double nx, double ny);

typedef uint8_t (*thing_is_fn)(thingp t);
thingp thing_overlaps(levelp level, thingp t, double nx, double ny,
                      thing_is_fn);

void thing_set_is_light_source(thingp t, uint8_t val);
uint8_t thing_is_light_source(thingp t);
void thing_set_is_candle_light(thingp t, uint8_t val);
uint8_t thing_is_candle_light(thingp t);
void thing_set_is_sleeping(thingp t, uint8_t val);
void thing_set_is_collected(thingp t, uint8_t val);
uint8_t thing_is_collected(thingp t);
void thing_set_is_chasing_target(thingp t, uint8_t val);
uint8_t thing_is_chasing_target(thingp t);
void thing_set_redo_maze_search(thingp t, uint8_t val);
uint8_t thing_redo_maze_search(thingp t);

void thing_set_dir_none(levelp, thingp t);
uint8_t thing_is_dir_none(thingp t);
void thing_set_dir_down(levelp, thingp t);
uint8_t thing_is_dir_down(thingp t);
void thing_set_dir_up(levelp, thingp t);
uint8_t thing_is_dir_up(thingp t);
void thing_set_dir_left(levelp, thingp t);
uint8_t thing_is_dir_left(thingp t);
void thing_set_dir_right(levelp, thingp t);
uint8_t thing_is_dir_right(thingp t);

void thing_set_dir_tl(levelp, thingp t);
uint8_t thing_is_dir_tl(thingp t);
void thing_set_dir_bl(levelp, thingp t);
uint8_t thing_is_dir_bl(thingp t);
void thing_set_dir_tr(levelp, thingp t);
uint8_t thing_is_dir_tr(thingp t);
void thing_set_dir_br(levelp, thingp t);
uint8_t thing_is_dir_br(thingp t);

void thing_set_opened_exit(thingp t, uint8_t val);
uint8_t thing_opened_exit(thingp t);
void thing_set_is_open(thingp t, uint8_t val);
void thing_set_is_angry(thingp t, uint8_t val);
void thing_set_is_projectile(thingp t, uint8_t val);
void thing_set_is_dead(thingp t, uint8_t val);
void thing_set_is_game_over(thingp t, uint8_t val);
const char *thing_name(thingp);
const char *thing_short_name(thingp);
const char *thing_tooltip(thingp);
uint8_t thing_z_depth(thingp);
uint8_t thing_z_order(thingp);
int32_t thing_player_cost(thingp);
int32_t thing_monst_cost(thingp);
double thing_speed(thingp);
tree_rootp thing_tiles(thingp);

thing_tilep thing_current_tile(thingp t);
void thing_place_timed(levelp,
                       tpp t, 
                       double x,
                       double y,
                       uint32_t ms, 
                       uint32_t jitter);
void thing_place_and_destroy_timed(levelp,
                                   tpp t, 
                                   thingp owner,
                                   double x,
                                   double y,
                                   uint32_t ms, 
                                   uint32_t destroy_in, 
                                   uint32_t jitter,
                                   uint8_t is_epicenter);
void thing_wid_update(levelp,
                      thingp t, 
                      double x, double y, 
                      uint8_t smooth, 
                      uint8_t is_new);
uint8_t thing_use(thingp t, uint32_t id);

/*
 * thing_ai.c
 */
int thing_run_from(levelp level,
                   thingp t, 
                   double *nexthop_x, 
                   double *nexthop_y, 
                   tpp tp);
int dmap_distance_to_player(int source_x, int source_y);
int dmap_distance_between_points(int target_x, int target_y, int source_x, int source_y);
uint8_t thing_find_nexthop(levelp, thingp t, double *x, double *y);
void dmap_process_init(void);
void dmap_process_fini(void);
void dmap_generate_map(double x, double y);
void dmap_generate_map_wander(levelp level);
void dmap_generate(levelp level, int force);

typedef struct {
    /*
     * First expiry
     */
    int16_t fire_in;

    /*
     * Second expiry
     */
    int16_t destroy_in;

    tpp tp;
    float x;
    float y;

    uint32_t owner_id;
    uint32_t thing_id;

    /*
     * Center of an explosion.
     */
    uint8_t is_epicenter:1;

} thing_place_context_t;

void thing_teleport(levelp, thingp t, int32_t x, int32_t y);

enum {
    THING_DIR_NONE,
    THING_DIR_DOWN,
    THING_DIR_UP,
    THING_DIR_LEFT,
    THING_DIR_RIGHT,
    THING_DIR_TL,
    THING_DIR_BL,
    THING_DIR_TR,
    THING_DIR_BR,
};

void thing_set_owner_id(levelp, thingp t, uint32_t owner_id);
void thing_set_owner(levelp, thingp t, thingp owner);

enum {
    THING_NONE,
    THING_WALL1,
    THING_WALL1_DECO,
    THING_BLOCK1,
    THING_CRATE1,
    THING_SPIKES1,
    THING_SPIKES2,
    THING_SPIKES3,
    THING_SPIKES4,
    THING_SPIKES5,
    THING_SPIKES6,
    THING_SMALLROCK1,
    THING_SMALLROCK2,
    THING_SMALLROCK3,
    THING_SMALLROCK4,
    THING_SMALLROCK5,
    THING_SMALLROCK6,
    THING_BOULDER1,
    THING_BOULDER2,
    THING_CHOCOLATEFROG,
    THING_GOLD1,
    THING_GOLD2,
    THING_GOLD3,
    THING_ROPE,
    THING_ROPETOP,
    THING_ROPEPILE,
    THING_LADDER1,
    THING_LADDER1_DECO,
    THING_DOOR1,
    THING_NOENTRY,
    THING_SHOP_FLOOR1,
    THING_FLOOR1,
    THING_SORCEROR,
    THING_SORCERORMOB,
    THING_PLAYER,
    THING_ZOMBIE1,
    THING_ZOMBIE2,
    THING_ZOMBIE3,
    THING_ZOMBIE4,
    THING_ZOMBIE5,
    THING_ZOMBIE_CORPSE1,
    THING_ZOMBIE_CORPSE2,
    THING_ZOMBIE_CORPSE3,
    THING_ZOMBIE_CORPSE4,
    THING_ZOMBIE_CORPSE5,
    THING_SKELETON1,
    THING_PLANTPOD,
    THING_PLANT,
    THING_ORC1,
    THING_ORC2,
    THING_ORC3,
    THING_ORC4,
    THING_ORC5,
    THING_SHOPKEEPER,
    THING_SNAKE1,
    THING_GHOST1,
    THING_GHOST2,
    THING_GHOST3,
    THING_GHOST4,
    THING_DEMON1,
    THING_DEMON2,
    THING_SKULL1,
    THING_SKULL2,
    THING_SQUIDLING1,
    THING_SQUIDLING2,
    THING_EYEBAWL,
    THING_SPIDER1,
    THING_SPIDER2,
    THING_CLUB1_ANIM,
    THING_STICK1_ANIM,
    THING_SHOTGUN_ANIM,
    THING_SWORD1_ANIM,
    THING_BOW1_ANIM,
    THING_AXE1_ANIM,
    THING_SCYTHE1_ANIM,
    THING_WEAPON_HIT1,
    THING_WEAPON_HIT2,
    THING_EXPLOSION1,
    THING_EXPLOSION2,
    THING_EXPLOSION3,
    THING_HIT_SUCCESS,
    THING_HIT_MISS,
    THING_BLOOD1,
    THING_FLAMES1,
    THING_BUBBLES1,
    THING_ACTION_LEFT,
    THING_ACTION_RIGHT,
    THING_ACTION_UP,
    THING_ACTION_DOWN,
    THING_ACTION_TRIGGER_HERO,
    THING_ACTION_TRIGGER_WALL,
    THING_ACTION_TRIGGER_MONST,
    THING_ACTION_SLEEP,
    THING_ACTION_ZAP,
    THING_WATER1,
    THING_FOOD_ANY, 
    THING_MOB_ANY, 
    THING_MONST_ANY, 
    THING_TREASURE_ANY, 
    THING_WEAPON_ANY, 
    THING_TRAP1,
    THING_TRAP2,
    THING_TRAP3,
    THING_GEM1,
    THING_GEM2,
    THING_GEM3,
    THING_TELEPORT1,
    THING_LAVA1,
    THING_ACID1,
#define THING_LAVA2 THING_LAVA1
#define THING_WATER2 THING_WATER1
#define THING_ACID2 THING_ACID1
#define THING_LAVA3 THING_LAVA1
#define THING_WATER3 THING_WATER1
#define THING_ACID3 THING_ACID1
#define THING_LAVA4 THING_LAVA1
#define THING_WATER4 THING_WATER1
#define THING_ACID4 THING_ACID1
    THING_EXIT1,
    THING_ENTRANCE1,
    THING_GENERATOR1,
    THING_ZOMBIEMOB,
    THING_ORCMOB,
    THING_GHOSTMOB1,
    THING_GHOSTMOB2,
    THING_GHOSTMOB3,
    THING_GHOSTMOB4,
    THING_EYEBAWLMOB,
    THING_BOMB,
    THING_BOW1,
    THING_CLUB1,
    THING_SHOTGUN,
    THING_PELLETS,
    THING_STICK1,
    THING_SWORD1,
    THING_AXE1,
    THING_SCYTHE1,
    THING_KEY,
    THING_LIGHT,
    THING_LIGHT2,
    THING_TORCH,
    THING_COINS1,
    THING_CHEST1,
    THING_ARROW,
    THING_FIREBALL1,
    THING_FIREBALL2,
    THING_FIREBALL3,
    THING_FIREBALL4,
    THING_ENERGY1,
    THING_ENERGY2,
    THING_PLASMABALL1,
    THING_PLASMABALL2,
    THING_SILKBALL,

    TP_MAX_ID,
};

#define THING_FIRST 1

typedef struct thing_ {

    uint32_t thing_id;

    /*
     * Pointer to common settings for this thing.
     */
    uint16_t tp_id;

    /*
     * Thing stats
     */
    int16_t hp;
    int16_t max_hp;
    int32_t gold;
    int32_t gold_owed;
    int32_t score;
    int32_t keys;
    int32_t torches;
    int32_t ropes;
    int32_t bombs;

    /*-------------------------------------------------------------------
     * Dynamic things we cannot save.
     *-------------------------------------------------------------------*/

    /*
     * Widget for displaying thing.
     */
    widp wid;

    /*-------------------------------------------------------------------
     * Dynamic things we cannot save.
     *-------------------------------------------------------------------*/

    /*
     * Thing dead in x ms. The tree can be the global tree or an owner tree.
     */
    int32_t destroy_in_ms;
    
    /*
     * Data we read in along with the thing template.
     */
    thing_template_data data;

    uint32_t weapon_carry_anim_thing_id;
    uint32_t weapon_swing_anim_thing_id;

    /*
     * Used to detect exit...
     */
    uint32_t in_shop_owned_by_thing_id;

    /*
     * Weapon thing template.
     */
    uint16_t weapon_tp_id;

    /*
     * Who created this thing? e.g. who cast a spell?
     */
    uint32_t owner_thing_id;

    /*
     * How many things this thing owns.
     */
    uint16_t owned_count;

    /*
     * How much damage per hit?
     */
    uint16_t damage;

    /*
     * Which djkstra map this thing is using.
     */
    uint8_t dmap_id;
    uint8_t dmap_wander_x;
    uint8_t dmap_wander_y;

    /*
     * Grid coordinates.
     */
    float x;
    float y;
    fpoint normal_velocity;
    fpoint tangent_velocity;

    /*
     * Last anim frame position. To be able to detect moving things.
     */
    float anim_x;
    float anim_y;

    /*
     * For freefalling.
     */
    float fall_speed;
    float jump_speed;
    float momentum;
    float rot;

    /*
     * How close for collision detection.
     */
    float collision_radius;

    /*
     * Map grid co-ordinates.
     */
    int16_t map_x;
    int16_t map_y;

    /*
     * For animation.
     */
    uint16_t current_tile;

    /*
     * Previous hop where we were. We use this to interpolate the real 
     * position when moving.
     */
    float last_x;
    float last_y;

    /*
     * For moving
     */
    float dx;
    float dy;

    /*
     * Thing to be shown scaled on the client
     */
    float scale;

    /*
     * Last time we were underwater.
     */
    uint32_t timestamp_last_submerged;

    /*
     * Last time we were teleported.
     */
    uint32_t timestamp_last_teleport;

    /*
     * So players are not killed too quickly
     */
    uint32_t timestamp_last_hit;

    /*
     * Last time we entered a shop.
     */
    uint32_t timestamp_last_shop_enter;

    /*
     * Last time we were attacked.
     */
    uint32_t timestamp_last_attacked;

    /*
     * When to change frame for animation.
     */
    uint32_t timestamp_change_to_next_frame;
    uint32_t timestamp_anim_stopped_moving;

    /*
     * When we last teleported.
     */
    uint32_t timestamp_teleport;

    /*
     * When we last checked for hitting something.
     */
    uint32_t timestamp_collision;

    /*
     * When we last spawned.
     */
    uint32_t timestamp_mob_spawn;

    /*
     * When did I last hit?
     */
    uint32_t timestamp_i_attacked;

    /*
     * When life sputtered into this corpse
     */
    uint32_t tick_born;

    /*
     * Last time it fired at a player.
     */
    uint32_t timestamp_fired;

    /*
     * Last utterance
     */
    uint32_t timestamp_sound_random;

    /*
     * e.g. IS_JOIN_BLOCK
     */
    uint8_t join_index;

    uint8_t dir;

    /*
     * How many rays of light are hitting this thing?
     */
    uint16_t lit;

    /*
     * How much light is this thing emitting. This cannot exceed the light 
     * strength of the template. This is used for torches and the like to see 
     * how much we can really see.
     */
    float torch_light_radius;
    uint32_t tick_torch;

    /*
     * For auto decrementing health if over max due to magical boost.
     */
    uint32_t tick_health;

    /*
     * Hit a limit and you drown.
     */
    uint32_t breath;
    uint32_t timestamp_last_breath;

    /*
     * Debugging this thing?
     */
    uint32_t debug:1;

    uint32_t is_sleeping:1;
    uint32_t is_submerged:1;
    uint32_t is_partially_submerged:1;
    uint32_t is_collected:1;
    uint32_t is_open:1;
    uint32_t is_dead:1;
    uint32_t is_bloodied:1;
    uint32_t is_angry:1;

    /*
     * Center of an explosion.
     */
    uint32_t is_epicenter:1;

    uint32_t is_moving:1;

    /*
     * Limit certain message types.
     */
    uint32_t message_open_door_need_key:1;

    /*
     * Do we have a light source?
     */
    uint32_t torch_light_radius_set:1;

    /*
     * Moves only occasionally?
     */
    uint32_t one_shot_move:1;

    /*
     * In the middle of a jump animation?
     */
    uint32_t is_jumping:1;
    uint32_t want_to_jump:1;
    uint32_t falling_too_fast:1;
} thing;

#include "thing_template.h"

static inline tpp thing_tp (thingp t)
{
    verify(t);

    return (id_to_tp(t->tp_id));
}

static inline uint8_t thing_is_open (thingp t)
{
    verify(t);

    return (t->is_open);
}

static inline uint8_t thing_is_angry (thingp t)
{
    verify(t);

    return (t->is_angry);
}

static inline uint8_t thing_is_dead (thingp t)
{
    verify(t);

    return (t->is_dead);
}

static inline uint8_t thing_is_bloodied (thingp t)
{
    verify(t);

    return (t->is_bloodied);
}

static inline uint8_t thing_is_epicenter (thingp t)
{
    verify(t);

    return (t->is_epicenter);
}

static inline uint8_t thing_is_exit (thingp t)
{
    verify(t);

    return (tp_is_exit(thing_tp(t)));
}

static inline uint8_t thing_is_dungeon_floor (thingp t)
{
    verify(t);

    return (tp_is_dungeon_floor(thing_tp(t)));
}

static inline uint8_t thing_is_food (thingp t)
{
    verify(t);

    return (tp_is_food(thing_tp(t)));
}

static inline uint8_t thing_is_monst (thingp t)
{
    verify(t);

    return (tp_is_monst(thing_tp(t)));
}

static inline uint8_t thing_is_mob_spawner (thingp t)
{
    verify(t);

    return (tp_is_mob_spawner(thing_tp(t)));
}

static inline uint8_t thing_is_player (thingp t)
{
    verify(t);

    return (tp_is_player(thing_tp(t)));
}

static inline uint8_t thing_is_key (thingp t)
{
    verify(t);

    return (tp_is_key(thing_tp(t)));
}

static inline uint8_t thing_is_collision_map_large (thingp t)
{
    verify(t);

    return (tp_is_collision_map_large(thing_tp(t)));
}

static inline uint8_t thing_is_collision_map_medium (thingp t)
{
    verify(t);

    return (tp_is_collision_map_medium(thing_tp(t)));
}

static inline uint8_t thing_is_collision_map_small (thingp t)
{
    verify(t);

    return (tp_is_collision_map_small(thing_tp(t)));
}

static inline uint8_t thing_is_collision_map_tiny (thingp t)
{
    verify(t);

    return (tp_is_collision_map_tiny(thing_tp(t)));
}

static inline uint8_t thing_is_rock (thingp t)
{
    verify(t);

    return (tp_is_rock(thing_tp(t)));
}

static inline uint8_t thing_is_shadow_caster (thingp t)
{
    verify(t);

    return (tp_is_shadow_caster(thing_tp(t)));
}

static inline uint8_t thing_is_shadow_caster_soft (thingp t)
{
    verify(t);

    return (tp_is_shadow_caster_soft(thing_tp(t)));
}

static inline uint8_t thing_is_weapon (thingp t)
{
    verify(t);

    return (tp_is_weapon(thing_tp(t)));
}

static inline uint8_t thing_is_carryable (thingp t)
{
    verify(t);

    return (tp_is_carryable(thing_tp(t)));
}

static inline uint8_t thing_is_treasure (thingp t)
{
    verify(t);

    return (tp_is_treasure(thing_tp(t)));
}

static inline uint8_t thing_is_fragile (thingp t)
{
    verify(t);

    return (tp_is_fragile(thing_tp(t)));
}

static inline uint8_t thing_is_animated_no_dir (thingp t)
{
    verify(t);

    return (tp_is_animated_no_dir(thing_tp(t)));
}

static inline uint8_t thing_is_weapon_swing_effect (thingp t)
{
    verify(t);

    return (tp_is_weapon_swing_effect(thing_tp(t)));
}

static inline uint8_t thing_is_fire (thingp t)
{
    verify(t);

    return (tp_is_fire(thing_tp(t)));
}

static inline double thing_collision_radius (thingp t)
{
    verify(t);

    if (t->collision_radius > 0) {
        return (t->collision_radius);
    }

    return (tp_get_collision_radius(thing_tp(t)));
}

static inline uint8_t thing_is_cats_eyes (thingp t)
{
    verify(t);

    return (tp_is_cats_eyes(thing_tp(t)));
}

static inline uint8_t thing_is_animation (thingp t)
{
    verify(t);

    return (tp_is_animation(thing_tp(t)));
}

static inline uint8_t thing_is_non_explosive_gas_cloud (thingp t)
{
    verify(t);

    return (tp_is_non_explosive_gas_cloud(thing_tp(t)));
}

static inline uint8_t thing_is_item_unusable (thingp t)
{
    verify(t);

    return (tp_is_item_unusable(thing_tp(t)));
}

static inline uint8_t thing_is_door (thingp t)
{
    verify(t);

    return (tp_is_door(thing_tp(t)));
}

static inline uint8_t thing_is_rrr1 (thingp t)
{
    verify(t);

    return (tp_is_rrr1(thing_tp(t)));
}

static inline uint8_t thing_is_rrr2 (thingp t)
{
    verify(t);

    return (tp_is_rrr2(thing_tp(t)));
}

static inline uint8_t thing_is_rrr3 (thingp t)
{
    verify(t);

    return (tp_is_rrr3(thing_tp(t)));
}

static inline uint8_t thing_is_rrr4 (thingp t)
{
    verify(t);

    return (tp_is_rrr4(thing_tp(t)));
}

static inline uint8_t thing_is_rrr5 (thingp t)
{
    verify(t);

    return (tp_is_rrr5(thing_tp(t)));
}

static inline uint8_t thing_is_rrr6 (thingp t)
{
    verify(t);

    return (tp_is_rrr6(thing_tp(t)));
}

static inline uint8_t thing_is_rrr7 (thingp t)
{
    verify(t);

    return (tp_is_rrr7(thing_tp(t)));
}

static inline uint8_t thing_is_rrr8 (thingp t)
{
    verify(t);

    return (tp_is_rrr8(thing_tp(t)));
}

static inline uint8_t thing_is_rrr9 (thingp t)
{
    verify(t);

    return (tp_is_rrr9(thing_tp(t)));
}

static inline uint8_t thing_is_rrr10 (thingp t)
{
    verify(t);

    return (tp_is_rrr10(thing_tp(t)));
}

static inline uint8_t thing_is_rrr11 (thingp t)
{
    verify(t);

    return (tp_is_rrr11(thing_tp(t)));
}

static inline uint8_t thing_is_rrr12 (thingp t)
{
    verify(t);

    return (tp_is_rrr12(thing_tp(t)));
}

static inline uint8_t thing_is_rrr13 (thingp t)
{
    verify(t);

    return (tp_is_rrr13(thing_tp(t)));
}

static inline uint8_t thing_is_rrr14 (thingp t)
{
    verify(t);

    return (tp_is_rrr14(thing_tp(t)));
}

static inline uint8_t thing_is_rrr15 (thingp t)
{
    verify(t);

    return (tp_is_rrr15(thing_tp(t)));
}

static inline uint8_t thing_is_rrr16 (thingp t)
{
    verify(t);

    return (tp_is_rrr16(thing_tp(t)));
}

static inline uint8_t thing_is_rrr17 (thingp t)
{
    verify(t);

    return (tp_is_rrr17(thing_tp(t)));
}

static inline uint8_t thing_is_rrr18 (thingp t)
{
    verify(t);

    return (tp_is_rrr18(thing_tp(t)));
}

static inline uint8_t thing_is_rrr19 (thingp t)
{
    verify(t);

    return (tp_is_rrr19(thing_tp(t)));
}

static inline uint8_t thing_is_rrr20 (thingp t)
{
    verify(t);

    return (tp_is_rrr20(thing_tp(t)));
}

static inline uint8_t thing_is_rrr21 (thingp t)
{
    verify(t);

    return (tp_is_rrr21(thing_tp(t)));
}

static inline uint8_t thing_is_rrr22 (thingp t)
{
    verify(t);

    return (tp_is_rrr22(thing_tp(t)));
}

static inline uint8_t thing_is_rrr23 (thingp t)
{
    verify(t);

    return (tp_is_rrr23(thing_tp(t)));
}

static inline uint8_t thing_is_rrr24 (thingp t)
{
    verify(t);

    return (tp_is_rrr24(thing_tp(t)));
}

static inline uint8_t thing_is_wanderer_lr (thingp t)
{
    verify(t);

    return (tp_is_wanderer_lr(thing_tp(t)));
}

static inline uint8_t thing_is_boulder (thingp t)
{
    verify(t);

    return (tp_is_boulder(thing_tp(t)));
}

static inline uint8_t thing_can_roll (thingp t)
{
    verify(t);

    return (tp_can_roll(thing_tp(t)));
}

static inline uint8_t thing_is_bubbles (thingp t)
{
    verify(t);

    return (tp_is_bubbles(thing_tp(t)));
}

static inline uint8_t thing_can_drown (thingp t)
{
    verify(t);

    return (tp_can_drown(thing_tp(t)));
}

static inline uint8_t thing_is_ropepile (thingp t)
{
    verify(t);

    return (tp_is_ropepile(thing_tp(t)));
}

static inline uint8_t thing_is_ropetop (thingp t)
{
    verify(t);

    return (tp_is_ropetop(thing_tp(t)));
}

static inline uint8_t thing_is_climbable (thingp t)
{
    verify(t);

    return (tp_is_climbable(thing_tp(t)));
}

static inline uint8_t thing_is_smallrock (thingp t)
{
    verify(t);

    return (tp_is_smallrock(thing_tp(t)));
}

static inline uint8_t thing_is_rope (thingp t)
{
    verify(t);

    return (tp_is_rope(thing_tp(t)));
}

static inline uint8_t thing_is_throwable (thingp t)
{
    verify(t);

    return (tp_is_throwable(thing_tp(t)));
}

static inline uint8_t thing_is_not_light_blocking (thingp t)
{
    verify(t);

    return (tp_is_not_light_blocking(thing_tp(t)));
}

static inline uint8_t thing_is_spikes (thingp t)
{
    verify(t);

    return (tp_is_spikes(thing_tp(t)));
}

static inline uint8_t thing_is_obstacle (thingp t)
{
    verify(t);

    return (tp_is_obstacle(thing_tp(t)));
}

static inline uint8_t thing_is_ladder (thingp t)
{
    verify(t);

    return (tp_is_ladder(thing_tp(t)));
}

static inline uint8_t thing_is_ladder_deco (thingp t)
{
    verify(t);

    return (tp_is_ladder_deco(thing_tp(t)));
}

static inline uint8_t thing_is_entrance (thingp t)
{
    verify(t);

    return (tp_is_entrance(thing_tp(t)));
}

static inline uint8_t thing_is_wall_deco (thingp t)
{
    verify(t);

    return (tp_is_wall_deco(thing_tp(t)));
}

static inline uint8_t thing_is_conical_breath_attack (thingp t)
{
    verify(t);

    return (tp_is_conical_breath_attack(thing_tp(t)));
}

static inline uint8_t thing_is_corpse (thingp t)
{
    verify(t);

    return (tp_is_corpse(thing_tp(t)));
}

static inline uint8_t thing_is_spawns_under (thingp t)
{
    verify(t);

    return (tp_is_spawns_under(thing_tp(t)));
}

static inline uint8_t thing_is_wanderer (thingp t)
{
    verify(t);

    return (tp_is_wanderer(thing_tp(t)));
}

static inline uint8_t thing_can_fall (thingp t)
{
    verify(t);

    return (tp_can_fall(thing_tp(t)));
}

static inline uint8_t thing_is_life_saving (thingp t)
{
    verify(t);

    return (tp_is_life_saving(thing_tp(t)));
}

static inline uint8_t thing_is_single_mob_spawner (thingp t)
{
    verify(t);

    return (tp_is_single_mob_spawner(thing_tp(t)));
}

static inline uint8_t thing_is_water (thingp t)
{
    verify(t);

    return (tp_is_water(thing_tp(t)));
}

static inline uint8_t thing_is_undead (thingp t)
{
    verify(t);

    return (tp_is_undead(thing_tp(t)));
}

static inline uint8_t thing_is_hidden (thingp t)
{
    verify(t);

    return (tp_is_hidden(thing_tp(t)));
}

static inline uint8_t thing_is_trap (thingp t)
{
    verify(t);

    return (tp_is_trap(thing_tp(t)));
}

static inline uint8_t thing_is_corridor_wall (thingp t)
{
    verify(t);

    return (tp_is_corridor_wall(thing_tp(t)));
}

static inline uint8_t thing_is_corridor (thingp t)
{
    verify(t);

    return (tp_is_corridor(thing_tp(t)));
}

static inline uint8_t thing_is_water_proof (thingp t)
{
    verify(t);

    return (tp_is_water_proof(thing_tp(t)));
}

static inline uint8_t thing_is_lava_proof (thingp t)
{
    verify(t);

    return (tp_is_lava_proof(thing_tp(t)));
}

static inline uint8_t thing_is_spider_proof (thingp t)
{
    verify(t);

    return (tp_is_spider_proof(thing_tp(t)));
}

static inline uint8_t thing_is_acid_proof (thingp t)
{
    verify(t);

    return (tp_is_acid_proof(thing_tp(t)));
}

static inline uint8_t thing_is_fires_when_angry (thingp t)
{
    verify(t);

    return (tp_is_fires_when_angry(thing_tp(t)));
}

static inline uint8_t thing_is_shopkeeper (thingp t)
{
    verify(t);

    return (tp_is_shopkeeper(thing_tp(t)));
}

static inline uint8_t thing_is_shop_floor (thingp t)
{
    verify(t);

    return (tp_is_shop_floor(thing_tp(t)));
}

static inline uint8_t thing_is_fireball (thingp t)
{
    verify(t);

    return (tp_is_fireball(thing_tp(t)));
}

static inline uint8_t thing_is_bullet (thingp t)
{
    verify(t);

    return (tp_is_bullet(thing_tp(t)));
}

static inline uint8_t thing_is_effect_fade_in_out (thingp t)
{
    verify(t);

    return (tp_is_effect_fade_in_out(thing_tp(t)));
}

static inline uint8_t thing_is_internal (thingp t)
{
    verify(t);

    return (tp_is_internal(thing_tp(t)));
}

static inline uint8_t thing_is_levitating (thingp t)
{
    verify(t);

    return (tp_is_levitating(thing_tp(t)));
}

static inline uint8_t thing_is_acid (thingp t)
{
    verify(t);

    return (tp_is_acid(thing_tp(t)));
}

static inline uint8_t thing_is_lava (thingp t)
{
    verify(t);

    return (tp_is_lava(thing_tp(t)));
}

static inline uint8_t thing_is_teleport (thingp t)
{
    verify(t);

    return (tp_is_teleport(thing_tp(t)));
}

static inline uint8_t thing_is_cobweb (thingp t)
{
    verify(t);

    return (tp_is_cobweb(thing_tp(t)));
}

static inline uint8_t thing_is_ethereal (thingp t)
{
    verify(t);

    return (tp_is_ethereal(thing_tp(t)));
}

static inline uint8_t thing_is_variable_size (thingp t)
{
    verify(t);

    return (tp_is_variable_size(thing_tp(t)));
}

static inline uint8_t thing_is_magical_weapon (thingp t)
{
    verify(t);

    return (tp_is_magical_weapon(thing_tp(t)));
}

static inline uint8_t thing_is_ranged_weapon (thingp t)
{
    verify(t);

    return (tp_is_ranged_weapon(thing_tp(t)));
}

static inline uint8_t thing_is_melee_weapon (thingp t)
{
    verify(t);

    return (tp_is_melee_weapon(thing_tp(t)));
}

static inline uint8_t thing_is_cloud_effect (thingp t)
{
    verify(t);

    return (tp_is_cloud_effect(thing_tp(t)));
}

static inline uint8_t thing_is_hard (thingp t)
{
    verify(t);

    return (tp_is_hard(thing_tp(t)));
}

static inline uint8_t thing_is_sleeping (thingp t)
{
    verify(t);

    return (t->is_sleeping);
}

static inline uint8_t thing_is_jumping (thingp t)
{
    verify(t);

    return (t->is_jumping);
}

static inline uint8_t thing_is_awake (thingp t)
{
    verify(t);

    return (!t->is_sleeping);
}

static inline uint8_t thing_is_bomb (thingp t)
{
    verify(t);

    return (tp_is_bomb(thing_tp(t)));
}

static inline uint8_t thing_is_sawblade (thingp t)
{
    verify(t);

    return (tp_is_sawblade(thing_tp(t)));
}

static inline uint8_t thing_is_visible_on_debug_only (thingp t)
{
    verify(t);

    return (tp_is_visible_on_debug_only(thing_tp(t)));
}

static inline uint8_t thing_is_action (thingp t)
{
    verify(t);

    return (tp_is_action(thing_tp(t)));
}

static inline uint8_t thing_is_action_sleep (thingp t)
{
    verify(t);

    return (tp_is_action_sleep(thing_tp(t)));
}

static inline uint8_t thing_is_action_zap (thingp t)
{
    verify(t);

    return (tp_is_action_zap(thing_tp(t)));
}

static inline uint8_t thing_is_action_trigger (thingp t)
{
    verify(t);

    return (tp_is_action_trigger(thing_tp(t)));
}

static inline uint8_t thing_is_action_trigger_on_wall (thingp t)
{
    verify(t);

    return (tp_is_action_trigger_on_wall(thing_tp(t)));
}

static inline uint8_t thing_is_action_trigger_on_hero (thingp t)
{
    verify(t);

    return (tp_is_action_trigger_on_hero(thing_tp(t)));
}

static inline uint8_t thing_is_action_trigger_on_monst (thingp t)
{
    verify(t);

    return (tp_is_action_trigger_on_monst(thing_tp(t)));
}

static inline uint8_t thing_is_action_down (thingp t)
{
    verify(t);

    return (tp_is_action_down(thing_tp(t)));
}

static inline uint8_t thing_is_action_up (thingp t)
{
    verify(t);

    return (tp_is_action_up(thing_tp(t)));
}

static inline uint8_t thing_is_action_left (thingp t)
{
    verify(t);

    return (tp_is_action_left(thing_tp(t)));
}

static inline uint8_t thing_is_action_right (thingp t)
{
    verify(t);

    return (tp_is_action_right(thing_tp(t)));
}

static inline uint8_t thing_can_walk_through (thingp t)
{
    verify(t);

    return (tp_can_walk_through(thing_tp(t)));
}

static inline uint8_t thing_is_weapon_carry_anim (thingp t)
{
    verify(t);

    return (tp_is_weapon_carry_anim(thing_tp(t)));
}

static inline uint8_t thing_is_animated_only_when_moving (thingp t)
{
    verify(t);

    return (tp_is_animated_only_when_moving(thing_tp(t)));
}

static inline uint8_t thing_is_warm_blooded (thingp t)
{
    verify(t);

    return (tp_is_warm_blooded(thing_tp(t)));
}

static inline uint8_t thing_can_be_enchanted (thingp t)
{
    verify(t);

    return (tp_can_be_enchanted(thing_tp(t)));
}

static inline uint8_t thing_is_stackable (thingp t)
{
    verify(t);

    return (tp_is_stackable(thing_tp(t)));
}

static inline uint8_t thing_is_torch (thingp t)
{
    verify(t);

    return (tp_is_torch(thing_tp(t)));
}

static inline uint8_t thing_is_explosion (thingp t)
{
    verify(t);

    return (tp_is_explosion(thing_tp(t)));
}

static inline uint8_t thing_is_hidden_from_editor (thingp t)
{
    verify(t);

    return (tp_is_hidden_from_editor(thing_tp(t)));
}

static inline uint8_t thing_is_animated (thingp t)
{
    verify(t);

    return (tp_is_animated(thing_tp(t)));
}

static inline uint8_t thing_is_combustable (thingp t)
{
    verify(t);

    return (tp_is_combustable(thing_tp(t)));
}

static inline uint8_t thing_is_projectile (thingp t)
{
    verify(t);

    return (tp_is_projectile(thing_tp(t)));
}

static inline uint8_t thing_is_joinable (thingp t)
{
    verify(t);

    return (tp_is_joinable(thing_tp(t)));
}

static inline uint8_t thing_is_wall (thingp t)
{
    verify(t);

    return (tp_is_wall(thing_tp(t)));
}

static inline uint8_t thing_is_effect_sway (thingp t)
{
    verify(t);

    return (tp_is_effect_sway(thing_tp(t)));
}

static inline uint8_t thing_is_effect_pulse (thingp t)
{
    verify(t);

    return (tp_is_effect_pulse(thing_tp(t)));
}

static inline uint8_t thing_is_effect_rotate_2way (thingp t)
{
    verify(t);

    return (tp_is_effect_rotate_2way(thing_tp(t)));
}

static inline tpp thing_weapon (const thingp t)
{
    if (t->weapon_tp_id) {
        return (id_to_tp(t->weapon_tp_id));
    }

    return (0);
}

/*
 * thing_move.c
 */
int thing_fall(levelp, thingp);
int thing_drown(levelp, thingp);
int thing_slide(levelp, thingp);
int thing_jump(levelp, thingp);
thingp things_throw(levelp level, thingp t);
void thing_move_set_dir(levelp,
                        thingp t,
                        double *x,
                        double *y,
                        uint8_t up,
                        uint8_t down,
                        uint8_t left,
                        uint8_t right);
int things_handle_impact(levelp,
                         const thingp A, 
                         double nx,
                         double ny,
                         const thingp B);

/*
 * thing.c
 */
uint8_t thing_player_move(levelp,
                          thingp t,
                          double x,
                          double y,
                          const uint8_t up,
                          const uint8_t down,
                          const uint8_t left,
                          const uint8_t right,
                          const uint8_t fire);
int thing_move(levelp,
               thingp t,
               double x,
               double y,
               const uint8_t up,
               const uint8_t down,
               const uint8_t left,
               const uint8_t right,
               const uint8_t fire);

void thing_move_to(thingp t, double x, double y);

void thing_wid_move(levelp level,
                    thingp t, 
                    double x, 
                    double y, 
                    uint8_t smooth);

/*
 * thing_effect.c
 */
void thing_effect(thingp t, int effect);

/*
 * thing_item.c
 */
void thing_auto_collect(levelp, thingp t, thingp it, tpp tp);
void thing_item_collect(levelp, thingp t, thingp it, tpp tp);
void thing_used(levelp, thingp t, tpp tp);
void thing_drop(levelp, thingp t, tpp tp);
int thing_wear_out(levelp, thingp t, tpp tp);
void thing_item_destroyed(levelp, thingp t, tpp tp);
void thing_wield_next_weapon(levelp, thingp t);

/*
 * thing_collision.c
 */
void 
thingp_get_interpolated_position(const thingp t, double *x, double *y);

int circle_box_collision(levelp,
                         thingp C, 
                         double bx,
                         double by,
                         thingp B,
                         double cx,
                         double cy,
                         fpoint *normal,
                         fpoint *intersect,
                         int check_only);

int circle_circle_collision(thingp A, 
                            thingp B,
                            double nx,
                            double ny,
                            fpoint *intersect);

/*
 * thing_place.c
 */
widp thing_place(levelp, thingp t, tpp tp);
widp thing_place_behind(levelp, thingp t, tpp tp);
widp thing_place_behind_or_under(levelp, thingp t, tpp tp);

/*
 * thing_dir.c
 */
void thing_dir(thingp t, double *dx, double *dy);
int thing_angle_to_dir(double dx, double dy);

/*
 * thing_weapon.c
 */
void thing_unwield(levelp, thingp t, const char *why);
void thing_sheath(levelp, thingp t);
void thing_swing(levelp, thingp t);
void thing_wield(levelp, thingp t, tpp tp);
void thing_weapon_sheath(levelp, thingp t);
void thing_weapon_swing_offset(levelp, thingp t, 
                               double *dx, double *dy);
thingp thing_weapon_carry_anim(levelp, thingp t);
thingp thing_weapon_swing_anim(levelp, thingp t);
void thing_set_weapon_placement(levelp, thingp t);
widp thing_get_weapon_carry_anim_wid(levelp, thingp t);
void thing_set_weapon_carry_anim_id(levelp, thingp t, 
                                    uint32_t weapon_carry_anim_id);
void thing_set_weapon_carry_anim(levelp, thingp t, 
                                 thingp weapon_carry_anim);
void thing_set_weapon_swing_anim_id(levelp, thingp t, 
                                    uint32_t weapon_swing_anim_id);
void thing_set_weapon_swing_anim(levelp, thingp t, 
                                 thingp weapon_swing_anim);

/*
 * thing_explosion.c
 */
extern int level_explosion_flash_effect;

void level_place_explosion_at(levelp level,
                              thingp owner,
                              double ox, 
                              double oy, 
                              double x, 
                              double y, 
                              uint8_t dist,
                              uint8_t is_epicenter,
                              const char *epicenter,
                              uint32_t nargs,
                              va_list args);
void level_place_explosion(levelp level, 
                           thingp owner,
                           tpp explodes_as,
                           double ox, double oy,
                           double x, double y);
void explosion_flood(levelp level, uint8_t x, uint8_t y);
void thing_explosion_placed(levelp level, thingp t);

/*
 * thing_blood.c
 */
void level_place_hit_success(levelp level, 
                         thingp owner,
                         double x, double y);
void level_place_hit_miss(levelp level, 
                         thingp owner,
                         double x, double y);
void level_place_blood(levelp level, 
                       thingp owner,
                       double x, double y);
void level_place_flames(levelp level, 
                       thingp owner,
                       double x, double y);
void level_place_blood_crit(levelp level, 
                            thingp owner,
                            double x, double y);

/*
 * thing_bubbles.c
 */
void level_place_bubbles(levelp level,
                         thingp owner,
                         double x, double y);
void thing_bubbles_placed(levelp level, thingp t);

/*
 * thing_bomb.c
 */
thingp thing_place_bomb(levelp level, 
                        thingp owner,
                        double x, double y);
void thing_collect_bomb(levelp level, 
                         thingp owner,
                         thingp it);
/*
 * thing_torch.c
 */
void thing_torch_update_count(levelp, thingp t, int force);

thingp thing_place_torch(levelp level, 
                         thingp owner,
                         double x, double y);
void thing_collect_torch(levelp level, 
                         thingp owner,
                         thingp it);

/*
 * thing_rope.c
 */
thingp thing_place_ropetop(levelp level, 
                           thingp owner,
                           double x, double y);
thingp thing_place_rope(levelp level, 
                        thingp owner,
                        double x, double y);
void thing_collect_rope(levelp level, 
                        thingp owner,
                        thingp it);
/*
 * thing_key.c
 */
void thing_use_key(levelp level, 
                   thingp owner,
                   double x, double y);
void thing_collect_key(levelp level, 
                        thingp owner,
                        thingp it);
/*
 * thing_collect.c
 */
void thing_collect(levelp level, 
                   thingp owner,
                   thingp it);

/*
 * thing_collision.c
 */
int thing_handle_collisions(levelp, thingp t);
void thing_to_coords(thingp t, fpoint *P0, fpoint *P1, fpoint *P2, fpoint *P3);

/*
 * thing_fire.c
 */
void thing_fire_at_target(levelp, thingp me);

void thing_fire(levelp,
                thingp t,
                const uint8_t up,
                const uint8_t down,
                const uint8_t left,
                const uint8_t right);
/*
 * thing_damage.c
 */
int32_t thing_stats_get_total_damage(thingp t);

/*
 * thing_speed.c
 */
double thing_stats_get_total_speed(thingp t);

/*
 * thing_damage.c
 */
int32_t thing_stats_get_total_damage_minus_defense(thingp t, 
                                                   thingp hitter, 
                                                   int32_t damage);

/*
 * thing_vision.c
 */
double thing_stats_get_total_vision(thingp t, double vision);
void level_place_light(levelp level, double x, double y);

/*
 * thing_health.c
 */
void thing_health_tick(levelp, thingp t);

/*
 * thing_score.c
 */
void thing_modify_score(thingp t, int val);

/*
 * thing_teleport.c
 */
void thing_reached_teleport(levelp, thingp t, thingp teleport);

/*
 * thing_exit.c
 */
void thing_reached_exit(levelp, thingp t);

/*
 * round the thing coords and find out what floor tile we are really on.
 * we use the bottom of the tile as that is where the 'feet' commonly are
 */
static inline void thing_real_to_map (thingp t, double *x, double *y)
{
    *x = (t->x + 0.0);
    *y = (t->y + 0.0);
}

static inline void thing_real_to_fmap (thingp t, double *x, double *y)
{
    *x = (t->x + 0.0);
    *y = (t->y + 0.0);
}

static inline void real_to_map (double ix, double iy, double *x, double *y)
{
    *x = (ix + 0.0);
    *y = (iy + 0.0);
}

/*
 * thing_effect.c
 */
void thing_effect_hit_miss(levelp, thingp t);
void thing_effect_hit_success(levelp, thingp t);
void thing_effect_flames(levelp, thingp t);
void thing_effect_bubbles(levelp, thingp t);
void thing_effect_hit_crit(levelp, thingp t);
void thing_effect_power_up(levelp, thingp t);

#define FOR_ALL_THINGS(level, t) \
    { \
    for (t = &level->things[0]; \
         t < &level->things[MAX_THINGS_PER_LEVEL]; t++) { \
        if (!t->thing_id) { continue; } \
        verify(t);

#define FOR_ALL_THINGS_END } }


static inline uint32_t thing_id (thingp t)
{
    verify(t);

    return (t->thing_id);
}

int thing_submerged_depth(levelp level, thingp t);
int thing_is_submerged(levelp, thingp t);
int thing_is_partially_or_fully_submerged(levelp, thingp t);

static inline int thing_is_stationary (thingp t)
{
    return (!thing_can_roll(t));
}

static inline fpoint thing_velocity (thingp t)
{
    fpoint v;

    v.x = t->momentum;                    
    v.y = t->fall_speed;

    return (v);
}

static void inline thing_set_velocity (thingp t, double x, double y)
{
    if (thing_is_stationary(t)) {
        return;
    }

    t->momentum = x;
    t->fall_speed = y;
}

#define THING_FALL_SPEED_CLING_ONTO_WALLS   0.2
#define THING_FALL_SPEED_TOO_FAST           0.45
#define THING_FALL_SPEED_GRAVITY            0.012
#define THING_FALL_SPEED_HIT_MONST          0.024
#define THING_FALL_SPEED_HIT_SPIKES         0.1
#define THING_FALL_SPEED_BOULDER_HURTS      0.05
#define THING_PUSH_SPEED_OBJ                0.1
