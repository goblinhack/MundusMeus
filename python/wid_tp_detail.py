import wid_popup
import tp


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

        w.add_text(
                font="vlarge",
                color="white",
                title=True,
                center=True,
                text="%%tp=" + tpp.name + "$ " + tpp.short_name)

        if tpp.long_name is not None:
            w.add_text(tpp.long_name)

        if tpp.is_weapon:
            w.add_text(color="gray",
                       text="Damage %%fg=red$" + tpp.damage)

            if tpp.is_double_handed:
                w.add_text(color="green",
                           text="This weapon requires two hands to use.")

        w.update()
        w.set_focus()
        w.move_to_pct(x=self.x, y=self.y)
