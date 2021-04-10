#include "IpMessageReader.h"

#include <QDataStream>
#include <QHostAddress>
#include <QTcpSocket>

#include "LogMessage.h"
#include "NotifierSettings.h"

void IpMessageReader::read(QTcpSocket* m_clientConnection)
{
    QDataStream stream(m_clientConnection);
    stream.setByteOrder(QDataStream::BigEndian);

    do
    {
        QString message = QString("IpMessageReader::readMessage: bytesAvailable: %1").arg(m_clientConnection->bytesAvailable());
        if (NotifierSettings::getInstance()->verbosity > 2)
            emit progressMessage(message);

        if (!this->isMessageComplete(stream, m_clientConnection->bytesAvailable(), m_messageBodySize, m_clientConnection))
        {
            QString message = "Returning from IpMessageReader::readMessage because message not complete.";
            if (NotifierSettings::getInstance()->verbosity > 2)
                emit progressMessage(message);
            return;
        }

        this->processMessage(stream, m_messageBodySize, m_clientConnection);
        m_messageBodySize = -1; // Reinitialize
    } while (!stream.atEnd());
}

void IpMessageReader::reset()
{
    m_messageBodySize = -1;
}

bool IpMessageReader::isMessageComplete(QDataStream& stream, uint bytesAvailable, IpMessage::BodySize& messageBodySize, QTcpSocket* clientConnection)
{
    static constexpr uint kMessageBodySizeSize = static_cast<uint>(sizeof(IpMessage::BodySize));
    static constexpr uint kMessageTypeSize = static_cast<uint>(sizeof(IpMessage::MessageType));

    uint headerSizeStillNeeded = (-1 == messageBodySize) ? kMessageBodySizeSize + kMessageTypeSize : kMessageTypeSize;
    if (bytesAvailable < headerSizeStillNeeded)
        return false;

    if (-1 == messageBodySize)
        stream >> messageBodySize; // You can only read messageBodySize once

    headerSizeStillNeeded = (-1 == messageBodySize) ? kMessageBodySizeSize + kMessageTypeSize : kMessageTypeSize;
    QString message = QString("IpMessageReader::isMessageComplete: bytesAvailable: %1, still need: %2: %3+%4+%5").
        arg(clientConnection->bytesAvailable()).
        arg(headerSizeStillNeeded + messageBodySize).
        arg((-1 == messageBodySize) ? kMessageBodySizeSize : 0).
        arg(kMessageTypeSize).
        arg(messageBodySize);

    if (NotifierSettings::getInstance()->verbosity > 2)
        emit progressMessage(message);

    if (bytesAvailable < headerSizeStillNeeded + messageBodySize)
        return false;

    return true;
}

void IpMessageReader::processMessage(QDataStream& stream, IpMessage::BodySize& messageBodySize, QTcpSocket* clientConnection)
{
    IpMessage* ipMessage = IpMessage::messageFactory(stream);

	this->sendStatusMessages(ipMessage, clientConnection);

    if (ipMessage != nullptr)
    {
        emit ipMessageReady(*ipMessage); // This can only emit ipMessageReady(IpMessage*), not with the actual type. NotifierSocketServer uses this.
//         ipMessage->emitIpMessageReady(); // This virtual function is overridden, so the ipMessage emits the signal with the actual message type, e.g. ipMessageReady(DeleteItemMessage*).
    }

    delete ipMessage;
}

void IpMessageReader::sendStatusMessages(IpMessage* ipMessage, QTcpSocket* clientConnection)
{
	if (NotifierSettings::getInstance()->verbosity > 1)
	{
		if (ipMessage != nullptr)
		{
			QString sender = clientConnection->peerName();
			if (sender.isEmpty())
				sender = clientConnection->peerAddress().toString();
			QString messageTypeName = typeid(*ipMessage).name();
			messageTypeName = messageTypeName.remove("class ");

			QString messageString = ipMessage->toString();
			emit progressMessage(QString("Received %1 from %2: %3").arg(messageTypeName, sender, messageString));
			if (NotifierSettings::getInstance()->verbosity > 2)
				emit progressMessage(QString("bytesAvailable: %1").arg(clientConnection->bytesAvailable()));
		}
		else
		{
			emit progressMessage("IpMessageReader: unknown message type");
			if (NotifierSettings::getInstance()->verbosity > 2)
				emit progressMessage(QString("bytesAvailable: %1").arg(clientConnection->bytesAvailable()));
		}
	}
}
