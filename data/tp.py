import mm;

class tp:
    """thing template"""
    def __init__(self, name):
        self.name = name;
        mm.tp_load(self);

    def set_short_name(self, value):
        self.short_name = value;
        mm.tp_set(self, "short_name", value);

x = tp("grass1");
x.set_short_name("bob");
