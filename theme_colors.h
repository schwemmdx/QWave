#ifndef THEME_COLORS_H
#define THEME_COLORS_H


#include <QColor>



class Theme {
public:
       static constexpr QColor BackGround{QColor(39,40,34)};
       static constexpr QColor BackGround2{QColor(30,31,28)};
       static constexpr QColor CurrentLine{QColor(62,61,51)};
       static constexpr QColor ForeGround{QColor(245,245,239)};
       static constexpr QColor Comment{QColor(136,132,112)};
       static constexpr QColor Green{QColor(152,205,67)};
       static constexpr QColor Red{QColor(235,36,109)};
       static constexpr QColor Orange{QColor(221,133,51)};
       static constexpr QColor Blue{QColor(106,217,237)};
       static constexpr QColor Purple{QColor(174,131,250)};
        /*
        QColor Cyan(139,233,253);
        QColor Green(80,250,123);
        QColor Orange(255,184,108);
        QColor Pink(255,121,198);
        QColor Purple(189,147,249);
        QColor Red(255,85,85);
        QColor Yellow(241,250,140);
        */

};

#endif // THEME_COLORS_H
