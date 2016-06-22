import mm

def set_game_video_pix_width (value):
    mm.game_video_pix_width = value
    mm.con(str(mm.game_video_pix_width))

def get_game_video_pix_width (x):
    return (mm.game_video_pix_width)
    
