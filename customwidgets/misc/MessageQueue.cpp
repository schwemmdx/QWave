#include "MessageQueue.h"
#include "Message.h"
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

MessageQueue* MessageQueue::m_instance = nullptr;

MessageQueue::MessageQueue(QWidget *parent)
    : QWidget(parent), m_layout(new QVBoxLayout(this)) {
    
    setStyleSheet("background: transparent;");
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setContentsMargins(10, 10, 10, 10);
    m_layout->setAlignment(Qt::AlignBottom);
    setLayout(m_layout);

    // Adjust initial geometry if needed
    setGeometry(0, 0, 400, 600);
}

MessageQueue* MessageQueue::instance(QWidget *parent) {
    if (!m_instance) {
        m_instance = new MessageQueue(parent);
    }
    return m_instance;
}

void MessageQueue::addMessage(const QString &text, 
const QColor &textColor,const QColor &borderColor, int timeoutMs, const QIcon &icon) {
    // Create and initialize a new Message object
    Message *newMessage = new Message(text, icon, textColor, borderColor,timeoutMs, this);
    
    // Connect the message expiration signal
    connect(newMessage, &Message::expired, this, &MessageQueue::onMessageExpired);
    
    // Display the message and add to queue
    showMessage(newMessage);
}

void MessageQueue::addInfo(const QString &text)
{
    addMessage(text,Qt::black,Monokai::Blue,5000,QIcon(":/icons/materials/info.png"));
}

void MessageQueue::addWarning(const QString &text)
{
    addMessage(text,Qt::black,Monokai::Orange,5000,QIcon(":/icons/materials/095-warnung.png"));
}

void MessageQueue::addError(const QString &text)
{
    addMessage(text,Qt::black,Monokai::Red,5000,QIcon(":/icons/materials/023-error.png"));
}




void MessageQueue::showMessage(Message *message) {
    // Limit the number of visible messages
    if (m_messageQueue.size() >= m_maxMessages) {
        Message *oldMessage = m_messageQueue.dequeue();
        oldMessage->deleteLater();
    }

    m_messageQueue.enqueue(message);
    m_layout->addWidget(message);
}

void MessageQueue::onMessageExpired(Message *message) {
    // Remove message from layout and queue, then delete it
    m_layout->removeWidget(message);
    m_messageQueue.removeOne(message);
    message->deleteLater();
}


