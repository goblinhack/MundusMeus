import mm


def init_tex():

    mm.tex_load(file="data/gfx/wid1.tga",       name="wid1")
    mm.tex_load(file="data/gfx/sword.tga",      name="sword")
    mm.tex_load(file="data/gfx/title.tga",      name="main_title")
    mm.tex_load(file="data/gfx/map.tga",        name="map")
    mm.tex_load(file="data/gfx/bigmap.tga",     name="bigmap")


def init_tiles_wid1():

    mm.tex_load_tiled(
            file="data/gfx/wid1.tga",
            name="chars",
            width=64, height=64)

    tiles = [
        "wid1-tl",
        "wid1-top1",
        "wid1-top2",
        "wid1-top3",
        "wid1-top4",
        "wid1-top5",
        "wid1-top6",
        "wid1-tr",
        "wid1-left1",
        "wid1-1",
        "wid1-2",
        "wid1-3",
        "wid1-4",
        "wid1-5",
        "wid1-6",
        "wid1-right1",
        "wid1-left2",
        "wid1-7",
        "wid1-8",
        "wid1-9",
        "wid1-10",
        "wid1-11",
        "wid1-12",
        "wid1-right2",
        "wid1-left3",
        "wid1-13",
        "wid1-14",
        "wid1-15",
        "wid1-16",
        "wid1-17",
        "wid1-18",
        "wid1-right3",
        "wid1-left4",
        "wid1-19",
        "wid1-20",
        "wid1-21",
        "wid1-22",
        "wid1-23",
        "wid1-24",
        "wid1-right4",
        "wid1-left5",
        "wid1-25",
        "wid1-26",
        "wid1-27",
        "wid1-28",
        "wid1-29",
        "wid1-30",
        "wid1-right5",
        "wid1-left6",
        "wid1-31",
        "wid1-32",
        "wid1-33",
        "wid1-34",
        "wid1-35",
        "wid1-36",
        "wid1-right6",
        "wid1-bl",
        "wid1-bot1",
        "wid1-bot2",
        "wid1-bot3",
        "wid1-bot4",
        "wid1-bot5",
        "wid1-bot6",
        "wid1-br",
    ]

    mm.tile_load_arr(
            tex_name="wid1",
            tex_name_black_and_white="",
            width=64, height=64,
            arr=tiles)


def init_tiles_bigmap():

    mm.tex_load_tiled(
            file="data/gfx/bigmap.tga",
            name="hex",
            width=2048,
            height=2048)

    tiles = [
        "bigmap",
    ]

    mm.tile_load_arr(tex_name="bigmap",
                     tex_name_black_and_white="",
                     width=2048, height=2048,
                     arr=tiles)


def init_tiles_chars():

    mm.tex_load_tiled(
            file="data/gfx/chars.tga",
            name="chars",
            width=16,
            height=16)

    tiles = [
        "player1",
        "player2",
        "player3",
        "player4",
        "player5",
        "player6",
        "player7",
        "player8",
        "player9",
        "player10",
        "player11",
        "player12",
        "player13",
        "player14",
        "player15",
        "player16",
        "player17",
        "player18",
        "player19",
        "player20",
        "player21",
        "player22",
        "player23",
        "player24",
        "player25",
        "player26",
        "player27",
        "player28",
        "player29",
        "player30",
        "player31",
        "player32",
        "player33",
        "player34",
        "player35",
        "player36",
        "player37",
        "player38",
        "player39",
        "player40",
        "player41",
        "player42",
        "player43",
        "player44",
        "player45",
        "player46",
        "player47",
        "player48",
        "player49",
        "player50",
        "player51",
        "player52",
        "player53",
        "player54",
        "player55",
        "player56",
        "player57",
        "player58",
        "player59",
        "player60",
        "player61",
        "player62",
        "player63",
        "player64",
    ]

    mm.tile_load_arr(
            tex_name="chars",
            tex_name_black_and_white="",
            width=16, height=16,
            arr=tiles)


def init_tiles_sea():

    mm.tex_load_tiled(
            file="data/gfx/sea.tga",
            name="sea",
            width=256,
            height=256)

    tiles = [
        "sea1.1",
        "sea1.2",
        "sea1.3",
        "sea1.4",
        "sea1.5",
        "sea1.6",
        "sea1.7",
        "sea1.8",
        "sea1.9",
        "sea1.10",
        "sea1.11",
        "sea1.12",
        "sea1.13",
        "sea1.14",
        "sea1.15",
        "sea1.16",
        "sea1.17",
        "sea1.18",
        "sea1.19",
        "sea1.20",
        "sea1.21",
        "sea1.22",
        "sea1.23",
        "sea1.24",
        "sea1.25",
        "sea1.26",
        "sea1.27",
        "sea1.28",
        "sea1.29",
        "sea1.30",
        "sea1.31",
        "sea1.32",
        "sea1.33",
        "sea1.34",
        "sea1.35",
        "sea1.36",
        "sea1.37",
        "sea1.38",
        "sea1.39",
        "sea1.40",
        "sea1.41",
        "sea1.42",
        "sea1.43",
        "sea1.44",
        "sea1.45",
        "sea1.46",
        "sea1.47",
        "sea1.48",
        "sea1.49",
        "sea1.50",
        "sea1.51",
        "sea1.52",
        "sea1.53",
        "sea1.54",
        "sea1.55",
        "sea1.56",
        "sea1.57",
        "sea1.58",
        "sea1.59",
        "sea1.60",
        "sea1.61",
        "sea1.62",
        "sea1.63",
        "sea1.64",
    ]

    mm.tile_load_arr(
            tex_name="sea",
            tex_name_black_and_white="",
            width=256, height=256,
            arr=tiles)

init_tex()
init_tiles_wid1()
init_tiles_bigmap()
init_tiles_sea()
init_tiles_chars()
