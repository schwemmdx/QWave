#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QIcon>
#include <QColor>

class Message : public QWidget {
    Q_OBJECT

public:
    Message(const QString &text, const QIcon &icon, const QColor &textColor, int timeoutMs, QWidget *parent = nullptr);

signals:
    void expired(Message *self);

private:
    QLabel *textLabel;
    QTimer *expirationTimer;

private slots:
    void startFadeOut();
};

#endif // MESSAGE_H
