import mm
import wid_popup
import tp
import copy
import wid_tp_detail
from enum import Enum


def wid_tp_grid_on_m_down_close(w, x, y, button):
    mm.con("TODO")
    return True


def wid_tp_grid_filter(w, f):
    p = w.get_top_parent()
    p.orig_args["filter"] = f
    WidTpGrid(k=p.orig_args)
    p.destroy()
    return False


class Item(Enum):
    all = 1
    weapon = 2
    magical = 3
    armor = 4
    healing = 5
    food = 6


def wid_tp_grid_on_m_down_filter_all_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.all.value)
    return True


def wid_tp_grid_on_m_down_filter_weapon_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.weapon.value)
    return True


def wid_tp_grid_on_m_down_filter_magic_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.magical.value)
    return True


def wid_tp_grid_on_m_down_filter_defensive_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.armor.value)
    return True


def wid_tp_grid_on_m_down_filter_healing_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.healing.value)
    return True


def wid_tp_grid_on_m_down_filter_edible_items(w, x, y, button):
    wid_tp_grid_filter(w, Item.food.value)
    return True


def wid_tp_grid_on_m_down(w, x, y, button):
    wid_tp_grid_common(w)
    w.set_tiles(tiles="button_green")
    return True


def wid_tp_grid_on_m_over(w, x, y, button):
    mm.con("o")
    return True


def wid_tp_grid_common(w):
    p = w.get_top_parent()

    if p.tp_detail is not None:
        p.tp_detail.destroy()
        p.tp_detail = None

    index = p.across * w.row + w.col
    name = p.tp_sorted_name_list[index]

    tpp = tp.all_tps[name]

    p.tp_detail = wid_tp_detail.WidTpDetail(name="tp window",
                                            tiles="wid2",
                                            body_tiles="wid1",
                                            width=p.detail_width,
                                            height=p.detail_height,
                                            x=p.detail_x,
                                            y=p.detail_y,
                                            tp_name=tpp.name)


def wid_tp_grid_on_key_down(w, sym, mod):
    wid_tp_grid_common(w)
    w.set_tiles(tiles="button_green")
    return True


def wid_tp_grid_on_m_over_b(w, relx, rely, wheelx, wheely):
    wid_tp_grid_common(w)
    return True


def wid_tp_grid_on_m_over_e(w):
    p = w.get_top_parent()

    if p.tp_detail is not None:
        p.tp_detail.destroy()
        p.tp_detail = None


class WidTpGrid(wid_popup.WidPopup):

    def __init__(self, k=None, **kp):

        if k is not None:
            super().__init__(**k)
            self.orig_args = copy.deepcopy(k)

            self.which = k["filter"]
            self.filter = k["filter"]
            self.x = k["x"]
            self.y = k["y"]
            self.detail_width = k["detail_width"]
            self.detail_height = k["detail_height"]
            self.detail_x = k["detail_x"]
            self.detail_y = k["detail_y"]
            self.width = k["width"]
        else:
            super().__init__(**kp)
            self.orig_args = copy.deepcopy(kp)

            self.which = Item.all
            self.filter = self.which.value
            self.x = kp["x"]
            self.y = kp["y"]
            self.detail_width = kp["detail_width"]
            self.detail_height = kp["detail_height"]
            self.detail_x = kp["detail_x"]
            self.detail_y = kp["detail_y"]
            self.width = kp["width"]

        self.tp_detail = None

        w = self

        button_events = (
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_all_items,
                    "tiles": "button_green",
                    "tooltip": "All items",
                },
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_weapon_items,
                    "tiles": "button_green",
                    "tooltip": "weapon filter",
                },
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_magic_items,
                    "tiles": "button_green",
                    "tooltip": "Magical items filter",
                },
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_defensive_items,
                    "tiles": "button_green",
                    "tooltip": "Defensive items filter",
                },
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_healing_items,
                    "tiles": "button_green",
                    "tooltip": "Healing items filter",
                },
                {
                    "on_m_down": wid_tp_grid_on_m_down_filter_edible_items,
                    "tiles": "button_green",
                    "tooltip": "Edible items filter",
                },
            )

        button_events[self.filter-1]["tiles"] = "button_red"

        w.add_text(
                font="small",
                color="white",
                title=True,
                center=True,
                on_button_list=button_events,
                text=" [%%tile=skull1.1$] " +
                     "[%%tile=sword_wooden1.1$] " +
                     "[%%tile=spellbook1.1$] " +
                     "[%%tile=shield1.1$] " +
                     "[%%tile=potion1.1$] " +
                     "[%%tile=brocolli$]"
                )

        self.tp_sorted_name_list = []

        added = 0

        for t in tp.all_tps:
            tpp = tp.all_tps[t]

            add = False

            if self.filter == Item.all.value:
                if tpp.is_player is True or \
                   tpp.is_food is True or \
                   tpp.is_weapon is True:
                    add = True

            if self.filter == Item.weapon.value:
                if tpp.is_weapon is True:
                    add = True

            if self.filter == Item.magical.value:
                if tpp.is_magical is True:
                    add = True

            if self.filter == Item.armor.value:
                if tpp.is_armor is True:
                    add = True

            if self.filter == Item.healing.value:
                if tpp.is_healing is True:
                    add = True

            if self.filter == Item.food.value:
                if tpp.is_food is True:
                    add = True

            if add is True:
                self.tp_sorted_name_list.append(tpp.name)
                added += 1

        self.tp_sorted_name_list.sort()

        font = "vlarge"
        tile_width, _unused_h, _unused_c = \
            mm.text_size_pct(font=font, text="[%%tile=player1.1$] ")
        self.across = int(self.width / tile_width)
        self.down = int((added / self.across) + 1)

#        grid = [[0 for x in range(self.across)] for y in range(self.down)]

        for y in range(self.down):
            button_events = []
            text = ""

            for x in range(self.across):
                index = (self.across * y) + x
                if index >= added:
                    break

                name = self.tp_sorted_name_list[index]

                tpp = tp.all_tps[name]

                button_events.append(
                        {
                            "on_m_down": wid_tp_grid_on_m_down,
                            "on_m_over_b": wid_tp_grid_on_m_over_b,
                            "on_m_over_e": wid_tp_grid_on_m_over_e,
                            "tiles": "button_plain",
                            "tooltip": name,
                        },
                    )

                tile = tpp.tiles[0]

                text += "[%%tile=" + tile + "$] "

            if text != "":
                w.add_text(
                        on_button_list=button_events,
                        font=font,
                        color="white",
                        text=text)

        if added == 0:
            w.add_text(
                    center=True,
                    font="small",
                    color="white",
                    text="--nothing--")

        w.update()
        w.set_focus()
        w.move_to_pct(x=self.x, y=self.y)
