#pragma once

#include <glm/glm.hpp>

struct Color {
    static Color Amaranth, Amber, Amethyst, Apricot, Aquamarine, Azure, BabyBlue, Beige, Black, Blue, BlueGreen, BlueViolet, Blush, Bronze,
                 Brown, Burgundy, Byzantium, Carmine, Cerise, Cerulean, Champagne, ChartreuseGreen, Chocolate, CobaltBlue, Coffee, Copper, Coral, Crimson, Cyan,
                 DesertSand, ElectricBlue, Emerald, Erin, Gold, Gray, Green, Harlequin, Indigo, Ivory, Jade, JungleGreen, Lavender, Lemon, Lilac, Lime, Magenta,
                 MagentaRose, Maroon, Mauve, NavyBlue, Ochre, Olive, Orange, OrangeRed, Orchid, Peach, Pear, Periwinkle, PersianBlue, Pink, Plum, PrussianBlue,
                 Puce, Purple, Raspberry, Red, RedViolet, Rose, Ruby, Salmon, Sangria, Sapphire, Scarlet, Silver, SlateGray, SpringBud, SpringGreen, Tan, Taupe,
                 Teal, Transparent, Turquoise, Ultramarine, Violet, Viridian, White, Yellow;

    int r = 255, g = 255, b = 255, a = 255;

    Color(int r, int g, int b, int a = 255);
};
