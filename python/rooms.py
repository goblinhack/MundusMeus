#!/usr/bin/env python3
import room


def create_fixed():
    rooms = []

    r = room.Room()
    r.vert_slice_add("floor", [
                    "......",
                    "......",
                    "......",
                    "......",
                    "......",
            ])
    r.vert_slice_add("wall", [
                    "xxDxxx",
                    "x    D",
                    "D    x",
                    "x    x",
                    "xxxDxx",
            ])

    r.vert_slice_add("obj", [
                    "      ",
                    "   o  ",
                    "      ",
                    "      ",
                    "      ",
            ])
    r.finalize()
    rooms.append(r)

    r = room.Room()
    r.vert_slice_add("floor", [
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
                    "..........",
            ])
    r.vert_slice_add("wall", [
                    "xxDxxxxxxx",
                    "x  x     D",
                    "x  x   xxx",
                    "x        x",
                    "x        x",
                    "x        x",
                    "xxxxxxxxxx",
            ])

    r.vert_slice_add("obj", [
                    "          ",
                    "   o      ",
                    "          ",
                    "          ",
                    "          ",
                    "          ",
                    "          ",
            ])
    r.finalize()
    rooms.append(r)

    r = room.Room()
    r.vert_slice_add("floor", [
                    "   .......",
                    "   .......",
                    "..........",
                    "..........",
                    "..........",
                    ".......   ",
                    ".......   ",
            ])
    r.vert_slice_add("wall", [
                    "   xxxxxxx",
                    "   x     D",
                    "xxxx     x",
                    "D        x",
                    "x     xxxx",
                    "x     x   ",
                    "xxxxxxx   ",
            ])

    r.vert_slice_add("obj", [
                    "          ",
                    "          ",
                    "          ",
                    "          ",
                    "          ",
                    "          ",
                    "          ",
            ])
    r.finalize()
    rooms.append(r)

    r = room.Room()
    r.vert_slice_add("floor", [
                    "  ....  ",
                    " ...... ",
                    "........",
                    "........",
                    "........",
                    " .......",
                    "  ......",
            ])
    r.vert_slice_add("wall", [
                    "  xxDx  ",
                    " xx  xx ",
                    "xx    xx",
                    "D      D",
                    "xx     x",
                    " xx    x",
                    "  xDxxxx",
            ])

    r.vert_slice_add("obj", [
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
                    "        ",
            ])
    r.finalize()
    rooms.append(r)

    r = room.Room()
    r.vert_slice_add("floor", [
                    "....  ....   ",
                    ".............",
                    ".............",
                    " ............",
                    " ............",
                    ".............",
                    ".............",
                    "............ ",
                    "............ ",
                    "....   ..... ",
            ])
    r.vert_slice_add("wall", [
                    "xxDx  xxxx   ",
                    "x  xxxx  xxxx",
                    "xx          D",
                    " x          x",
                    " x          x",
                    "xx          x",
                    "D          xx",
                    "x          x ",
                    "x  xxxxx   x ",
                    "xxxx   xxxDx ",
            ])

    r.vert_slice_add("obj", [
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
            ])
    r.finalize()
    rooms.append(r)

    r = room.Room()
    r.vert_slice_add("floor", [
                    ".....   .....",
                    ".............",
                    ".............",
                    ".............",
                    "   .......   ",
                    "   .......   ",
                    "   .......   ",
                    ".............",
                    ".............",
                    ".............",
                    ".....   .....",
            ])
    r.vert_slice_add("wall", [
                    "xxxxx   xxxxx",
                    "x   xxDxx   x",
                    "x           x",
                    "xxxx     xxxx",
                    "   x     x   ",
                    "   D     D   ",
                    "   x     x   ",
                    "xxxx     xxxx",
                    "x           x",
                    "x   xxDxx   x",
                    "xxxxx   xxxxx",
            ])

    r.vert_slice_add("obj", [
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
                    "             ",
            ])
    r.can_be_placed_as_first_room = True
    r.finalize()
    rooms.append(r)

    return rooms
