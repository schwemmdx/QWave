#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QWidget>
#include <QQueue>
#include <QVBoxLayout>
#include <QTimer>
#include <QIcon>
#include <QColor>

class Message;

class MessageQueue : public QWidget {
    Q_OBJECT

public:
    static MessageQueue* instance(QWidget *parent = nullptr);
    void addMessage(const QString &text, const QColor &textColor, int timeoutMs = 5000, const QIcon &icon = QIcon());
    
private:
    explicit MessageQueue(QWidget *parent = nullptr);
    static MessageQueue *m_instance;
    
    QVBoxLayout *m_layout;
    QQueue<Message*> m_messageQueue;
    int m_maxMessages = 5;

    void showMessage(Message *message);

private slots:
    void onMessageExpired(Message *message);
};

#endif // MESSAGEQUEUE_H
