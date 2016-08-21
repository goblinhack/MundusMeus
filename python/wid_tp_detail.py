import mm
import wid
import wid_popup
import sys
import tp
import copy

def wid_tp_detail_on_mouse_down(w, x, y, button):
    return True

class WidTpDetail(wid_popup.WidPopup):

    def __init__(self, x, y, tp_name, **k):

        super().__init__(**k)
        self.x = x
        self.y = y
        self.tp_name = tp_name

        w = self

        tpp = tp.all_tps[tp_name]

        button_events=( 
                { 
                    "on_mouse_down":wid_tp_detail_on_mouse_down, 
                    "tiles":"button_green", 
                    "tooltip":"All items", 
                },
        )

        w.add_text(
                font="large", 
                color="white", 
                title=True,
                center=True,
                text="%%tile=player1$ " + tpp.short_name)

        w.add_text(
                font="small", 
                color="white", 
                center=True,
                on_button_list=button_events,
                text="x")

        w.update()
        w.set_focus()
        w.move_to_pct(x=self.x, y=self.y)
