#include "MessageQueue.h"
#include "ThemeColors.h"

MessageQueue* MessageQueue::m_instance = nullptr;

MessageQueue::MessageQueue(QWidget *parent)
    : QWidget(parent), m_layout(new QVBoxLayout(this)) {
    // Set up as a child of the parent
    setWindowFlags(Qt::FramelessWindowHint); // No need for Qt::Tool or WindowStaysOnTopHint
    setAttribute(Qt::WA_TranslucentBackground);

    // Style and layout
    setStyleSheet("background: transparent;");
    setContentsMargins(5, 5, 5, 5);
    setFixedWidth(400);
    setFixedHeight(600);
    m_layout->setAlignment(Qt::AlignBottom);
    setLayout(m_layout);
}

MessageQueue* MessageQueue::instance(QWidget *parent) {
    if (!m_instance) {
        m_instance = new MessageQueue(parent); // Use the provided parent
    }
    return m_instance;
}

void MessageQueue::addInfo(const QString &text) {
    addMessage(text, MSG_VOLATILE, Monokai::PrimaryLabel, Monokai::SecondaryLabel, 2500, QIcon(":materials/dark_theme/info.png"));
}

void MessageQueue::addWarning(const QString &text) {
    addMessage(text, MSG_VOLATILE, Monokai::PrimaryLabel, Monokai::WarningColor, 3000, QIcon(":materials/dark_theme/warning.png"));
}

void MessageQueue::addError(const QString &text) {
    addMessage(text, MSG_PERSISTANT, Monokai::PrimaryLabel, Monokai::ErrorColor, 0, QIcon(":materials/dark_theme/error.png"));
}
Message* MessageQueue::addProgressMessage(const QString &text) {
    // Create a progress message
    Message *progressMessage = addMessage(text, MSG_PROGRESS, Monokai::PrimaryLabel, Monokai::SecondaryLabel, 1000, QIcon(":materials/dark_theme/progress.png"));

    // Show the message
    showMessage(progressMessage);

    return progressMessage; // Return the progress message
}

Message* MessageQueue::addMessage(const QString &text, MessageType msgType, const QColor &textColor,
                                   const QColor &borderColor, int timeoutMs, const QIcon &icon) {
    Message *newMessage = new Message(text, msgType, icon, textColor, borderColor, timeoutMs, this);
    connect(newMessage, &Message::expired, this, &MessageQueue::onMessageExpired);

    // Display the message in the queue
    showMessage(newMessage);
    return newMessage;
}

void MessageQueue::showMessage(Message *message) {
    if (m_messageQueue.size() >= m_maxMessages) {
        Message *oldMessage = m_messageQueue.dequeue();
        oldMessage->deleteLater();
    }

    m_messageQueue.enqueue(message);
    m_layout->addWidget(message);
}

void MessageQueue::onMessageExpired(Message *message) {
    m_layout->removeWidget(message);
    m_messageQueue.removeOne(message);
    message->deleteLater();
}
