#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QQueue>
#include "Message.h"

class MessageQueue : public QWidget {
    Q_OBJECT

public:
    static MessageQueue* instance(QWidget *parent = nullptr); // Accept optional parent

    void addInfo(const QString &text);
    void addWarning(const QString &text);
    void addError(const QString &text);
  Message* addProgressMessage(const QString &text);


private:
    explicit MessageQueue(QWidget *parent = nullptr); // Pass parent to constructor
    Message* addMessage(const QString &text, MessageType msgType, const QColor &textColor,
                        const QColor &borderColor, int timeoutMs, const QIcon &icon);
    void showMessage(Message *message);

private slots:
    void onMessageExpired(Message *message);

private:
    static MessageQueue *m_instance;
    QVBoxLayout *m_layout;
    QQueue<Message *> m_messageQueue;
    const int m_maxMessages = 8;
};

#endif // MESSAGEQUEUE_H
