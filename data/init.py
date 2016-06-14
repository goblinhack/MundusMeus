import mm

def init_console ():

    mm.con("Welcome to the %%fg=yellow$Mundus Meus%%fg=reset$ console")
    mm.con(" ");
    mm.con("%%fg=red$          Welcome to the MundusMeus debug console!%%fg=reset$");
    mm.con(" ");
    mm.con("Press %%fg=red$<tab>%%fg=reset$ to complete commands.");
    mm.con("Press %%fg=red$?%%fg=reset$ to show command options.");
    mm.con("You can also enter raw python code here.");

def init_tex ():

    mm.tex_load(file="data/gfx/sword.tga", 
            name="sword");

    mm.tex_load(file="data/gfx/main_title.tga", 
            name="main_title");

    mm.tex_load_tiled(file="data/gfx/sprites_large.tga",
            name="sprites_large",
            width=64,
            height=64);

init_console()
init_tex()
