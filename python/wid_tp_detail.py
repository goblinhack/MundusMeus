import mm
import wid
import wid_popup
import sys
import tp
import copy

def wid_tp_detail_on_m_down(w, x, y, button):
    return True

class WidTpDetail(wid_popup.WidPopup):

    def __init__(self, x, y, tp_name, **k):

        super().__init__(**k)
        self.x = x
        self.y = y
        self.tp_name = tp_name

        w = self

        tpp = tp.all_tps[tp_name]

        button_events = (
                {
                    "on_m_down": wid_tp_detail_on_m_down,
                    "tiles": "button_green",
                    "tooltip": "All items",
                },
        )

        tile_name = tpp.tiles[0]

        w.add_text(
                font="vlarge",
                color="white",
                title=True,
                center=True,
                text="%%tile=" + tile_name + "$ " + tpp.short_name)

        if tpp.description is not None:
            w.add_text(tpp.description)

        if tpp.is_weapon:
            w.add_text(color="gray",
                       text="Damage %%fg=red$" + tpp.damage)

            if tpp.is_double_handed:
                w.add_text(color="green",
                           text="This weapon requires two hands to use.")

        w.update()
        w.set_focus()
        w.move_to_pct(x=self.x, y=self.y)
