/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info for license.
 */

#include <SDL.h>

#include "main.h"
#include "resource.h"
#include "tex.h"
#include "tile.h"
#include "init_fn.h"
#include "thing.h"
#include "sound.h"

static uint8_t resource_init_1 (void *context)
{
    return (true);
}

static uint8_t resource_init_1a (void *context)
{
    return (true);
}

static uint8_t resource_init_2 (void *context)
{
    return (true);
}

static uint8_t resource_init_3 (void *context)
{
    return (true);
}

static uint8_t resource_init_4 (void *context)
{
    return (true);
}

static uint8_t resource_init_5 (void *context)
{
    return (true);
}

static uint8_t resource_init_6 (void *context)
{
    return (true);
}

static uint8_t resource_init_7 (void *context)
{
    return (true);
}

static uint8_t resource_init_8 (void *context)
{
    return (true);
}

static uint8_t resource_init_9 (void *context)
{
    return (true);
}

static uint8_t resource_init_10 (void *context)
{
    return (true);
}

static uint8_t resource_init_11 (void *context)
{
    return (true);
}

static uint8_t resource_init_12 (void *context)
{
    return (true);
}

static uint8_t resource_init_13 (void *context)
{
    return (true);
}

static uint8_t resource_init_14 (void *context)
{
    return (true);
}

static uint8_t resource_init_15 (void *context)
{
    return (true);
}

static uint8_t resource_init_16 (void *context)
{
    return (true);
}

static uint8_t resource_init_17 (void *context)
{
    return (true);
}

static uint8_t resource_init_18 (void *context)
{
    return (true);
}

static uint8_t resource_init_19 (void *context)
{
    return (true);
}

static uint8_t resource_init_20 (void *context)
{
    return (true);
}

static uint8_t resource_init_21 (void *context)
{
    return (true);
}

static uint8_t resource_init_22 (void *context)
{
    return (true);
}

static uint8_t resource_init_23 (void *context)
{
    return (true);
}

static uint8_t resource_init_24 (void *context)
{
    return (true);
}

static uint8_t resource_init_25 (void *context)
{
    return (true);
}

static uint8_t resource_init_26 (void *context)
{
    return (true);
}

static uint8_t resource_init_27 (void *context)
{
    return (true);
}

static uint8_t resource_init_28 (void *context)
{
    return (true);
}

static uint8_t resource_init_29 (void *context)
{
    return (true);
}

static uint8_t resource_init_30 (void *context)
{
    return (true);
}

static uint8_t resource_init_31 (void *context)
{
    return (true);
}

static uint8_t resource_init_32 (void *context)
{
    return (true);
}

static uint8_t resource_init_33 (void *context)
{
    return (true);
}

static uint8_t resource_init_34 (void *context)
{
    return (true);
}

static uint8_t resource_init_35 (void *context)
{
    return (true);
}

static uint8_t resource_init_36 (void *context)
{
    return (true);
}

static uint8_t resource_init_37 (void *context)
{
    return (true);
}

static uint8_t resource_init_38 (void *context)
{
    return (true);
}

static uint8_t resource_init_39 (void *context)
{
    return (true);
}

static uint8_t resource_init_40 (void *context)
{
    return (true);
}

static uint8_t resource_init_41 (void *context)
{
    return (true);
}

static uint8_t resource_init_last (void *context)
{
    tp_load(THING_GRASS1, "grass1");

    return (true);
}

uint8_t resource_init (void)
{
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_1,
                          0, "resource_init_1");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_1a,
                          0, "resource_init_1a");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_2,
                          0, "resource_init_2");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_3,
                          0, "resource_init_3");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_4,
                          0, "resource_init_4");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_5,
                          0, "resource_init_5");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_6,
                          0, "resource_init_6");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_7,
                          0, "resource_init_7");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_8,
                          0, "resource_init_8");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_9,
                          0, "resource_init_9");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_10,
                          0, "resource_init_10");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_11,
                          0, "resource_init_11");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_12,
                          0, "resource_init_12");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_13,
                          0, "resource_init_13");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_14,
                          0, "resource_init_14");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_15,
                          0, "resource_init_15");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_16,
                          0, "resource_init_16");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_17,
                          0, "resource_init_17");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_18,
                          0, "resource_init_18");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_19,
                          0, "resource_init_19");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_20,
                          0, "resource_init_20");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_21,
                          0, "resource_init_21");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_22,
                          0, "resource_init_22");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_23,
                          0, "resource_init_23");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_24,
                          0, "resource_init_24");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_25,
                          0, "resource_init_25");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_26,
                          0, "resource_init_26");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_27,
                          0, "resource_init_27");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_28,
                          0, "resource_init_28");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_29,
                          0, "resource_init_29");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_30,
                          0, "resource_init_30");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_31,
                          0, "resource_init_31");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_32,
                          0, "resource_init_32");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_33,
                          0, "resource_init_33");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_34,
                          0, "resource_init_34");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_35,
                          0, "resource_init_35");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_36,
                          0, "resource_init_36");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_37,
                          0, "resource_init_37");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_38,
                          0, "resource_init_38");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_39,
                          0, "resource_init_39");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_40,
                          0, "resource_init_40");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_41,
                          0, "resource_init_41");
    action_init_fn_create(&init_fns,
                          (action_init_fn_callback)resource_init_last,
                          0, "resource_init_last");

    return (true);
}
