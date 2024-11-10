#include "Message.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

Message::Message(const QString &text, const QIcon &icon, const QColor &textColor, int timeoutMs, QWidget *parent)
    : QWidget(parent), expirationTimer(new QTimer(this)) {

    setStyleSheet("background-color: #222; border-radius: 5px; padding: 0px;");

    // Layout for the message
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);  // Set spacing between the icon and text

    // Set up icon label with a fixed size of 32x32
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(icon.pixmap(32, 32));  // Set the icon size to 32x32
    layout->addWidget(iconLabel);

    // Set up text label with color and text wrapping
    iconLabel->setFixedSize(32, 32);  // Ensure the icon has a fixed size
    textLabel = new QLabel(text, this);
    textLabel->setStyleSheet("color: " + textColor.name() + ";");
    textLabel->setWordWrap(true);  // Enable word wrapping
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse); // Optional: allows selection of text
    layout->addWidget(textLabel);

    layout->setAlignment(iconLabel, Qt::AlignTop);  // Align the icon to the top
    layout->setAlignment(textLabel, Qt::AlignTop); 

    setLayout(layout);

    // Adjust the size of the widget based on the text content
    adjustSize();  // Automatically adjust the widget size based on content

    // Start expiration timer
    expirationTimer->setSingleShot(true);
    expirationTimer->setInterval(timeoutMs);
    connect(expirationTimer, &QTimer::timeout, this, &Message::startFadeOut);
    expirationTimer->start();
}

void Message::startFadeOut() {
    // Set up fade-out animation
    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(fadeEffect);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnimation->setDuration(500);  // 1 second fade out
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        emit expired(this);
    });

    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
