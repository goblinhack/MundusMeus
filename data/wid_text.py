import traceback
import mm
import wid


class WidText(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 text, 
                 font="small",
                 parent=0, 
                 **kw):
        self.parent = parent
        self.name = name

        super().__init__(name, **kw)
        self.set_tl_br_pct(x1, y1, x2, y2)

        self.log("Created wid")

        self.children = []
        self.width = x2 - x1
        self.height = y2 - y1

        lines = text.split("\n")
        y = 0

        for line in lines:

            words = line.split()
            x = 0

            for word in words:
                w, h = mm.text_size_pct(font=font, text=word + " ")

                w = w / self.width
                h = h / self.height

                mm.con("{0} {1} {2} {3} {4}".format(word, x, y, w, h))

                child = wid.Wid(name="wid text child", parent=self.wid_id)
                child.set_tl_br_pct(x, y, x + w, y + h)
                child.set_text(text=word, font=font)

                x = x + w

            y = y + h

        self.update()


