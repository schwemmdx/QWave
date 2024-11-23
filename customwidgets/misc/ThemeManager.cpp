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
