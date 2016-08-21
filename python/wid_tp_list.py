import mm
import wid
import wid_popup
import sys
import tp
import copy
import wid_tp_detail
from enum import Enum

def wid_tp_list_common(w):
    p = w.get_top_parent()
    p.destroy()

def wid_tp_list_yes(w):
    wid_tp_list_common(w)
    return False

def wid_tp_list_no():
    wid_tp_list_common(w)
    return False

def wid_tp_list_on_mouse_down_close(w, x, y, button):
    wid_tp_list_common(w)
    return True

def wid_tp_list_filter(w, f):
    p = w.get_top_parent()
    p.orig_args["filter"] = f
    WidTpList(k=p.orig_args)
    p.destroy()
    return False

class Item(Enum):
     all      = 1
     weapon   = 2
     magical  = 3
     armor    = 4
     healing  = 5
     food     = 6

def wid_tp_list_on_mouse_down_filter_1(w, x, y, button):
    wid_tp_list_filter(w, Item.all.value)
    return True

def wid_tp_list_on_mouse_down_filter_2(w, x, y, button):
    wid_tp_list_filter(w, Item.weapon.value)
    return True

def wid_tp_list_on_mouse_down_filter_3(w, x, y, button):
    wid_tp_list_filter(w, Item.magical.value)
    return True

def wid_tp_list_on_mouse_down_filter_4(w, x, y, button):
    wid_tp_list_filter(w, Item.armor.value)
    return True

def wid_tp_list_on_mouse_down_filter_5(w, x, y, button):
    wid_tp_list_filter(w, Item.healing.value)
    return True

def wid_tp_list_on_mouse_down_filter_6(w, x, y, button):
    wid_tp_list_filter(w, Item.food.value)
    return True

def wid_tp_list_on_mouse_down_no(w, x, y, button):
    wid_tp_list_no()
    return False

def wid_tp_list_on_key_down_yes(w, sym, mod):
    wid_tp_list_yes(w)
    return True

def wid_tp_list_on_key_down_no(w, sym, mod):
    wid_tp_list_no()
    return False # so focus does not select us after we are dead!

def wid_tp_list_on_mouse_over_begin(w):
    p = w.get_top_parent()

    if p.tp_detail != None:
        p.tp_detail.destroy()
        p.tp_detail = None

    tpp = p.tps[w.row]

    p.tp_detail = wid_tp_detail.WidTpDetail(name="tp window",
                                  tiles="wid2",
                                  body_tiles="wid1",
                                  width=0.25,
                                  height=0.5,
                                  x=0.50,
                                  y=0.00,
                                  tp_name=tpp.name)

def wid_tp_list_on_mouse_over_end(w):

    p = w.get_top_parent()

    if p.tp_detail != None:
        p.tp_detail.destroy()
        p.tp_detail = None

    return

class WidTpList(wid_popup.WidPopup):

    def __init__(self, k=None, **kp):

        if k != None:
            super().__init__(**k)
            self.orig_args = copy.deepcopy(k)

            self.which = k["filter"]
            self.filter = k["filter"]
            self.x = k["x"]
            self.y = k["y"]
        else:
            super().__init__(**kp)
            self.orig_args = copy.deepcopy(kp)

            self.which = Item.all
            self.filter = self.which.value
            self.x = kp["x"]
            self.y = kp["y"]

        self.tp_detail = None

        w = self

        button_events=( 
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_1, 
                    "tiles":"button_green", 
                    "tooltip":"All items", 
                },
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_2, 
                    "tiles":"button_green", 
                    "tooltip":"weapon filter",
                },
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_3, 
                    "tiles":"button_green", 
                    "tooltip":"Magical items filter",
                },
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_4, 
                    "tiles":"button_green", 
                    "tooltip":"Defensive items filter",
                },
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_5, 
                    "tiles":"button_green", 
                    "tooltip":"Healing items filter",
                },
                { 
                    "on_mouse_down":wid_tp_list_on_mouse_down_filter_6, 
                    "tiles":"button_green", 
                    "tooltip":"Edible items filter",
                },
            )

        button_events[self.filter-1]["tiles"] = "button_red"

        w.add_text(
                font="small", 
                color="white", 
                title=True,
                center=True,
                on_button_list=button_events,
#                text="[x] [x] [x] [x] [x] [x]")
                text="[%%tile=icon-star$] " +
                     "[%%tile=icon-sword$] " +
                     "[%%tile=icon-magic$] " +
                     "[%%tile=icon-shield$] " +
                     "[%%tile=icon-heal$] " +
                     "[%%tile=icon-food$]"
                )

        self.tps = []

        count = 0
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
                if count < 26:
                    key = chr(ord('a') + count)
                    key_str = "{0})".format(key)
                else:
                    key_str = "~"
                count += 1

                self.tps.append(tpp)

                w.add_text(
                        on_key_down=wid_tp_list_on_key_down_yes,
                        on_key_sym=mm.SDLK_RETURN,
                        on_mouse_down=wid_tp_list_on_mouse_down_close,
                        on_mouse_over_begin=wid_tp_list_on_mouse_over_begin,
                        on_mouse_over_end=wid_tp_list_on_mouse_over_end,
                        font="small", 
                        color="white", 
                        text="%%font=fixed$" + key_str + "%%tile=" + tpp.name + "$~%%font=small$" + tpp.short_name)
                added += 1

        if added == 0:
            w.add_text(
                    center=True,
                    font="small", 
                    color="white", 
                    text="--nothing--")

        w.update()
        w.set_focus()
        w.move_to_pct(x=self.x, y=self.y)
