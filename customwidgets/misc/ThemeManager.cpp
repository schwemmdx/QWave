#include <QDebug>

#include "ThemeColors.h"
#include "ThemeManager.h"


// Singleton instance
ThemeManager& ThemeManager::instance() {
    static ThemeManager instance;
    return instance;
}

// Private constructor
ThemeManager::ThemeManager() {
    initializeThemes();
}

// Initialize predefined themes
void ThemeManager::initializeThemes() {
    // Light theme
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor("#FFFFFF"));
    lightPalette.setColor(QPalette::WindowText, QColor("#000000"));
    lightPalette.setColor(QPalette::Base, QColor("#F0F0F0"));
    lightPalette.setColor(QPalette::AlternateBase, QColor("#E0E0E0"));
    lightPalette.setColor(QPalette::ToolTipBase, QColor("#FFFFE0"));
    lightPalette.setColor(QPalette::ToolTipText, QColor("#000000"));
    lightPalette.setColor(QPalette::Text, QColor("#000000"));
    lightPalette.setColor(QPalette::Button, QColor("#E0E0E0"));
    lightPalette.setColor(QPalette::ButtonText, QColor("#000000"));
    lightPalette.setColor(QPalette::Highlight, QColor("#3498DB"));
    lightPalette.setColor(QPalette::HighlightedText, QColor("#FFFFFF"));

    // Dark theme
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, Altium::BackGround2);
    darkPalette.setColor(QPalette::Text,Altium::LightText);
    darkPalette.setColor(QPalette::WindowText, Altium::LightText);
    darkPalette.setColor(QPalette::Base,Altium::BackGround2);
    darkPalette.setColor(QPalette::AlternateBase, Altium::BackGround2);
    darkPalette.setColor(QPalette::ToolTipBase, Altium::BackGround.lighter());
    darkPalette.setColor(QPalette::ToolTipText, Altium::LightText),
    darkPalette.setColor(QPalette::Text, Altium::LightText);
    darkPalette.setColor(QPalette::Button, Altium::BackGround.lighter());
    darkPalette.setColor(QPalette::ButtonText, Altium::LightText);
    darkPalette.setColor(QPalette::BrightText, Altium::LightText.lighter());
    darkPalette.setColor(QPalette::Highlight, Altium::Highlight);
    darkPalette.setColor(QPalette::HighlightedText,Altium::LightText);

    //pApplication->setPalette(darkpalette);

    // Add themes to the map
    themes["Light"] = lightPalette;
    themes["Dark"] = darkPalette;

    iconPaths["Light"] = ":/materials/light_theme/";
    iconPaths["Dark"] = ":/materials/dark_theme/";

    
    // Set default theme
    setTheme("Light");
}

// Set the active theme
bool ThemeManager::setTheme(const QString& themeName) {
    if (!themes.contains(themeName)) {
        qWarning() << "Theme not found:" << themeName;
        return false;
    }

    activePalette = themes[themeName];
    activeThemeName = themeName;
    activeIco = iconPaths[themeName];

    return true;
}

// Get the active theme name
QString ThemeManager::getCurrent()
{
    return activeThemeName;
}

// Get the active QPalette
QPalette ThemeManager::palette() const {
    return activePalette;
}
