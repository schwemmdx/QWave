#include "MultiColumnToolbar.h"
#include <QGridLayout>
#include <QToolButton>
#include <QSizePolicy>
#include <QIcon>

ActionListWidget::ActionListWidget(QWidget *parent)
    : QWidget(parent) {
    setFixedWidth(widgetWidth);

    // Create vertical layout
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(5); // Space between items
    layout->setContentsMargins(10, 10, 10, 10);

    // Create and populate the list with actions
    QVector<QAction*> actions = createExampleActions();
    setupList(layout, actions);

    setLayout(layout);
}

QVector<QAction*> ActionListWidget::createExampleActions() {
    QVector<QAction*> actions;

    // Example actions with icons and labels (replace with actual icons)
    QVector<QString> actionNames = {"Open", "Save", "Close", "Edit", "Cut", "Copy", "Paste", "Delete", "Print"};
    QVector<QString> actionIcons = {
        ":/icons/open.png", ":/icons/save.png", ":/icons/close.png",
        ":/icons/edit.png", ":/icons/cut.png", ":/icons/copy.png",
        ":/icons/paste.png", ":/icons/delete.png", ":/icons/print.png"
    };

    for (int i = 0; i < actionNames.size(); ++i) {
        QAction *action = new QAction(QIcon(actionIcons[i]), actionNames[i], this);
        actions.append(action);
    }

    return actions;
}

void ActionListWidget::setupList(QGridLayout *layout, QVector<QAction*> actions) {
    int row=0;
    int col =0;
    for (auto action : actions) {
        QToolButton *button = new QToolButton(this);
        button->setDefaultAction(action);
        button->setToolButtonStyle(Qt::ToolButtonIconOnly); // Icon above text
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        button->setFixedSize(30, 30); // Set fixed width and height

        if(col==5){
            col = 0;
            row+=1;
        }
        layout->addWidget(button,row,col);
        col++;
    }

   
}
