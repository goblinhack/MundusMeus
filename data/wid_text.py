import traceback
import mm
import wid


class WidText(wid.Wid):

    def __init__(self, name, 
                 x1, y1, x2, y2, 
                 row_text, 
                 row_font, 
                 row_color, 
                 row_center, 
                 row_rhs, 
                 row_width, 
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

        y = 0
        print(row_width)
        for row in range(0, len(row_text)):
            text = row_text[row]
            font = row_font[row]
            color = row_color[row]
            width = row_width[row]
            center = row_center[row]
            rhs = row_rhs[row]

            lines = text.split("\n")

            for line in lines:

                words = line.split()
                x = 0

                if center is True:
                    x = self.usable_w - width
                    x = x / self.usable_w
                    x /= 2.0

                if rhs is True:
                    x = self.usable_w - width
                    x = x / self.usable_w

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

                    self.children.append(child)

                    x = x + w

                y = y + h

        self.update()


def text_size_pct(row_text, row_font):

    row_width = []

    y = 0
    for row in range(0, len(row_text)):
        text = row_text[row]
        font = row_font[row]
        row_width.append(0)

        lines = text.split("\n")
        max_w = 0

        for line in lines:

            words = line.split()
            x = 0
            max_h = 0

            for word in words:
                w, h, c = mm.text_size_pct(font=font, text=word + " ")

                if x + w > 1.0:
                    x = 0
                    y = y + h
                    max_w = 1.0

                x = x + w

                if x > max_w:
                    max_w = x

                if h > max_h:
                    max_h = h

            if words != []:
                y = y + max_h

            row_width[row] = max_w

    return max_w, y, row_width
