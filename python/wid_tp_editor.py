import mm
import wid_popup
import tp
import copy
import wid_tp_detail
import wid_focus
import game
from enum import Enum


global mywid


def wid_tp_editor_on_m_down_close(w, x, y, button):
    mm.con("TODO")
    return True


def wid_tp_editor_filter(w, f):
    p = w.get_top_parent()
    p.orig_args["filter"] = f
    w = WidTpEditor(k=p.orig_args)
    w.visible()
    p.destroy()
    return False


class Item(Enum):
    world = 1
    dungeon = 2
    weapon = 3
    magical = 4
    armor = 5
    healing = 6
    food = 7


def wid_tp_editor_on_m_down_filter_dungeon_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.dungeon.value)
    return True


def wid_tp_editor_on_m_down_filter_world_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.world.value)
    return True


def wid_tp_editor_on_m_down_filter_weapon_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.weapon.value)
    return True


def wid_tp_editor_on_m_down_filter_magic_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.magical.value)
    return True


def wid_tp_editor_on_m_down_filter_def_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.armor.value)
    return True


def wid_tp_editor_on_m_down_filter_healing_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.healing.value)
    return True


def wid_tp_editor_on_m_down_filter_edible_items(w, x, y, button):
    wid_tp_editor_filter(w, Item.food.value)
    return True


def wid_tp_editor_on_m_down(w, x, y, button):
    wid_tp_editor_common(w)

    index = w.context
    p = w.get_top_parent()
    name = p.tp_sorted_name_list[index]
    tpp = tp.all_tps[name]
    game.g.editor_mode_tp = tpp
    game.g.map_help()
    mywid.toggle_hidden()
    return True


def wid_tp_editor_common(w):
    p = w.get_top_parent()

    if p.tp_detail is not None:
        p.tp_detail.destroy()
        p.tp_detail = None

    index = w.context
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


def wid_tp_editor_on_key_down(w, sym, mod):
    wid_tp_editor_common(w)
    w.set_tiles(tiles="button_green")
    return True


def wid_tp_editor_on_m_over_b(w, relx, rely, wheelx, wheely):
    wid_tp_editor_common(w)
    return True


def wid_tp_editor_on_m_over_e(w):
    p = w.get_top_parent()

    if p.tp_detail is not None:
        p.tp_detail.destroy()
        p.tp_detail = None


class WidTpEditor(wid_popup.WidPopup):

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

            self.which = Item.world
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

        global mywid
        mywid = w

        w.set_on_key_down(key_down)

        button_events = (
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_world_items,
                    "tiles": "button_green",
                    "tooltip": "World items",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_dungeon_items,
                    "tiles": "button_green",
                    "tooltip": "Dungeon items",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_weapon_items,
                    "tiles": "button_green",
                    "tooltip": "Weapon filter",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_magic_items,
                    "tiles": "button_green",
                    "tooltip": "Magical items filter",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_def_items,
                    "tiles": "button_green",
                    "tooltip": "Defensive items filter",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_healing_items,
                    "tiles": "button_green",
                    "tooltip": "Healing items filter",
                },
                {
                    "on_m_down": wid_tp_editor_on_m_down_filter_edible_items,
                    "tiles": "button_green",
                    "tooltip": "Edible items filter",
                },
            )

        button_events[self.filter-1]["tiles"] = "button_red"

        w.add_text(
                font="large",
                color="white",
                title=True,
                center=True,
                on_button_list=button_events,
                text="'%%tile=ball1.1$' " +
                     "'%%tile=skull1.1$' " +
                     "'%%tile=sword_wooden1.1$' " +
                     "'%%tile=spellbook1.1$' " +
                     "'%%tile=shield1.1$' " +
                     "'%%tile=potion1.1$' " +
                     "'%%tile=brocolli$'"
                )

        self.tp_sorted_name_list = []

        for t in tp.all_tps:
            tpp = tp.all_tps[t]

            add = False

            if self.filter == Item.world.value:
                if tpp.is_player is True or \
                   tpp.is_world_item is True:
                    add = True

            if self.filter == Item.dungeon.value:
                if tpp.is_dungeon_item is True:
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

        self.tp_sorted_name_list.sort()

        font = "vvlarge"

        text = ""
        index = 0
        button_events = []

        for index in range(len(self.tp_sorted_name_list)):

            name = self.tp_sorted_name_list[index]

            tpp = tp.all_tps[name]

            text += "'%%tp=" + name + "$'"

            name = tpp.short_name

            button_events.append(
                    {
                        "on_m_down": wid_tp_editor_on_m_down,
                        "on_m_over_b": wid_tp_editor_on_m_over_b,
                        "on_m_over_e": wid_tp_editor_on_m_over_e,
                        "tiles": "button_plain",
                        "tooltip": name,
                        "context": index,
                    },
                )

        if text != "":
            w.add_text(
                    on_button_list=button_events,
                    font=font,
                    color="white",
                    text=text)
        else:
            w.add_text(
                    center=True,
                    font="small",
                    color="white",
                    text="--nothing--")

        w.update()
        w.set_focus()
        w.to_front()
        w.move_to_pct(x=self.x, y=self.y)
        w.hide()


def key_down(w, sym, mod):

    if sym == mm.SDLK_q:
        mywid.toggle_hidden()
        return True

    if sym == mm.SDLK_ESCAPE or sym == mm.SDLK_TAB:
        mywid.toggle_hidden()
        return True

    if mod == mm.KMOD_LCTRL:
        if sym == mm.SDLK_e:
            mywid.toggle_hidden()
            return True

    return True


def visible():
    global mywid
    if mywid is None:
        return
    mywid.toggle_hidden()
    mywid.set_focus()
    mywid.to_front()
    wid_focus.set_focus(mywid)

    game.g.editor_mode = True


def hide():
    global mywid
    if mywid is None:
        return

    mywid.hide()
    game.g.editor_mode = False
    return True
