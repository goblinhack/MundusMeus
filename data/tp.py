import mm;

class tp:
    """thing template"""
    def name(self):
        return self.name

x = tp();
x.name = "grass1";
mm.tp_load(x);
