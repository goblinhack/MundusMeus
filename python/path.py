import os
import mm


def fixup(p):
    p = os.path.normcase(p)

    mm.con(p)
    (drive, path) = os.path.splitdrive(p)

    if ":" in drive:
        drive = drive[0]

    p = "/" + drive + "/" + path
    mm.con(p)

    return p
