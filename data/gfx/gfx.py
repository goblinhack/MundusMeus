import mm;

def init_tex ():

    mm.tex_load(file="data/gfx/sword.tga", 
            name="sword");

    mm.tex_load(file="data/gfx/main_title.tga", 
            name="main_title");

def init_tiles ():

    mm.tex_load_tiled(file="data/gfx/sprites_large.tga",
            name="sprites_large",
            width=64,
            height=64);

    sprites_large = [

        "grass1.1",
        "rock1-1-top1",
        "rock1-1-top2",
        "rock1-1-top3",
        "rock1-1-top4",
        "rock1-1-top5",
        "rock1-1-top6",
        "rock1-1-tr",
        "entrance1-1-1",
        "entrance1-1-2",
        "entrance1-1-3",
        "exit1-1-1",
        "exit1-1-2",
        "exit1-1-3",
        "",
        "",
        # --------------------------------------------------------------------
        "rock1-2-left",
        "rock1-2-mid1",
        "rock1-2-mid2",
        "rock1-2-mid3",
        "rock1-2-mid4",
        "rock1-2-mid5",
        "rock1-2-mid6",
        "rock1-2-right",
        "entrance1-2-1",
        "entrance1-2-2",
        "entrance1-2-3",
        "exit1-2-1",
        "exit1-2-2",
        "exit1-2-3",
        "",
        "",
        # --------------------------------------------------------------------
        "rock1-3-left",
        "rock1-3-mid1",
        "rock1-3-mid2",
        "rock1-3-mid3",
        "rock1-3-mid4",
        "rock1-3-mid5",
        "rock1-3-mid6",
        "rock1-3-right",
        "entrance1-3-1",
        "entrance1-3-2",
        "entrance1-3-3",
        "exit1-3-1",
        "exit1-3-2",
        "exit1-3-3",
        "",
        "",
        # --------------------------------------------------------------------
        "rock1-4-left",
        "rock1-4-mid1",
        "rock1-4-mid2",
        "rock1-4-mid3",
        "rock1-4-mid4",
        "rock1-4-mid5",
        "rock1-4-mid6",
        "rock1-4-right",
        "ladder1-top",
        "spikes1-1",
        "spikes2-1",
        "spikes3-1",
        "spikes4-1",
        "spikes5-1",
        "spikes6-1",
        "rope-top",
        # --------------------------------------------------------------------
        "rock1-5-left",
        "rock1-5-mid1",
        "rock1-5-mid2",
        "rock1-5-mid3",
        "rock1-5-mid4",
        "rock1-5-mid5",
        "rock1-5-mid6",
        "rock1-5-right",
        "ladder1-mid1",
        "spikes1-1-blood",
        "spikes2-1-blood",
        "spikes3-1-blood",
        "spikes4-1-blood",
        "spikes5-1-blood",
        "spikes6-1-blood",
        "rope1",
        # --------------------------------------------------------------------
        "rock1-6-left",
        "rock1-6-mid1",
        "rock1-6-mid2",
        "rock1-6-mid3",
        "rock1-6-mid4",
        "rock1-6-mid5",
        "rock1-6-mid6",
        "rock1-6-right",
        "ladder1-mid2",
        "door1-1",
        "door1-2",
        "door1-3",
        "door1-4",
        "door1-5",
        "door1-6",
        "rope2",
        # --------------------------------------------------------------------
        "rock1-7-left",
        "rock1-7-mid1",
        "rock1-7-mid2",
        "rock1-7-mid3",
        "rock1-7-mid4",
        "rock1-7-mid5",
        "rock1-7-mid6",
        "rock1-7-right",
        "ladder1-mid3",
        "pickaxe1",
        "crate1",
        "block1",
        "key1",
        "",
        "",
        "rope3",
        # --------------------------------------------------------------------
        "rock1-8-bl",
        "rock1-8-bot1",
        "rock1-8-bot2",
        "rock1-8-bot3",
        "rock1-8-bot4",
        "rock1-8-bot5",
        "rock1-8-bot6",
        "rock1-8-br",
        "ladder1-mid4",
        "cash1",
        "",
        "chest1",
        "",
        "",
        "ropepile",
        "rope4",
        # --------------------------------------------------------------------
        "torch-lit1-1",
        "torch-lit1-2",
        "torch-lit1-3",
        "torch-lit1-4",
        "torch-lit1-5",
        "torch-lit1-6",
        "torch-lit1-7",
        "torch-lit1-8",
        "torch-lit1-9",
        "torch-lit1-10",
        "torch-lit1-11",
        "torch-lit1-12",
        "torch-lit1-13",
        "torch-lit1-14",
        "torch-lit1-15",
        "torch-lit1-16",
        # --------------------------------------------------------------------
        "torch-lit2-1",
        "torch-lit2-2",
        "torch-lit2-3",
        "torch-lit2-4",
        "torch-lit2-5",
        "torch-lit2-6",
        "torch-lit2-7",
        "torch-lit2-8",
        "torch-lit2-9",
        "torch-lit2-10",
        "torch-lit2-11",
        "torch-lit2-12",
        "torch-lit2-13",
        "torch-lit2-14",
        "torch-lit2-15",
        "torch-lit2-16",
        # --------------------------------------------------------------------
        "",
        "large-collision-map",
        "medium-collision-map",
        "small-collision-map",
        "tiny-collision-map",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "cobweb1-1-1",
        "cobweb1-1-2",
        "cobweb1-1-3",
        "chocolatefrog",
        # --------------------------------------------------------------------
        "gold1.1",
        "gem1.1",
        "gold1.2",
        "gem1.2",
        "gold1.3",
        "gem1.3",
        "",
        "",
        "",
        "",
        "",
        "",
        "cobweb1-2-1",
        "cobweb1-2-2",
        "cobweb1-2-3",
        "gun1",
        # --------------------------------------------------------------------
        "gold2.1",
        "gem2.1",
        "gold2.2",
        "gem2.2",
        "gold2.3",
        "gem2.3",
        "",
        "",
        "",
        "",
        "",
        "",
        "cobweb1-3-1",
        "cobweb1-3-2",
        "cobweb1-3-3",
        "gun2",
        # --------------------------------------------------------------------
        "gold3.1",
        "gem3.1",
        "gold3.2",
        "gem3.2",
        "gold3.3",
        "gem3.3",
        "bridge1-1-1",
        "bridge1-1-2",
        "bridge1-1-3",
        "damage1-7",
        "damage1-6",
        "damage1-5",
        "damage1-4",
        "damage1-3",
        "damage1-2",
        "damage1-1",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "bridge1-2-1",
        "bridge1-2-2",
        "bridge1-2-3",
        "blood1-7",
        "blood1-6",
        "blood1-5",
        "blood1-4",
        "blood1-3",
        "blood1-2",
        "blood1-1",
        # --------------------------------------------------------------------
        "smallrock1",
        "smallrock2",
        "smallrock3",
        "smallrock4",
        "smallrock5",
        "smallrock6",
        "",
        "bridge1-3-2",
        "bridge1-3-3",
        "icon-torches",
        "icon-ropes",
        "icon-hp",
        "icon-keys",
        "icon-bombs",
        "icon-skull",
        "icon-drown",
        # --------------------------------------------------------------------
        "player1-1",
        "player1-lr-1",
        "player1-lr-2",
        "player1-lr-3",
        "player1-lr-4",
        "player1-lr-5",
        "player1-lr-6",
        "player1-lr-7",
        "player1-lr-8",
        "player1-up-1",
        "player1-up-2",
        "player1-up-3",
        "player1-up-4",
        "player1-up-5",
        "player1-up-6",
        "player1-dead",
        # --------------------------------------------------------------------
        "spider1-1-1",
        "spider1-1-2",
        "spider1-1-3",
        "spider1-2-1",
        "spider1-2-2",
        "spider1-2-3",
        "spider1-3-1",
        "spider1-3-2",
        "spider1-3-3",
        "spider1-4-1",
        "spider1-4-2",
        "spider1-4-3",
        "spider1-5-1",
        "spider1-5-2",
        "spider1-5-3",
        "",
        # --------------------------------------------------------------------
        "spider1-6-1",
        "spider1-6-2",
        "spider1-6-3",
        "spider1-7-1",
        "spider1-7-2",
        "spider1-7-3",
        "spider1-8-1",
        "spider1-8-2",
        "spider1-8-3",
        "spider1-9-1",
        "spider1-9-2",
        "spider1-9-3",
        "spider1-10-1",
        "spider1-10-2",
        "spider1-10-3",
        "",
        # --------------------------------------------------------------------
        "snake1-1",
        "snake1-2",
        "snake1-3",
        "snake1-4",
        "snake1-5",
        "snake1-6",
        "snake1-7",
        "snake1-8",
        "snake1-9",
        "snake1-10",
        "snake1-11",
        "snake1-12",
        "snake1-13",
        "snake1-14",
        "snake1-15",
        "snake1-16",
        # --------------------------------------------------------------------
        "bat1-1-1",
        "bat1-1-2",
        "bat1-1-3",
        "bat1-2-1",
        "bat1-2-2",
        "bat1-2-3",
        "bat1-3-1",
        "bat1-3-2",
        "bat1-3-3",
        "bat1-4-1",
        "bat1-4-2",
        "bat1-4-3",
        "bat1-5-1",
        "bat1-5-2",
        "bat1-5-3",
        "",
        # --------------------------------------------------------------------
        "ghost1-1",
        "ghost1-2",
        "ghost1-3",
        "ghost1-4",
        "ghost1-5",
        "ghost1-6",
        "ghost1-7",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "orc1-1",
        "orc1-lr-1",
        "orc1-lr-2",
        "orc1-lr-3",
        "orc1-lr-4",
        "orc1-lr-5",
        "orc1-lr-6",
        "orc1-lr-7",
        "orc1-lr-8",
        "orc1-up-1",
        "orc1-up-2",
        "orc1-up-3",
        "orc1-up-4",
        "orc1-up-5",
        "orc1-up-6",
        "",
        # --------------------------------------------------------------------
        "shopkeeper1-1",
        "shopkeeper1-lr-1",
        "shopkeeper1-lr-2",
        "shopkeeper1-lr-3",
        "shopkeeper1-lr-4",
        "shopkeeper1-lr-5",
        "shopkeeper1-lr-6",
        "shopkeeper1-lr-7",
        "shopkeeper1-lr-8",
        "shopkeeper1-up-1",
        "shopkeeper1-up-2",
        "shopkeeper1-up-3",
        "shopkeeper1-up-4",
        "shopkeeper1-up-5",
        "shopkeeper1-up-6",
        "",
        # --------------------------------------------------------------------
        "demon1-1",
        "demon1-lr-1",
        "demon1-lr-2",
        "demon1-lr-3",
        "demon1-lr-4",
        "demon1-lr-5",
        "demon1-lr-6",
        "demon1-lr-7",
        "demon1-lr-8",
        "demon1-up-1",
        "demon1-up-2",
        "demon1-up-3",
        "demon1-up-4",
        "demon1-up-5",
        "demon1-up-6",
        "",
        # --------------------------------------------------------------------
        "skull1-1-1",
        "skull1-1-2",
        "skull1-1-3",
        "skull1-2-1",
        "skull1-2-2",
        "skull1-2-3",
        "skull1-3-1",
        "skull1-3-2",
        "skull1-3-3",
        "skull1-4-1",
        "skull1-4-2",
        "skull1-4-3",
        "skull1-5-1",
        "skull1-5-2",
        "skull1-5-3",
        "",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "weapon_swing1.12",
        "weapon_swing1.11",
        "weapon_swing1.10",
        "weapon_swing1.9",
        "weapon_swing1.8",
        "weapon_swing1.7",
        "weapon_swing1.6",
        "weapon_swing1.5",
        "weapon_swing1.4",
        "weapon_swing1.3",
        "weapon_swing1.2",
        "weapon_swing1.1",
        "sword1",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "weapon_swing2.1",
        "weapon_swing2.2",
        "weapon_swing2.3",
        "weapon_swing2.4",
        "weapon_swing2.5",
        "weapon_swing2.6",
        "weapon_swing2.7",
        "weapon_swing2.8",
        "weapon_swing2.9",
        "weapon_swing2.10",
        "weapon_swing2.11",
        "weapon_swing2.12",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "weapon_swing3.1",
        "weapon_swing3.2",
        "weapon_swing3.3",
        "weapon_swing3.4",
        "weapon_swing3.5",
        "weapon_swing3.6",
        "weapon_swing3.7",
        "weapon_swing3.8",
        "weapon_swing3.9",
        "weapon_swing3.10",
        "weapon_swing3.11",
        "weapon_swing3.12",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "weapon_swing4.12",
        "weapon_swing4.11",
        "weapon_swing4.10",
        "weapon_swing4.9",
        "weapon_swing4.8",
        "weapon_swing4.7",
        "weapon_swing4.6",
        "weapon_swing4.5",
        "weapon_swing4.4",
        "weapon_swing4.3",
        "weapon_swing4.2",
        "weapon_swing4.1",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "dynamite1.1",
        "dynamite1.3",
        "dynamite1.5",
        "dynamite1.7",
        "dynamite1.9",
        "dynamite1.11",
        "dynamite1.13",
        "dynamite1.15",
        "dynamite1.17",
        "dynamite1.19",
        "dynamite1.21",
        "dynamite1.23",
        "bubbles-1",
        "bubbles-2",
        "bubbles-3",
        "bubbles-4",
        # --------------------------------------------------------------------
        "dynamite1.2",
        "dynamite1.4",
        "dynamite1.6",
        "dynamite1.8",
        "dynamite1.10",
        "dynamite1.12",
        "dynamite1.14",
        "dynamite1.16",
        "dynamite1.18",
        "dynamite1.20",
        "dynamite1.22",
        "dynamite1.24",
        "bubbles-5",
        "bubbles-6",
        "bubbles-7",
        "bubbles-8",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        # --------------------------------------------------------------------
        "water1",
        "water2",
        "water3",
        "water4",
        "water5",
        "water6",
        "water7",
        "water8",
        "water9",
        "water10",
        "water11",
        "water12",
        "water13",
        "water14",
        "water15",
        "water16",
        "water17",
        "water18",
        "water19",
        "water20",
        "water21",
        "water22",
        "water23",
        "water24",
        "water25",
        "water26",
        "water27",
        "water28",
        "water29",
        "water30",
        "water31",
        "water32",
        "water33",
        "water34",
        "water35",
        "water36",
        "water37",
        "water38",
        "water39",
        "water40",
        "water41",
        "water42",
        "water43",
        "water44",
        "water45",
        "water46",
        "water47",
        "water48",
        "water49",
        "water50",
        "water51",
        "water52",
        "water53",
        "water54",
        "water55",
        "water56",
        "water57",
        "water58",
        "water59",
        "water60",
        "water61",
        "water62",
        "water63",
        "water64",
        # --------------------------------------------------------------------
        "boulder-lava-1-1-1",
        "boulder-lava-1-1-2",
        "boulder-lava-1-1-3",
        "boulder-large-1-1-1",
        "boulder-large-1-1-2",
        "boulder-large-1-1-3",
        "spike-1-1",
        "spike-1-2",
        "spike-1-3",
        "boulder1-1",
        "boulder1-2",
        "boulder2-1",
        "boulder2-2",
        "boulder3-1",
        "boulder3-2",
        "boulder3-3",
        # --------------------------------------------------------------------
        "boulder-lava-1-2-1",
        "boulder-lava-1-2-2",
        "boulder-lava-1-2-3",
        "boulder-large-1-2-1",
        "boulder-large-1-2-2",
        "boulder-large-1-2-3",
        "spike-2-1",
        "spike-2-2",
        "spike-2-3",
        "boulder1-3",
        "boulder1-4",
        "boulder2-3",
        "boulder2-4",
        "boulder3-4",
        "boulder3-5",
        "boulder3-6",
        # --------------------------------------------------------------------
        "boulder-lava-1-3-1",
        "boulder-lava-1-3-2",
        "boulder-lava-1-3-3",
        "boulder-large-1-3-1",
        "boulder-large-1-3-2",
        "boulder-large-1-3-3",
        "spike-3-1",
        "spike-3-2",
        "spike-3-3",
        "boulder1-5",
        "boulder1-6",
        "boulder2-5",
        "boulder2-6",
        "boulder3-7",
        "boulder3-8",
        "boulder3-9",
        # --------------------------------------------------------------------
    ];

    mm.tile_load_arr(
            tex_name = "sprites_large",
            tex_name_black_and_white = "sprites_large",
            width = 64, height = 64,
            arr = sprites_large);

init_tex();
init_tiles();
