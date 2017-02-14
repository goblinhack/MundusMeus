import mm
import color

colors = [
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Midnight",
    },
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Dead of night, 1am",
    },
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Dead of night, 2am",
    },
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Dead of night, 3am",
    },
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Dead of night, 4am",
    },
    {
        "color": color.ORANGE,
        "alpha": 50,
        "hour_str": "Early morning, 5am",
    },
    {
        "color": color.ORANGE,
        "alpha": 100,
        "hour_str": "Dawn, 6am",
    },
    {
        "color": color.ORANGE,
        "alpha": 150,
        "hour_str": "Morning, 7am",
    },
    {
        "color": color.WHITE,
        "alpha": 100,
        "hour_str": "Morning, 8am",
    },
    {
        "color": color.WHITE,
        "alpha": 150,
        "hour_str": "Morning, 9am",
    },
    {
        "color": color.WHITE,
        "alpha": 200,
        "hour_str": "Morning, 10am",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Second breakfast, 11am",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Lunchtime",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Afternoon, 1pm",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Afternoon, 2pm",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Afternoon, 3pm",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Afternoon, 4pm",
    },
    {
        "color": color.WHITE,
        "alpha": 255,
        "hour_str": "Afternoon, 5pm",
    },
    {
        "color": color.YELLOW1,
        "alpha": 255,
        "hour_str": "Dusk, 6pm",
    },
    {
        "color": color.ORANGE,
        "alpha": 150,
        "hour_str": "Sunset, 7pm",
    },
    {
        "color": color.GRAY60,
        "alpha": 100,
        "hour_str": "Sunset, 8pm",
    },
    {
        "color": color.GRAY60,
        "alpha": 50,
        "hour_str": "Night, 9pm",
    },
    {
        "color": color.GRAY60,
        "alpha": 50,
        "hour_str": "Night, 10pm",
    },
    {
        "color": color.CADETBLUE3,
        "alpha": 50,
        "hour_str": "Night, 11pm",
    },
]


def set_lighting_hour(hour):

    c = colors[hour]["color"]
    a = colors[hour]["alpha"]
    a *= 2.0
    if a > 255:
        a = 255

    return (c, a)


def set_lighting(game, move=0, moves_per_day=0):

    l = game.level

    move_in_day = (move + (moves_per_day / 2)) % moves_per_day

    hour = ((move_in_day * 24) / moves_per_day) % 24
    hour = int(hour)

    moves_per_hour = moves_per_day / 24

    step = (move % moves_per_hour) / moves_per_hour
    if step > 1.0:
        step = 1.0

    #
    # Get the lighting for this hour and the next hour and then give
    # a smooth transition
    #
    c1, a1 = set_lighting_hour(hour)
    c2, a2 = set_lighting_hour((hour + 1) % 24)

    r1 = c1[0]
    g1 = c1[1]
    b1 = c1[2]

    r2 = c2[0]
    g2 = c2[1]
    b2 = c2[2]

    r = int(r1 + (r2 - r1) * step)
    g = int(g1 + (g2 - g1) * step)
    b = int(b1 + (b2 - b1) * step)
    a = int(a1 + (a2 - a1) * step)

    if r > 255.0:
        r = 255
    elif r < 0:
        r = 0

    if g > 255.0:
        g = 255
    elif g < 0:
        g = 0

    if b > 255.0:
        b = 255
    elif b < 0:
        b = 0

    if a > 255.0:
        a = 255
    elif a < 0:
        a = 0

    game.hour = hour
    game.day = int(1 + (move / moves_per_day))
    game.hour_str = colors[hour]["hour_str"]

    if l.chunk[0][0].is_biome_dungeon:
        r = 255
        g = 255
        b = 255
        a = 255

    mm.game_set_daylight_color_r(r)
    mm.game_set_daylight_color_g(g)
    mm.game_set_daylight_color_b(b)
    mm.game_set_daylight_color_a(a)
