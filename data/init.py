import mm
import os
import fnmatch
from os.path import dirname, basename
import imp

def init_console ():

    mm.con("Welcome to the %%fg=yellow$Mundus Meus%%fg=reset$ console")
    mm.con(" ");
    mm.con("%%fg=red$          Welcome to the MundusMeus debug console!%%fg=reset$");
    mm.con(" ");
    mm.con("Press %%fg=red$<tab>%%fg=reset$ to complete commands.");
    mm.con("Press %%fg=red$?%%fg=reset$ to show command options.");
    mm.con("You can also enter raw python code here.");

def find_plugins(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

def load_plugin(filepath):
    if basename(filepath) == "init.py":
        return

    if basename(filepath) == "tp.py":
        return

    mod_name,file_ext = os.path.splitext(os.path.split(filepath)[-1])

    if file_ext.lower() == '.py':
        py_mod = imp.load_source(mod_name, filepath)

    elif file_ext.lower() == '.pyc':
        py_mod = imp.load_compiled(mod_name, filepath)

def init_plugins():
    for filename in find_plugins(dirname(__file__), '*.py'):
        mm.con("Loading " + filename);
        load_plugin(filename);

def main():
    init_console()
    init_plugins()

main();
