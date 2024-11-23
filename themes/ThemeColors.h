#ifndef THEME_COLORS_H
#define THEME_COLORS_H

#include <QColor>

class Monokai {
public:
    // Primary Colors
    static constexpr QColor PrimaryColor{QColor(248, 248, 242)};  
    static constexpr QColor HighlightColor{QColor(166, 226, 46,75)}; // Green for highlights (Monokai)

    // Secondary Colors
    static constexpr QColor SecondaryColor{QColor(102, 217, 239)}; // Cyan
    static constexpr QColor AccentColor{QColor(255, 204, 0)};      // Yellow for accents

    // Background Colors
    static constexpr QColor SystemBackground{QColor(39, 40, 34)};  // Dark Gray (Base background)

    static constexpr QColor SystemBackgroundDark{QColor(29, 30, 24)};
    static constexpr QColor SecondaryBackground{QColor(49, 51, 45)}; // Slightly lighter gray
    static constexpr QColor TertiaryBackground{QColor(75, 76, 68)};  // Medium gray for elevated UI elements

    // Label Colors
    static constexpr QColor PrimaryLabel{QColor(248, 248, 242)};      // Off-white
    static constexpr QColor SecondaryLabel{QColor(248, 248, 242, 153)}; // Off-white, 60% opacity
    static constexpr QColor TertiaryLabel{QColor(248, 248, 242, 77)};  // Off-white, 30% opacity
    static constexpr QColor QuaternaryLabel{QColor(248, 248, 242, 38)}; // Off-white, 15% opacity

    // Border Colors
    static constexpr QColor BorderColor{QColor(117, 113, 94)};    // Muted Yellow-gray
    static constexpr QColor FocusBorderColor{QColor(102, 217, 239)}; // Cyan for focus/active state

    // Fill Colors
    static constexpr QColor PrimaryFill{QColor(117, 113, 94, 51)};  // Yellow-gray, 20% opacity
    static constexpr QColor SecondaryFill{QColor(117, 113, 94, 26)}; // Yellow-gray, 10% opacity

    // Warning and Error Colors
    static constexpr QColor WarningColor{QColor(255, 204, 0)};  // Yellow (Monokai Accent)
    static constexpr QColor ErrorColor{QColor(249, 38, 114)};   // Vibrant Pink for errors
};

class IOSLight {
public:
    // Primary Colors
    static constexpr QColor PrimaryColor{QColor(0, 122, 255)};
    static constexpr QColor PrimaryText{QColor(0, 0, 0)};

    // Secondary Colors
    static constexpr QColor SecondaryColor{QColor(52, 199, 89)};
    static constexpr QColor SecondaryText{QColor(28, 28, 30)};

    // Background Colors
    static constexpr QColor SystemBackground{QColor(255, 255, 255)};
    static constexpr QColor SecondaryBackground{QColor(242, 242, 247)};
    static constexpr QColor TertiaryBackground{QColor(229, 229, 234)};

    // Label Colors
    static constexpr QColor PrimaryLabel{QColor(0, 0, 0)};
    static constexpr QColor SecondaryLabel{QColor(60, 60, 67, 153)}; // 60% opacity black
    static constexpr QColor TertiaryLabel{QColor(60, 60, 67, 77)};   // 30% opacity black
    static constexpr QColor QuaternaryLabel{QColor(60, 60, 67, 38)}; // 10% opacity black

    // Fill Colors
    static constexpr QColor PrimaryFill{QColor(120, 120, 128, 51)}; // Separator
    static constexpr QColor SecondaryFill{QColor(120, 120, 128, 26)}; // Overlay
};

class IOSDark {
public:
    // Primary Colors
    static constexpr QColor PrimaryColor{QColor(10, 132, 255)};
    static constexpr QColor PrimaryText{QColor(255, 255, 255)};

    // Secondary Colors
    static constexpr QColor SecondaryColor{QColor(48, 209, 88)};
    static constexpr QColor SecondaryText{QColor(242, 242, 247)};

    // Background Colors
    static constexpr QColor SystemBackground{QColor(0, 0, 0)};
    static constexpr QColor SecondaryBackground{QColor(28, 28, 30)};
    static constexpr QColor TertiaryBackground{QColor(44, 44, 46)};

    // Label Colors
    static constexpr QColor PrimaryLabel{QColor(255, 255, 255)};
    static constexpr QColor SecondaryLabel{QColor(235, 235, 245, 153)}; // 60% opacity white
    static constexpr QColor TertiaryLabel{QColor(235, 235, 245, 77)};   // 30% opacity white
    static constexpr QColor QuaternaryLabel{QColor(235, 235, 245, 38)}; // 10% opacity white

    // Fill Colors
    static constexpr QColor PrimaryFill{QColor(72, 72, 74, 51)}; // Separator
    static constexpr QColor SecondaryFill{QColor(72, 72, 74, 26)}; // Overlay
};


#endif // THEME_COLORS_H
