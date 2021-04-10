#include "NotificationConnectionHandler.h"

#include <QDataStream>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTextStream>
#include <typeinfo>

#include "IpMessage.h"
#include "IpMessageReader.h"
#include "LogMessage.h"
#include "NotifierSettings.h"

const QString kHelloMessage = QString("camProx Cx2Suite Push Notification Communications Protocol %1");
const char* kProtocolVersion = "v1.1";


NotificationConnectionHandler::NotificationConnectionHandler(QTcpSocket* socket)
:   ConnectionHandler(socket),
	m_messageBodySize(-1),
    m_messageReader(m_messageBodySize)
{
    connect(&m_messageReader, &IpMessageReader::progressMessage, this, &ConnectionHandler::progressMessage);
    connect(&m_messageReader, &IpMessageReader::ipMessageReady, this, &NotificationConnectionHandler::handleIpMessage);

    emit progressMessage("NotificationConnectionHandler: sending hello message");

    QTextStream ts(m_clientConnection);
    ts << kHelloMessage.arg(kProtocolVersion) << "\n";
}

const char* NotificationConnectionHandler::protocolVersion()
{
    return kProtocolVersion;
}

const QString NotificationConnectionHandler::protocolVersionString()
{
    return kHelloMessage.arg(kProtocolVersion);
}

void NotificationConnectionHandler::readMessage()
{
    m_messageReader.read(m_clientConnection);
}

void NotificationConnectionHandler::handleIpMessage(IpMessage& ipMessage)
{
    emit forwardMessage(this, ipMessage);
}

void NotificationConnectionHandler::send(IpMessage& message)
{
    if (NotifierSettings::getInstance()->verbosity > 2)
    {
        QString sender = m_clientConnection->peerName();
        if (sender.isEmpty())
            sender = m_clientConnection->peerAddress().toString();
        emit progressMessage(QString("Sending to %1").arg(sender));
    }

    QByteArray block = message.prepare();
    m_clientConnection->write(block);
}
