#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QColor>
#include <QBrush>
#include <QString>
#include <QPalette>
#include <QMap>

struct GridColors{
    QColor major;
    QColor minor;
};

class ThemeManager {
public:
    // Singleton access
    static ThemeManager& instance();

    // Set the active theme
    bool setTheme(const QString& themeName);

    // Get the active QPalette for the current theme
    QPalette palette() const;

    // Get the active theme name
    QString getCurrent();

private:
    // Private constructor for singleton
    ThemeManager();


    // Initialize predefined themes
    void initializeThemes();

    // Store predefined themes as QPalettes
    QMap<QString, QPalette> themes;
    //Icon resource string 
    QMap<QString,QString> iconPaths;

    // Active theme
    QPalette activePalette;
    QString activeThemeName;
    QString activeIco;


    //coloring of the chart and its components
    QColor axisColor;
    QColor axisLabelColor;
    GridColors yleftGrid;
    GridColors yRightGrid;
    GridColors xGrid;

    
    
};

#endif // THEMEMANAGER_H
