#ifndef IpMessageReader_h__
#define IpMessageReader_h__

#include <QObject>

#include "IpMessage.h"

class QTcpSocket;

class IpMessageReader : public QObject
{
    Q_OBJECT
public:
    IpMessageReader(IpMessage::BodySize& messageBodySize) : m_messageBodySize(messageBodySize) {}
    void read(QTcpSocket* m_clientConnection);

    void reset();
signals:
    void progressMessage(const QString message);
    void ipMessageReady(IpMessage&);

private:
    bool isMessageComplete(QDataStream& stream, uint bytesAvailable, IpMessage::BodySize& messageBodySize, QTcpSocket* clientConnection);
    void processMessage(QDataStream& stream, IpMessage::BodySize& messageBodySize, QTcpSocket* clientConnection);
	void sendStatusMessages(IpMessage* ipMessage, QTcpSocket* clientConnection);

    IpMessage::BodySize& m_messageBodySize;
};


#endif // IpMessageReader_h__
