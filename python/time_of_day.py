import mm
import color


def set_lighting(game, move=0, moves_per_day=0):

    move_in_day = (move + (moves_per_day / 2)) % moves_per_day

    hour = ((move_in_day * 24) / moves_per_day) % 24
    hour = int(hour)

    game.hour = hour
    game.day = move % moves_per_day

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
            "color": color.ORANGE,
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

    c = colors[hour]["color"]
    a = colors[hour]["alpha"]
    game.hour_str = colors[hour]["hour_str"]

    mm.game_set_daylight_color_r(c[0])
    mm.game_set_daylight_color_g(c[1])
    mm.game_set_daylight_color_b(c[2])
    mm.game_set_daylight_color_a(a)
