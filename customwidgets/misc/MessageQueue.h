#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <QWidget>
#include <QQueue>
#include <QVBoxLayout>
#include <QTimer>
#include <QIcon>
#include <QColor>

#include "ThemeColors.h"

class Message;

class MessageQueue : public QWidget {
    Q_OBJECT

public:
    static MessageQueue* instance(QWidget *parent = nullptr);
    void addMessage(const QString &text, const QColor &textColor = Monokai::Foreground, 
    const QColor &borderColor=Monokai::Foreground,
    int timeoutMs = 5000, const QIcon &icon = QIcon());

    void addInfo(const QString &text);
    void addWarning(const QString &text);
    void addError(const QString &text);
    
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
