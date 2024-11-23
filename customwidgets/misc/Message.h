#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QIcon>

#include "ThemeColors.h"

class Message : public QWidget {
    Q_OBJECT

public:
    explicit Message(const QString &text, const QIcon &icon= QIcon(), 
                     const QColor &textColor = Monokai::SecondaryLabel,
                     const QColor &borderColor= Monokai::BorderColor,
                     int timeoutMs = 3000, QWidget *parent = nullptr);

signals:
    void expired(Message *message);

private:
    QLabel *textLabel;       // Label for the message text
    QTimer *expirationTimer; // Timer for auto-expiration
    void startFadeOut();     // Trigger fade-out animation
};

#endif // MESSAGE_H
