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

        super().__init__(name, parent=parent)
        self.set_tl_br_pct(x1, y1, x2, y2)

        self.children = []
        self.width, self.height = self.get_size_pct()

        self.usable_w = self.width
        self.usable_h = self.height
        self.update()

        lines = text.split("\n")
        y = 0

        color = None

        for line in lines:

            words = line.split()
            x = 0

            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word + " ")
                if c != "none":
                    color = c

                w = w / self.usable_w
                h = h / self.usable_h

                if x + w > 1.0:
                    x = 0
                    y = y + h

                child = wid.Wid(name="wid text child", parent=self.wid_id)
                child.set_tl_br_pct(x, y, x + w, y + h)

                if color != None:
                    child.set_text(text=word, lhs=True, font=font, color=color)
                else:
                    child.set_text(text=word, lhs=True, font=font)

                x = x + w

            y = y + h

        self.update()


class WidTextSize(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 text, 
                 font="small",
                 parent=0, 
                 **kw):
        self.parent = parent
        self.name = name

        super().__init__(name, parent=parent)
        self.set_tl_br_pct(x1, y1, x2, y2)

        self.children = []
        self.width, self.height = self.get_size_pct()

        self.usable_w = self.width
        self.usable_h = self.height

        lines = text.split("\n")
        y = 0

        color = None

        for line in lines:

            words = line.split()
            x = 0

            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word + " ")
                if c != "none":
                    color = c

                w = w / self.usable_w
                h = h / self.usable_h

                if x + w > 1.0:
                    x = 0
                    y = y + h

                child = wid.Wid(name="wid text child", parent=self.wid_id)
                child.set_tl_br_pct(x, y, x + w, y + h)

                if color != None:
                    child.set_text(text=word, lhs=True, font=font, color=color)
                else:
                    child.set_text(text=word, lhs=True, font=font)

                x = x + w

            y = y + h

        self.update()

