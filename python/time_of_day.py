import mm
import color


def set_lighting(move=0, moves_per_day=0):

    move_in_day = (move + (moves_per_day / 2)) % moves_per_day

    hour = ((move_in_day * 24) / moves_per_day) % 24
    hour = int(hour)

    print(hour)
    colors = [
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
        {
            "color": color.BLUE,
            "alpha": 50,
        },
    ]

    c = colors[hour]["color"]
    a = colors[hour]["alpha"]
    print(move, c, a)

    mm.game_set_daylight_color_r(c[0])
    mm.game_set_daylight_color_g(c[1])
    mm.game_set_daylight_color_b(c[2])
    mm.game_set_daylight_color_a(a)
