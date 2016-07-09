import mm;

def init_tex ():

    mm.tex_load(file="data/gfx/sword.tga", name="sword");
#    mm.tex_load(file="data/gfx/main_title.tga", name="main_title");
    mm.tex_load(file="data/gfx/title2.tga", name="main_title");

def init_tiles ():

    mm.tex_load_tiled(
            file="data/gfx/hex.tga", 
            name="hex", 
            width=64, 
            height=64);

    hex_tiles = [
        "grass1.1",
    ];

    mm.tile_load_arr(
            tex_name = "hex",
            tex_name_black_and_white = "",
            width = 64, height = 64,
            arr = hex_tiles);

init_tex();
init_tiles();
