#include "SettingsTree.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFontDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>

// SettingsTree implementation
SettingsTree::SettingsTree(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(2);
    setHeaderLabels(QStringList() << "Setting" << "Value");
}

SettingsTree::~SettingsTree() = default;

void SettingsTree::addSetting(const QString &category, const QString &name, const QVariant &value, SettingType type, Callback onChange)
{
    QTreeWidgetItem *categoryItem = findCategory(category);
    if (!categoryItem) {
        categoryItem = new QTreeWidgetItem(this);
        categoryItem->setText(0, category);
        categoryItem->setExpanded(true);
    }

    QTreeWidgetItem *settingItem = findSetting(categoryItem, name);
    if (!settingItem) {
        settingItem = new QTreeWidgetItem(categoryItem);
        settingItem->setText(0, name);
    }

    settingsMap[settingItem] = {value, onChange}; // Store value and callback
    createEditor(settingItem, type, value, onChange);
}

QTreeWidgetItem* SettingsTree::findCategory(const QString &category) const
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = topLevelItem(i);
        if (item->text(0) == category)
            return item;
    }
    return nullptr;
}

QTreeWidgetItem* SettingsTree::findSetting(QTreeWidgetItem *categoryItem, const QString &name) const
{
    for (int i = 0; i < categoryItem->childCount(); ++i) {
        QTreeWidgetItem *item = categoryItem->child(i);
        if (item->text(0) == name)
            return item;
    }
    return nullptr;
}

void SettingsTree::createEditor(QTreeWidgetItem *item, SettingType type, const QVariant &value, Callback onChange)
{
    QWidget *editor = createWidget(type, value, onChange);
    if (editor) {
        setItemWidget(item, 1, editor);
    }
}

QWidget* SettingsTree::createWidget(SettingType type, const QVariant &value, Callback onChange)
{
    switch (type) {
        case Bool: {
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setChecked(value.toBool());
            connect(checkBox, &QCheckBox::toggled, this, [onChange](bool checked) {
                if (onChange) onChange(checked);
            });
            return checkBox;
        }
        case Int: {
            QSpinBox *spinBox = new QSpinBox();
            spinBox->setRange(-100000, 100000);
            spinBox->setValue(value.toInt());
            connect(spinBox, qOverload<int>(&QSpinBox::valueChanged), this, [onChange](int newValue) {
                if (onChange) onChange(newValue);
            });
            return spinBox;
        }
        case Float: {
            QDoubleSpinBox *doubleSpinBox = new QDoubleSpinBox();
            doubleSpinBox->setRange(-100000.0, 100000.0);
            doubleSpinBox->setDecimals(2);
            doubleSpinBox->setValue(value.toDouble());
            connect(doubleSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [onChange](double newValue) {
                if (onChange) onChange(newValue);
            });
            return doubleSpinBox;
        }
        case String: {
            QLineEdit *lineEdit = new QLineEdit();
            lineEdit->setText(value.toString());
            connect(lineEdit, &QLineEdit::textChanged, this, [onChange](const QString &text) {
                if (onChange) onChange(text);
            });
            return lineEdit;
        }
        case Font: {
            QPushButton *fontEditor = new QPushButton();
            fontEditor->setText(value.toString());
            connect(fontEditor, &QPushButton::clicked, [fontEditor, onChange]() {
                bool ok;
                QFont font = QFontDialog::getFont(&ok, QFont(fontEditor->text()), nullptr, "Select Font");
                if (ok) {
                    fontEditor->setText(font.toString());
                    if (onChange) onChange(font);
                }
            });
            return fontEditor;
        }
        case Color: {
            QPushButton *colorEditor = new QPushButton();
            QColor initialColor = QColor(value.toString());
            colorEditor->setText(initialColor.name());
            colorEditor->setStyleSheet(QString("background-color: %1;").arg(initialColor.name()));
            connect(colorEditor, &QPushButton::clicked, [colorEditor, onChange]() {
                QColor selectedColor = QColorDialog::getColor(QColor(colorEditor->text()), nullptr, "Select Color");
                if (selectedColor.isValid()) {
                    colorEditor->setText(selectedColor.name());
                    colorEditor->setStyleSheet(QString("background-color: %1;").arg(selectedColor.name()));
                    if (onChange) onChange(selectedColor);
                }
            });
            return colorEditor;
        }
        default:
            return nullptr;
    }
}
