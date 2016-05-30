/*
 * Copyright (C) 2011-2017 goblinhack@gmail.com
 *
 * See the README file for license info.
 */

#pragma once

#include "main.h"
#include "tree.h"

void color_init(void);
void glcolor_save(void);
void glcolor_restore(void);
color string2color(const char **s);
color color_find(const char *s);
const char *color_find_name(const char *s);
const char *color_find_nth(int nth);

extern color ALICE_BLUE;
extern color ALICEBLUE;
extern color ANTIQUEWHITE1;
extern color ANTIQUEWHITE2;
extern color ANTIQUEWHITE3;
extern color ANTIQUEWHITE4;
extern color ANTIQUE_WHITE;
extern color ANTIQUEWHITE;
extern color AQUAMARINE1;
extern color AQUAMARINE2;
extern color AQUAMARINE3;
extern color AQUAMARINE4;
extern color AQUAMARINE;
extern color AZURE1;
extern color AZURE2;
extern color AZURE3;
extern color AZURE4;
extern color AZURE;
extern color BEIGE;
extern color BISQUE1;
extern color BISQUE2;
extern color BISQUE3;
extern color BISQUE4;
extern color BISQUE;
extern color BLACK;
extern color BLANCHED_ALMOND;
extern color BLANCHEDALMOND;
extern color BLUE1;
extern color BLUE2;
extern color BLUE3;
extern color BLUE4;
extern color BLUE;
extern color BLUE_VIOLET;
extern color BLUEVIOLET;
extern color BROWN1;
extern color BROWN2;
extern color BROWN3;
extern color BROWN4;
extern color BROWN;
extern color BURLYWOOD1;
extern color BURLYWOOD2;
extern color BURLYWOOD3;
extern color BURLYWOOD4;
extern color BURLYWOOD;
extern color CADETBLUE1;
extern color CADETBLUE2;
extern color CADETBLUE3;
extern color CADETBLUE4;
extern color CADET_BLUE;
extern color CADETBLUE;
extern color CHARTREUSE1;
extern color CHARTREUSE2;
extern color CHARTREUSE3;
extern color CHARTREUSE4;
extern color CHARTREUSE;
extern color CHOCOLATE1;
extern color CHOCOLATE2;
extern color CHOCOLATE3;
extern color CHOCOLATE4;
extern color CHOCOLATE;
extern color CORAL1;
extern color CORAL2;
extern color CORAL3;
extern color CORAL4;
extern color CORAL;
extern color CORNFLOWER_BLUE;
extern color CORNFLOWERBLUE;
extern color CORNSILK1;
extern color CORNSILK2;
extern color CORNSILK3;
extern color CORNSILK4;
extern color CORNSILK;
extern color CYAN1;
extern color CYAN2;
extern color CYAN3;
extern color CYAN4;
extern color CYAN;
extern color DARK_BLUE;
extern color DARKBLUE;
extern color DARK_CYAN;
extern color DARKCYAN;
extern color DARKGOLDENROD1;
extern color DARKGOLDENROD2;
extern color DARKGOLDENROD3;
extern color DARKGOLDENROD4;
extern color DARK_GOLDENROD;
extern color DARKGOLDENROD;
extern color DARK_GRAY;
extern color DARKGRAY;
extern color DARK_GREEN;
extern color DARKGREEN;
extern color DARK_GREY;
extern color DARKGREY;
extern color DARK_KHAKI;
extern color DARKKHAKI;
extern color DARK_MAGENTA;
extern color DARKMAGENTA;
extern color DARKOLIVEGREEN1;
extern color DARKOLIVEGREEN2;
extern color DARKOLIVEGREEN3;
extern color DARKOLIVEGREEN4;
extern color DARK_OLIVE_GREEN;
extern color DARKOLIVEGREEN;
extern color DARKORANGE1;
extern color DARKORANGE2;
extern color DARKORANGE3;
extern color DARKORANGE4;
extern color DARK_ORANGE;
extern color DARKORANGE;
extern color DARKORCHID1;
extern color DARKORCHID2;
extern color DARKORCHID3;
extern color DARKORCHID4;
extern color DARK_ORCHID;
extern color DARKORCHID;
extern color DARK_RED;
extern color DARKRED;
extern color DARK_SALMON;
extern color DARKSALMON;
extern color DARKSEAGREEN1;
extern color DARKSEAGREEN2;
extern color DARKSEAGREEN3;
extern color DARKSEAGREEN4;
extern color DARK_SEA_GREEN;
extern color DARKSEAGREEN;
extern color DARK_SLATE_BLUE;
extern color DARKSLATEBLUE;
extern color DARKSLATEGRAY1;
extern color DARKSLATEGRAY2;
extern color DARKSLATEGRAY3;
extern color DARKSLATEGRAY4;
extern color DARK_SLATE_GRAY;
extern color DARKSLATEGRAY;
extern color DARK_SLATE_GREY;
extern color DARKSLATEGREY;
extern color DARK_TURQUOISE;
extern color DARKTURQUOISE;
extern color DARK_VIOLET;
extern color DARKVIOLET;
extern color DEEPPINK1;
extern color DEEPPINK2;
extern color DEEPPINK3;
extern color DEEPPINK4;
extern color DEEP_PINK;
extern color DEEPPINK;
extern color DEEPSKYBLUE1;
extern color DEEPSKYBLUE2;
extern color DEEPSKYBLUE3;
extern color DEEPSKYBLUE4;
extern color DEEP_SKY_BLUE;
extern color DEEPSKYBLUE;
extern color DIM_GRAY;
extern color DIMGRAY;
extern color DIM_GREY;
extern color DIMGREY;
extern color DODGERBLUE1;
extern color DODGERBLUE2;
extern color DODGERBLUE3;
extern color DODGERBLUE4;
extern color DODGER_BLUE;
extern color DODGERBLUE;
extern color FIREBRICK1;
extern color FIREBRICK2;
extern color FIREBRICK3;
extern color FIREBRICK4;
extern color FIREBRICK;
extern color FLORAL_WHITE;
extern color FLORALWHITE;
extern color FOREST_GREEN;
extern color FORESTGREEN;
extern color GAINSBORO;
extern color GHOST_WHITE;
extern color GHOSTWHITE;
extern color GOLD1;
extern color GOLD2;
extern color GOLD3;
extern color GOLD4;
extern color GOLDENROD1;
extern color GOLDENROD2;
extern color GOLDENROD3;
extern color GOLDENROD4;
extern color GOLDENROD;
extern color GOLD;
extern color GRAY10;
extern color GRAY11;
extern color GRAY12;
extern color GRAY13;
extern color GRAY14;
extern color GRAY15;
extern color GRAY16;
extern color GRAY17;
extern color GRAY18;
extern color GRAY19;
extern color GRAY1;
extern color GRAY20;
extern color GRAY21;
extern color GRAY22;
extern color GRAY23;
extern color GRAY24;
extern color GRAY25;
extern color GRAY26;
extern color GRAY27;
extern color GRAY28;
extern color GRAY29;
extern color GRAY2;
extern color GRAY30;
extern color GRAY31;
extern color GRAY32;
extern color GRAY33;
extern color GRAY34;
extern color GRAY35;
extern color GRAY36;
extern color GRAY37;
extern color GRAY38;
extern color GRAY39;
extern color GRAY3;
extern color GRAY40;
extern color GRAY41;
extern color GRAY42;
extern color GRAY43;
extern color GRAY44;
extern color GRAY45;
extern color GRAY46;
extern color GRAY47;
extern color GRAY48;
extern color GRAY49;
extern color GRAY4;
extern color GRAY50;
extern color GRAY51;
extern color GRAY52;
extern color GRAY53;
extern color GRAY54;
extern color GRAY55;
extern color GRAY56;
extern color GRAY57;
extern color GRAY58;
extern color GRAY59;
extern color GRAY5;
extern color GRAY60;
extern color GRAY61;
extern color GRAY62;
extern color GRAY63;
extern color GRAY64;
extern color GRAY65;
extern color GRAY66;
extern color GRAY67;
extern color GRAY68;
extern color GRAY69;
extern color GRAY6;
extern color GRAY70;
extern color GRAY71;
extern color GRAY72;
extern color GRAY73;
extern color GRAY74;
extern color GRAY75;
extern color GRAY76;
extern color GRAY77;
extern color GRAY78;
extern color GRAY79;
extern color GRAY7;
extern color GRAY80;
extern color GRAY81;
extern color GRAY82;
extern color GRAY83;
extern color GRAY84;
extern color GRAY85;
extern color GRAY86;
extern color GRAY87;
extern color GRAY88;
extern color GRAY89;
extern color GRAY8;
extern color GRAY90;
extern color GRAY91;
extern color GRAY92;
extern color GRAY93;
extern color GRAY94;
extern color GRAY95;
extern color GRAY96;
extern color GRAY97;
extern color GRAY98;
extern color GRAY99;
extern color GRAY9;
extern color GRAY;
extern color GREEN1;
extern color GREEN2;
extern color GREEN3;
extern color GREEN4;
extern color GREEN;
extern color GREEN_YELLOW;
extern color GREENYELLOW;
extern color GREY10;
extern color GREY11;
extern color GREY12;
extern color GREY13;
extern color GREY14;
extern color GREY15;
extern color GREY16;
extern color GREY17;
extern color GREY18;
extern color GREY19;
extern color GREY1;
extern color GREY20;
extern color GREY21;
extern color GREY22;
extern color GREY23;
extern color GREY24;
extern color GREY25;
extern color GREY26;
extern color GREY27;
extern color GREY28;
extern color GREY29;
extern color GREY2;
extern color GREY30;
extern color GREY31;
extern color GREY32;
extern color GREY33;
extern color GREY34;
extern color GREY35;
extern color GREY36;
extern color GREY37;
extern color GREY38;
extern color GREY39;
extern color GREY3;
extern color GREY40;
extern color GREY41;
extern color GREY42;
extern color GREY43;
extern color GREY44;
extern color GREY45;
extern color GREY46;
extern color GREY47;
extern color GREY48;
extern color GREY49;
extern color GREY4;
extern color GREY50;
extern color GREY51;
extern color GREY52;
extern color GREY53;
extern color GREY54;
extern color GREY55;
extern color GREY56;
extern color GREY57;
extern color GREY58;
extern color GREY59;
extern color GREY5;
extern color GREY60;
extern color GREY61;
extern color GREY62;
extern color GREY63;
extern color GREY64;
extern color GREY65;
extern color GREY66;
extern color GREY67;
extern color GREY68;
extern color GREY69;
extern color GREY6;
extern color GREY70;
extern color GREY71;
extern color GREY72;
extern color GREY73;
extern color GREY74;
extern color GREY75;
extern color GREY76;
extern color GREY77;
extern color GREY78;
extern color GREY79;
extern color GREY7;
extern color GREY80;
extern color GREY81;
extern color GREY82;
extern color GREY83;
extern color GREY84;
extern color GREY85;
extern color GREY86;
extern color GREY87;
extern color GREY88;
extern color GREY89;
extern color GREY8;
extern color GREY90;
extern color GREY91;
extern color GREY92;
extern color GREY93;
extern color GREY94;
extern color GREY95;
extern color GREY96;
extern color GREY97;
extern color GREY98;
extern color GREY99;
extern color GREY9;
extern color GREY;
extern color HONEYDEW1;
extern color HONEYDEW2;
extern color HONEYDEW3;
extern color HONEYDEW4;
extern color HONEYDEW;
extern color HOTPINK1;
extern color HOTPINK2;
extern color HOTPINK3;
extern color HOTPINK4;
extern color HOT_PINK;
extern color HOTPINK;
extern color INDIANRED1;
extern color INDIANRED2;
extern color INDIANRED3;
extern color INDIANRED4;
extern color INDIAN_RED;
extern color INDIANRED;
extern color IVORY1;
extern color IVORY2;
extern color IVORY3;
extern color IVORY4;
extern color IVORY;
extern color KHAKI1;
extern color KHAKI2;
extern color KHAKI3;
extern color KHAKI4;
extern color KHAKI;
extern color LAVENDERBLUSH1;
extern color LAVENDERBLUSH2;
extern color LAVENDERBLUSH3;
extern color LAVENDERBLUSH4;
extern color LAVENDER_BLUSH;
extern color LAVENDERBLUSH;
extern color LAVENDER;
extern color LAWN_GREEN;
extern color LAWNGREEN;
extern color LEMONCHIFFON1;
extern color LEMONCHIFFON2;
extern color LEMONCHIFFON3;
extern color LEMONCHIFFON4;
extern color LEMON_CHIFFON;
extern color LEMONCHIFFON;
extern color LIGHTBLUE1;
extern color LIGHTBLUE2;
extern color LIGHTBLUE3;
extern color LIGHTBLUE4;
extern color LIGHT_BLUE;
extern color LIGHTBLUE;
extern color LIGHT_CORAL;
extern color LIGHTCORAL;
extern color LIGHTCYAN1;
extern color LIGHTCYAN2;
extern color LIGHTCYAN3;
extern color LIGHTCYAN4;
extern color LIGHT_CYAN;
extern color LIGHTCYAN;
extern color LIGHTGOLDENROD1;
extern color LIGHTGOLDENROD2;
extern color LIGHTGOLDENROD3;
extern color LIGHTGOLDENROD4;
extern color LIGHT_GOLDENROD;
extern color LIGHTGOLDENROD;
extern color LIGHT_GOLDENROD_YELLOW;
extern color LIGHTGOLDENRODYELLOW;
extern color LIGHT_GRAY;
extern color LIGHTGRAY;
extern color LIGHT_GREEN;
extern color LIGHTGREEN;
extern color LIGHT_GREY;
extern color LIGHTGREY;
extern color LIGHTPINK1;
extern color LIGHTPINK2;
extern color LIGHTPINK3;
extern color LIGHTPINK4;
extern color LIGHT_PINK;
extern color LIGHTPINK;
extern color LIGHTSALMON1;
extern color LIGHTSALMON2;
extern color LIGHTSALMON3;
extern color LIGHTSALMON4;
extern color LIGHT_SALMON;
extern color LIGHTSALMON;
extern color LIGHT_SEA_GREEN;
extern color LIGHTSEAGREEN;
extern color LIGHTSKYBLUE1;
extern color LIGHTSKYBLUE2;
extern color LIGHTSKYBLUE3;
extern color LIGHTSKYBLUE4;
extern color LIGHT_SKY_BLUE;
extern color LIGHTSKYBLUE;
extern color LIGHT_SLATE_BLUE;
extern color LIGHTSLATEBLUE;
extern color LIGHT_SLATE_GRAY;
extern color LIGHTSLATEGRAY;
extern color LIGHT_SLATE_GREY;
extern color LIGHTSLATEGREY;
extern color LIGHTSTEELBLUE1;
extern color LIGHTSTEELBLUE2;
extern color LIGHTSTEELBLUE3;
extern color LIGHTSTEELBLUE4;
extern color LIGHT_STEEL_BLUE;
extern color LIGHTSTEELBLUE;
extern color LIGHTYELLOW1;
extern color LIGHTYELLOW2;
extern color LIGHTYELLOW3;
extern color LIGHTYELLOW4;
extern color LIGHT_YELLOW;
extern color LIGHTYELLOW;
extern color LIME_GREEN;
extern color LIMEGREEN;
extern color LIME;
extern color LINEN;
extern color MAGENTA1;
extern color MAGENTA2;
extern color MAGENTA3;
extern color MAGENTA4;
extern color MAGENTA;
extern color MAROON1;
extern color MAROON2;
extern color MAROON3;
extern color MAROON4;
extern color MAROON;
extern color MEDIUM_AQUAMARINE;
extern color MEDIUMAQUAMARINE;
extern color MEDIUM_BLUE;
extern color MEDIUMBLUE;
extern color MEDIUMORCHID1;
extern color MEDIUMORCHID2;
extern color MEDIUMORCHID3;
extern color MEDIUMORCHID4;
extern color MEDIUM_ORCHID;
extern color MEDIUMORCHID;
extern color MEDIUMPURPLE1;
extern color MEDIUMPURPLE2;
extern color MEDIUMPURPLE3;
extern color MEDIUMPURPLE4;
extern color MEDIUM_PURPLE;
extern color MEDIUMPURPLE;
extern color MEDIUM_SEA_GREEN;
extern color MEDIUMSEAGREEN;
extern color MEDIUM_SLATE_BLUE;
extern color MEDIUMSLATEBLUE;
extern color MEDIUM_SPRING_GREEN;
extern color MEDIUMSPRINGGREEN;
extern color MEDIUM_TURQUOISE;
extern color MEDIUMTURQUOISE;
extern color MEDIUM_VIOLET_RED;
extern color MEDIUMVIOLETRED;
extern color MIDNIGHT_BLUE;
extern color MIDNIGHTBLUE;
extern color MINT_CREAM;
extern color MINTCREAM;
extern color MISTYROSE1;
extern color MISTYROSE2;
extern color MISTYROSE3;
extern color MISTYROSE4;
extern color MISTY_ROSE;
extern color MISTYROSE;
extern color MOCCASIN;
extern color NAVAJOWHITE1;
extern color NAVAJOWHITE2;
extern color NAVAJOWHITE3;
extern color NAVAJOWHITE4;
extern color NAVAJO_WHITE;
extern color NAVAJOWHITE;
extern color NAVY_BLUE;
extern color NAVYBLUE;
extern color NAVY;
extern color OLD_LACE;
extern color OLDLACE;
extern color OLIVEDRAB1;
extern color OLIVEDRAB2;
extern color OLIVEDRAB3;
extern color OLIVEDRAB4;
extern color OLIVE_DRAB;
extern color OLIVEDRAB;
extern color LIGHT_ORANGE;
extern color ORANGE1;
extern color ORANGE2;
extern color ORANGE3;
extern color ORANGE4;
extern color ORANGE;
extern color ORANGERED1;
extern color ORANGERED2;
extern color ORANGERED3;
extern color ORANGERED4;
extern color ORANGE_RED;
extern color ORANGERED;
extern color ORCHID1;
extern color ORCHID2;
extern color ORCHID3;
extern color ORCHID4;
extern color ORCHID;
extern color PALE_GOLDENROD;
extern color PALEGOLDENROD;
extern color PALEGREEN1;
extern color PALEGREEN2;
extern color PALEGREEN3;
extern color PALEGREEN4;
extern color PALE_GREEN;
extern color PALEGREEN;
extern color PALETURQUOISE1;
extern color PALETURQUOISE2;
extern color PALETURQUOISE3;
extern color PALETURQUOISE4;
extern color PALE_TURQUOISE;
extern color PALETURQUOISE;
extern color PALEVIOLETRED1;
extern color PALEVIOLETRED2;
extern color PALEVIOLETRED3;
extern color PALEVIOLETRED4;
extern color PALE_VIOLET_RED;
extern color PALEVIOLETRED;
extern color PAPAYA_WHIP;
extern color PAPAYAWHIP;
extern color PEACHPUFF1;
extern color PEACHPUFF2;
extern color PEACHPUFF3;
extern color PEACHPUFF4;
extern color PEACH_PUFF;
extern color PEACHPUFF;
extern color PERU;
extern color PINK1;
extern color PINK2;
extern color PINK3;
extern color PINK4;
extern color PINK;
extern color PLUM1;
extern color PLUM2;
extern color PLUM3;
extern color PLUM4;
extern color PLUM;
extern color POWDER_BLUE;
extern color POWDERBLUE;
extern color PURPLE1;
extern color PURPLE2;
extern color PURPLE3;
extern color PURPLE4;
extern color PURPLE;
extern color RED1;
extern color RED2;
extern color RED3;
extern color RED4;
extern color RED;
extern color ROSYBROWN1;
extern color ROSYBROWN2;
extern color ROSYBROWN3;
extern color ROSYBROWN4;
extern color ROSY_BROWN;
extern color ROSYBROWN;
extern color ROYALBLUE1;
extern color ROYALBLUE2;
extern color ROYALBLUE3;
extern color ROYALBLUE4;
extern color ROYAL_BLUE;
extern color ROYALBLUE;
extern color SADDLE_BROWN;
extern color SADDLEBROWN;
extern color SALMON1;
extern color SALMON2;
extern color SALMON3;
extern color SALMON4;
extern color SALMON;
extern color SANDY_BROWN;
extern color SANDYBROWN;
extern color SEAGREEN1;
extern color SEAGREEN2;
extern color SEAGREEN3;
extern color SEAGREEN4;
extern color SEA_GREEN;
extern color SEAGREEN;
extern color SEASHELL1;
extern color SEASHELL2;
extern color SEASHELL3;
extern color SEASHELL4;
extern color SEASHELL;
extern color SIENNA1;
extern color SIENNA2;
extern color SIENNA3;
extern color SIENNA4;
extern color SIENNA;
extern color SKYBLUE1;
extern color SKYBLUE2;
extern color SKYBLUE3;
extern color SKYBLUE4;
extern color SKY_BLUE;
extern color SKYBLUE;
extern color SLATEBLUE1;
extern color SLATEBLUE2;
extern color SLATEBLUE3;
extern color SLATEBLUE4;
extern color SLATE_BLUE;
extern color SLATEBLUE;
extern color SLATEGRAY1;
extern color SLATEGRAY2;
extern color SLATEGRAY3;
extern color SLATEGRAY4;
extern color SLATE_GRAY;
extern color SLATEGRAY;
extern color SLATE_GREY;
extern color SLATEGREY;
extern color SNOW1;
extern color SNOW2;
extern color SNOW3;
extern color SNOW4;
extern color SNOW;
extern color SPRINGGREEN1;
extern color SPRINGGREEN2;
extern color SPRINGGREEN3;
extern color SPRINGGREEN4;
extern color SPRING_GREEN;
extern color SPRINGGREEN;
extern color STEELBLUE1;
extern color STEELBLUE2;
extern color STEELBLUE3;
extern color STEELBLUE4;
extern color STEEL_BLUE;
extern color STEELBLUE;
extern color TAN1;
extern color TAN2;
extern color TAN3;
extern color TAN4;
extern color TAN;
extern color THISTLE1;
extern color THISTLE2;
extern color THISTLE3;
extern color THISTLE4;
extern color THISTLE;
extern color TOMATO1;
extern color TOMATO2;
extern color TOMATO3;
extern color TOMATO4;
extern color TOMATO;
extern color TURQUOISE1;
extern color TURQUOISE2;
extern color TURQUOISE3;
extern color TURQUOISE4;
extern color TURQUOISE;
extern color VIOLET;
extern color VIOLETRED1;
extern color VIOLETRED2;
extern color VIOLETRED3;
extern color VIOLETRED4;
extern color VIOLET_RED;
extern color VIOLETRED;
extern color WHEAT1;
extern color WHEAT2;
extern color WHEAT3;
extern color WHEAT4;
extern color WHEAT;
extern color WHITE;
extern color WHITE_SMOKE;
extern color WHITESMOKE;
extern color YELLOW1;
extern color YELLOW2;
extern color YELLOW3;
extern color YELLOW4;
extern color YELLOW_GREEN;
extern color YELLOWGREEN;
extern color YELLOW;

extern color gl_save_color;
extern color gl_last_color;

static inline color gl_color_current (void)
{
    return (gl_last_color);
}

static inline int color_cmp (color a, color b)
{
    return ((a.r == b.r) &&
            (b.g == b.g) &&
            (b.b == b.b) &&
            (b.a == b.a));
}

extern color COLOR_NONE;

static inline int color_none (color a)
{
    color none = {0};

    return (color_cmp(a, none));
}

typedef struct {
    tree_key_string tree;
    color c;
    int include_in_palette;
} tree_color_val;

const char *color_find_col(color a);

extern tree_rootp colors;
