#ifndef MESSAGE_H
#define MESSAGE_H

#include <QWidget>
#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

// Enum to define message types
enum MessageType {
    MSG_VOLATILE = 0,
    MSG_PROGRESS,
    MSG_WARNING,
    MSG_PERSISTANT,
};

class Message : public QWidget {
    Q_OBJECT

public:
    explicit Message(const QString &text, MessageType msgType, const QIcon &icon,
                     const QColor &textColor, const QColor &borderColor, 
                     int timeoutMs, QWidget *parent = nullptr);

public slots:
    void updateProgress(int value); // Slot to update progress bar
    void startFadeOut();            // Slot to start fading out the message
    void updateText(const QString &newText);

    //void setMode(MessageType msgType); // Slot to change message mode

signals:
    void expired(Message *message); // Signal emitted when the message expires

private:
    MessageType type;
    QLabel *textLabel;
    QPushButton *closeBtn;
    QProgressBar *progressBar;
    QTimer *expirationTimer;

    void setupLayout(const QString &text, const QIcon &icon, const QColor &textColor, const QColor &borderColor);
};

#endif // MESSAGE_H
