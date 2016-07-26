import traceback
import mm
import wid


class WidText(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 tiles,
                 text, 
                 font="small",
                 parent=0, 
                 **kw):
        self.parent = parent
        self.name = name

        super().__init__(name, tiles=tiles, parent=parent)
        self.set_tl_br_pct(x1, y1, x2, y2)

        self.log("Created wid")

        self.children = []
        self.width = x2 - x1
        self.height = y2 - y1

        self.tile_size = mm.tile_size_pct(tiles + "-tl")
        self.tile_width = self.tile_size[0]
        self.tile_height = self.tile_size[1]

        mm.con("{0}".format(self.tile_width))

        self.usable_width = self.width - self.tile_width
        self.usable_height = self.height - self.tile_height

        lines = text.split("\n")
        y = self.tile_height * 0.5

        for line in lines:

            words = line.split()
            x = self.tile_width * 0.5

            for word in words:
                w, h = mm.text_size_pct(font=font, text=word + " ")

                w = w / self.usable_width
                h = h / self.usable_height

                if x + w > 1.0 - self.tile_width * 0.5 :
                    x = self.tile_width * 0.5
                    y = y + h

                print("{0} ---{1}---".format(x, word))
                child = wid.Wid(name="wid text child", parent=self.wid_id)
                child.set_tl_br_pct(x, y, x + w, y + h)
                child.set_text(text=word, font=font)

                x = x + w

            y = y + h

        self.update()


