import mm;

def init_tex ():

    mm.tex_load(file="data/gfx/sword.tga", name="sword");
    mm.tex_load(file="data/gfx/main_title.tga", name="main_title");

def init_tiles ():

    mm.tex_load_tiled(file="data/gfx/sprites_large.tga", name="sprites_large", width=64, height=64);

    sprites_large = [
        "grass1.1",
        "grass1.2",
        "grass1.3",
        "grass1.4",
        "grass1.5",
        "grass1.6",
        "forest1.1",
        "forest1.2",
        "forest1.3",
        "forest1.4",
        "forest1.5",
        "forest1.6",
        "swamp1.1",
        "swamp1.2",
        "swamp1.3",
        "swamp1.4",
        #
        "grass2.1",
        "grass2.2",
        "grass2.3",
        "grass2.4",
        "grass2.5",
        "grass2.6",
        "forest2.1",
        "forest2.2",
        "forest2.3",
        "forest2.4",
        "forest2.5",
        "forest2.6",
        "swamp2.1",
        "swamp2.2",
        "swamp2.3",
        "swamp2.4",
        #
        "grass3.1",
        "grass3.2",
        "grass3.3",
        "grass3.4",
        "grass3.5",
        "grass3.6",
        "forest3.1",
        "forest3.2",
        "forest3.3",
        "forest3.4",
        "forest3.5",
        "forest3.6",
        "swamp3.1",
        "swamp3.2",
        "swamp3.3",
        "swamp3.4",
        #
        "grass4.1",
        "grass4.2",
        "grass4.3",
        "grass4.4",
        "grass4.5",
        "grass4.6",
        "forest4.1",
        "forest4.2",
        "forest4.3",
        "forest4.4",
        "forest4.5",
        "forest4.6",
        "swamp4.1",
        "swamp4.2",
        "swamp4.3",
        "swamp4.4",
        #
        "grass5.1",
        "grass5.2",
        "grass5.3",
        "grass5.4",
        "grass5.5",
        "grass5.6",
        "forest5.1",
        "forest5.2",
        "forest5.3",
        "forest5.4",
        "forest5.5",
        "forest5.6",
        "swamp5.1",
        "swamp5.2",
        "swamp5.3",
        "swamp5.4",
        #
        "sand1.1",
        "sand1.2",
        "sand1.3",
        "sand1.4",
        "sand1.5",
        "sand1.6",
        "sand1.7",
        "rock1.1",
        "rock1.2",
        "rock1.3",
        "rock1.4",
        "mountain1.1",
        "mountain1.2",
        "mountain1.3",
        "mountain1.4",
        "mountain1.5",
        #
        "sand2.1",
        "sand2.2",
        "sand2.3",
        "sand2.4",
        "sand2.5",
        "sand2.6",
        "sand2.7",
        "rock2.1",
        "rock2.2",
        "rock2.3",
        "rock2.4",
        "mountain2.1",
        "mountain2.2",
        "mountain2.3",
        "mountain2.4",
        "mountain2.5",
        #
        "northern_settlement1.1",
        "northern_settlement1.2",
        "northern_settlement1.3",
        "northern_settlement1.4",
        "northern_settlement1.5",
        "northern_settlement1.6",
        "northern_settlement1.7",
        "rock3.1",
        "rock3.2",
        "rock3.3",
        "rock3.4",
        "mountain3.1",
        "mountain3.2",
        "mountain3.3",
        "mountain3.4",
        "mountain3.5",
        #
        "northern_settlement2.1",
        "northern_settlement2.2",
        "northern_settlement2.3",
        "northern_settlement2.4",
        "northern_settlement2.5",
        "northern_settlement2.6",
        "northern_settlement2.7",
        "northern_rock1.1",
        "northern_rock1.2",
        "northern_rock1.3",
        "northern_rock1.4",
        "northern_mountain1.1",
        "northern_mountain1.2",
        "northern_mountain1.3",
        "northern_mountain1.4",
        "northern_mountain1.5",
        #
        "northern_settlement3.1",
        "northern_settlement3.2",
        "northern_settlement3.3",
        "northern_settlement3.4",
        "northern_settlement3.5",
        "northern_settlement3.6",
        "northern_settlement3.7",
        "northern_rock2.1",
        "northern_rock2.2",
        "northern_rock2.3",
        "northern_rock2.4",
        "northern_mountain2.1",
        "northern_mountain2.2",
        "northern_mountain2.3",
        "northern_mountain2.4",
        "northern_mountain2.5",
        #
        "settlement1.1",
        "settlement1.2",
        "settlement1.3",
        "settlement1.4",
        "settlement1.5",
        "settlement1.6",
        "settlement1.7",
        "settlement1.8",
        "settlement1.9",
        "castle1.1",
        "castle1.2",
        "castle1.3",
        "castle1.4",
        "castle1.5",
        "castle1.6",
        "castle1.7",
        #
        "settlement2.1",
        "settlement2.2",
        "settlement2.3",
        "settlement2.4",
        "settlement2.5",
        "settlement2.6",
        "settlement2.7",
        "settlement2.8",
        "settlement2.9",
        "castle2.1",
        "castle2.2",
        "castle2.3",
        "castle2.4",
        "castle2.5",
        "castle2.6",
        "castle2.7",
        #
        "snow_settlement1.1",
        "snow_settlement1.2",
        "snow_settlement1.3",
        "snow_settlement1.4",
        "snow_settlement1.5",
        "snow_castle1.1",
        "snow1.1",
        "snow1.2",
        "snow1.3",
        "snow1.4",
        "snow1.5",
        "snow1.6",
        "snow1.7",
        "snow1.8",
        "snow1.9",
        "snow1.10",
        #
        "snow_settlement2.1",
        "snow_settlement2.2",
        "snow_settlement2.3",
        "snow_settlement2.4",
        "snow_settlement2.5",
        "snow_castle2.1",
        "snow2.1",
        "snow2.2",
        "snow2.3",
        "snow2.4",
        "snow2.5",
        "snow2.6",
        "snow2.7",
        "snow2.8",
        "snow2.9",
        "snow2.10",
        #
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
        #
        "sea2.1",
        "sea2.2",
        "sea2.3",
        "sea2.4",
        "sea2.5",
        "sea2.6",
        "sea2.7",
        "sea2.8",
        "sea2.9",
        "sea2.10",
        "sea2.11",
        "sea2.12",
        "sea2.13",
        "sea2.14",
        "sea2.15",
        "sea2.16",
        #
        "sea3.1",
        "sea3.2",
        "sea3.3",
        "sea3.4",
        "sea3.5",
        "sea3.6",
        "sea3.7",
        "sea3.8",
        "sea3.9",
        "sea3.10",
        "sea3.11",
        "sea3.12",
        "sea3.13",
        "sea3.14",
        "sea3.15",
        "sea3.16",
        #
        "sea4.1",
        "sea4.2",
        "sea4.3",
        "sea4.4",
        "sea4.5",
        "sea4.6",
        "sea4.7",
        "sea4.8",
        "sea4.9",
        "sea4.10",
        "sea4.11",
        "sea4.12",
        "sea4.13",
        "sea4.14",
        "sea4.15",
        "sea4.16",
    ];

    mm.tile_load_arr(
            tex_name = "sprites_large",
            tex_name_black_and_white = "",
            width = 64, height = 64,
            arr = sprites_large);

init_tex();
init_tiles();
