#include "Message.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QHBoxLayout>

Message::Message(const QString &text, MessageType msgType, const QIcon &icon,
                 const QColor &textColor, const QColor &borderColor, 
                 int timeoutMs, QWidget *parent)
    : QWidget(parent), type(msgType), expirationTimer(new QTimer(this)), progressBar(new QProgressBar(this)) {

    setupLayout(text, icon, textColor, borderColor); // Pass text here

    progressBar->setStyleSheet(
    "QProgressBar {"
    "    border: 0px solid #3C3D37;"
    "    border-radius: 0px;"
    "    background-color: #272822;"
    "    text-align: center;"
    "    color: #F8F8F2;" // Text color
    "}"
    "QProgressBar::chunk {"
    "    background-color: #A6E22E;" // Monokai green
    "    width: 15px;"
    "}"
    );
    progressBar->setTextVisible(false);
    expirationTimer->setSingleShot(true);
    expirationTimer->setInterval(timeoutMs);
   
    setFixedWidth(400);
    connect(expirationTimer, &QTimer::timeout, this, &Message::startFadeOut);
    connect(closeBtn, &QPushButton::clicked, this, &Message::startFadeOut);

    if (type == MSG_VOLATILE) {
        expirationTimer->start();
    } else if (type == MSG_PROGRESS) {
        progressBar->setVisible(true);
        expirationTimer->stop(); // Progress messages don’t expire by timer
    } else if (type == MSG_PERSISTANT) {
        closeBtn->show();
    }
}

void Message::setupLayout(const QString &text, const QIcon &icon, const QColor &textColor, const QColor &borderColor) {
    // Outer frame with visible border
    QFrame *borderFrame = new QFrame(this);
    borderFrame->setStyleSheet(QString(
        "QFrame { "
        "background-color: #272822; "
        "border: 1px solid %1; "
        "border-radius: 3px; }"
    ).arg(borderColor.name()));

    QVBoxLayout *borderLayout = new QVBoxLayout(borderFrame);
    borderLayout->setContentsMargins(0, 0, 0, 0);
    borderLayout->setSpacing(0);

    // Grid layout for the main content
    QGridLayout *contentLayout = new QGridLayout();
    contentLayout->setContentsMargins(10, 10, 10, 10); // Margins for the content
    contentLayout->setVerticalSpacing(5); // Vertical spacing between grid elements
    contentLayout->setHorizontalSpacing(10); // Horizontal spacing

    // Icon
    QLabel *iconLabel = new QLabel(borderFrame);
    iconLabel->setPixmap(icon.pixmap(32, 32));
    iconLabel->setFixedSize(32, 32);
    iconLabel->setStyleSheet("border: none;"); // No border for the icon
    contentLayout->addWidget(iconLabel, 0, 0, 1, 1, Qt::AlignLeft | Qt::AlignTop); // Align icon to the top

    // Text
    textLabel = new QLabel(text, borderFrame);
    textLabel->setStyleSheet(QString("color: %1; font-size: 14px; border: none;").arg(textColor.name())); // No border for the text
    textLabel->setWordWrap(true);
    contentLayout->addWidget(textLabel, 0, 1, 1, 1, Qt::AlignVCenter); // Vertically center the text

    // Close Button
    closeBtn = new QPushButton(borderFrame);
    closeBtn->setIcon(QIcon(":materials/dark_theme/close.png"));
    closeBtn->setFixedSize(16, 16);
    closeBtn->setStyleSheet("border: none;"); // No border for the button
    contentLayout->addWidget(closeBtn, 0, 2, 1, 1, Qt::AlignRight | Qt::AlignTop); // Align button to the top-right

    // Add the grid layout to the outer border layout
    borderLayout->addLayout(contentLayout);

    // Spacer between text and progress bar
    borderLayout->addSpacing(5); // Add 5px space

    // Progress Bar
    progressBar->setVisible(false);
    progressBar->setFixedHeight(8);
    progressBar->setStyleSheet("QProgressBar { border: none; }"); // No border for the progress bar
    borderLayout->addWidget(progressBar);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(borderFrame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);
}


void Message::updateProgress(int value) {
    if (progressBar) {
        value = std::clamp(value,0,100);
        progressBar->setValue(value);
        progressBar->setVisible(value < 100); // Hide the progress bar when complete
    }
    if (value >= 100) {
        expirationTimer->start(500); // Fade out after progress completion
    }
}

void Message::updateText(const QString &newText) {
    if (textLabel) {
        textLabel->setText(newText);
    }
}


void Message::startFadeOut() {
    QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(fadeEffect);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(fadeEffect, "opacity");
    fadeAnimation->setDuration(500);
    fadeAnimation->setStartValue(1.0);
    fadeAnimation->setEndValue(0.0);

    connect(fadeAnimation, &QPropertyAnimation::finished, this, [this]() {
        emit expired(this);
    });

    fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
