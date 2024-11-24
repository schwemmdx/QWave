#ifndef SETTINGSTREE_H
#define SETTINGSTREE_H

#include <QTreeWidget>
#include <QVariant>
#include <functional>

class SettingsTree : public QTreeWidget
{
    Q_OBJECT

public:
    // Enum for setting types
    enum SettingType {
        Bool,
        Int,
        Float,
        String,
        Font,
        Color
    };

    // Type alias for a callback function
    using Callback = std::function<void(const QVariant &newValue)>;

    explicit SettingsTree(QWidget *parent = nullptr);
    ~SettingsTree() override;

    // Adds a setting with a specific callback
    void addSetting(const QString &category, const QString &name, const QVariant &value, SettingType type, Callback onChange);

private:
    // Struct to store setting data
    struct SettingData {
        QVariant value;
        Callback onChange;
    };

    // Helper functions
    QTreeWidgetItem* findCategory(const QString &category) const;
    QTreeWidgetItem* findSetting(QTreeWidgetItem *categoryItem, const QString &name) const;

    // Creates an editor widget for a specific setting
    void createEditor(QTreeWidgetItem *item, SettingType type, const QVariant &value, Callback onChange);
    QWidget* createWidget(SettingType type, const QVariant &value, Callback onChange);

    // Map to store settings and their associated data
    QMap<QTreeWidgetItem*, SettingData> settingsMap;
};



#endif // SETTINGSTREE_H
