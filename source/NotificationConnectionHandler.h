#ifndef SolverConnectionHandler_h
#define SolverConnectionHandler_h

#include "ConnectionHandler.h"
#include "IpMessage.h"
#include "IpMessageReader.h"

class QTcpSocket;

class NotificationConnectionHandler : public ConnectionHandler
{
    Q_OBJECT
public:
    NotificationConnectionHandler(QTcpSocket* socket);
    ~NotificationConnectionHandler() = default;

    static const char* protocolVersion();
    static const QString protocolVersionString();
    void send(IpMessage& message);

signals:
    void forwardMessage(NotificationConnectionHandler* receiver, IpMessage& message);
       
private slots:
    virtual void readMessage() override;
    void handleIpMessage(IpMessage& ipMessage);

private:

	IpMessage::BodySize m_messageBodySize;
    IpMessageReader m_messageReader;
};

#endif
