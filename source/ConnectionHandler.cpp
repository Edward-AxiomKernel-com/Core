#include "ConnectionHandler.h"

#include <QHostAddress>
#include <QTcpSocket>


ConnectionHandler::ConnectionHandler(QTcpSocket* socket)
:   QObject(NULL),
    m_clientConnection(socket)
{
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    connect(socket, &QTcpSocket::disconnected, this, &ConnectionHandler::deleteLater);
    connect(socket, &QTcpSocket::disconnected, this, &ConnectionHandler::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &ConnectionHandler::readMessage);
    connect(socket, &QTcpSocket::stateChanged, this, &ConnectionHandler::onStateChanged);
}

ConnectionHandler::~ConnectionHandler()
{
    // Nothing to do because on disconnect, the socket is deleted later.
}

void ConnectionHandler::disconnectFromHost()
{
    m_clientConnection->disconnectFromHost();
}

void ConnectionHandler::onStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::ClosingState)
    {
        QString peer = m_clientConnection->peerName();
        if (peer.isEmpty())
            peer = m_clientConnection->peerAddress().toString();

        emit progressMessage(tr("Closing connection with %1").arg(peer));
        emit closing(peer);
    }
}

