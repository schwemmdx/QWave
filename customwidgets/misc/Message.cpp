#include "Message.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>
#include <QIcon>

#include "ThemeColors.h"

Message::Message(const QString &text, const QIcon &icon, 
    const QColor &textColor,const QColor &borderColor, int timeoutMs, QWidget *parent)
    : QWidget(parent), expirationTimer(new QTimer(this)) {

    // Create the border frame with styling
    QFrame *borderFrame = new QFrame(this);
    borderFrame->setObjectName("borderFrame"); // Optional: for debugging or styling
    borderFrame->setStyleSheet(
        "QFrame#borderFrame { "
        "   background-color: " + QColor(Monokai::SecondaryBackground).name() + "; "
        "   border: 1px solid " + borderColor.name() + "; "
        "   border-radius: 5px; "
        "}"
    );

    // Create a layout for the border frame
    QVBoxLayout *borderLayout = new QVBoxLayout(borderFrame);
    borderLayout->setContentsMargins(10, 10, 10, 10); // Padding between border and content
    borderLayout->setSpacing(5);

    // Layout for content (icon and text)
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(0, 0, 0, 0); // No additional margins
    contentLayout->setSpacing(15);                 // Space between icon and text

    // Add the icon
    QLabel *iconLabel = new QLabel(borderFrame); // Parent is the borderFrame
    iconLabel->setPixmap(icon.pixmap(32, 32));
    iconLabel->setFixedSize(32, 32);
    iconLabel->setAlignment(Qt::AlignTop);
    iconLabel->setStyleSheet("background: "+QColor(Monokai::SecondaryBackground).name()+";"); // Ensure no background is applied
    contentLayout->addWidget(iconLabel);

    // Add the text
    textLabel = new QLabel(text, borderFrame); // Parent is the borderFrame
    textLabel->setStyleSheet(
        "background: "+QColor(Monokai::SecondaryBackground).name()+";" // Ensure no background is applied
        "color: " + textColor.name() + ";"
        "font-size: 14px;"
    );
    textLabel->setWordWrap(true);
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contentLayout->addWidget(textLabel);

    // Add the content layout to the border layout
    borderLayout->addLayout(contentLayout);

    // Main layout for the Message widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // No margins for the outer widget
    mainLayout->setSpacing(0);                  // No spacing between the widget and the frame
    mainLayout->addWidget(borderFrame);
    setLayout(mainLayout);

    // Expiration timer
    expirationTimer->setSingleShot(true);
    expirationTimer->setInterval(timeoutMs);
    connect(expirationTimer, &QTimer::timeout, this, &Message::startFadeOut);
    expirationTimer->start();
}

void Message::startFadeOut() {
    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(fadeEffect);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnimation->setDuration(1000); // 1-second fade-out
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        emit expired(this);
    });

    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
