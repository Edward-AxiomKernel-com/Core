#include "PushNotifierClient.h"

#include <QAuthenticator>
#include <QDataStream>
#include <QNetworkProxy>

#include "NotificationConnectionHandler.h"
#include "NotifierSettings.h"

#ifdef _DEBUG
constexpr int kRetrySeconds = 5;
#else
constexpr int kRetrySeconds = 60;
#endif
constexpr int kMillisecondsPerSecond = 1000;
constexpr int kRetryDelayMilliseconds = kRetrySeconds * kMillisecondsPerSecond;

PushNotifierClient::PushNotifierClient(QObject* parent)
    : QTcpSocket(parent),
    m_receivedHello(false),
    m_messageBodySize(-1),
    m_messageReader(m_messageBodySize)
{
    connect(this, &QAbstractSocket::stateChanged, this, &PushNotifierClient::onStateChanged);
    connect(this, &QAbstractSocket::disconnected, this, &PushNotifierClient::onDisconnected);
    connect(this, qOverload<QAbstractSocket::SocketError>(&QAbstractSocket::error), this, &PushNotifierClient::onError);
    connect(this, &QAbstractSocket::proxyAuthenticationRequired, this, &PushNotifierClient::onProxyAuthenticationRequired);
    connect(this, &QIODevice::readyRead, this, &PushNotifierClient::onReadyRead);

    m_retryTimer.setSingleShot(true);
    m_retryTimer.setInterval(kRetryDelayMilliseconds);
    connect(&m_retryTimer, &QTimer::timeout, this, &PushNotifierClient::onRetryTimerExpired);

    connect(&m_messageReader, &IpMessageReader::progressMessage, this, &PushNotifierClient::message);
    connect(&m_messageReader, &IpMessageReader::ipMessageReady, this, &PushNotifierClient::handleIpMessage);

    QTimer::singleShot(0, this, [this]() { this->connectToServer(); });
}

PushNotifierClient::~PushNotifierClient()
{
    m_retryTimer.stop();
    this->abort();
}

void PushNotifierClient::disconnectFromHost()
{
    if (NotifierSettings::getInstance()->verbosity > 3)
        emit message(tr("disconnectFromHost while in %1 state.").arg(this->stateString()));

    m_retryTimer.stop();
    this->QTcpSocket::disconnectFromHost();
}

void PushNotifierClient::onStateChanged(QAbstractSocket::SocketState socketState)
{
    emit stateChanged(stateName(socketState));

    if (NotifierSettings::getInstance()->verbosity > 0)
        emit message(stateName(socketState));

    if (QAbstractSocket::UnconnectedState == socketState)
    {
        m_receivedHello = false;

        if (NotifierSettings::getInstance()->useNotifier)
        {
            if (NotifierSettings::getInstance()->verbosity > 1)
                emit message(QString("Disconnected: about to retry."));


            if (NotifierSettings::getInstance()->verbosity > 0)
                emit message(tr("Waiting %1 seconds to try reconnecting").arg(kRetrySeconds));

            m_retryTimer.start();
        }
    }
}

void PushNotifierClient::onRetryTimerExpired()
{
    if (NotifierSettings::getInstance()->verbosity > 3)
        this->message(tr("About to connect from PushNotifierClient::onRetryTimerExpired (after %1 seconds).").arg(kRetrySeconds));
    this->connectToServer();
}

void PushNotifierClient::connectToServer()
{
    if (!NotifierSettings::getInstance()->useNotifier)
        return;

    if (QAbstractSocket::UnconnectedState != this->state())
    {
        if (NotifierSettings::getInstance()->verbosity > 3)
            emit message(tr("Call to connectToServer while not in %1 state. Aborting attempt.").arg(stateName(QAbstractSocket::UnconnectedState)));
        return;
    }

    if (NotifierSettings::getInstance()->verbosity > -1)
        emit message(tr("Connecting to %1, port %2").arg(NotifierSettings::getInstance()->ipAddress).arg(NotifierSettings::getInstance()->port));

    if (NotifierSettings::getInstance()->verbosity > 3)
    {
        qDebug() << tr("Connecting to server while in %1 state.").arg(this->stateString());
        emit message(tr("Connecting to server while in %1 state.").arg(this->stateString()));
        emit message(tr("Message body size: %1.").arg(m_messageBodySize));
    }

    m_messageReader.reset();
    this->connectToHost(NotifierSettings::getInstance()->ipAddress, NotifierSettings::getInstance()->port);
}

QString PushNotifierClient::stateString()
{
    return stateName(this->state());
}

void PushNotifierClient::send(std::unique_ptr<IpMessage> ipMessage)
{
	if (!NotifierSettings::getInstance()->useNotifier)
		return;

    if (NotifierSettings::getInstance()->verbosity > 2)
    {
        QString messageType = typeid(*ipMessage).name();
        messageType = messageType.remove("class ");
        emit message(tr("Sending %1 %2").arg(messageType, ipMessage->toString()));
    }

    QByteArray block = ipMessage->prepare();
    this->write(block);
}

void PushNotifierClient::onDisconnected()
{
    if (NotifierSettings::getInstance()->verbosity > 1)
        emit message(QString("Disconnected"));
}

void PushNotifierClient::onError(QAbstractSocket::SocketError socketError)
{
    emit message(QString("Error: %1").arg(PushNotifierClient::getErrorData(socketError)));
}
 
// We will eventually need to support proxies.
void PushNotifierClient::onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
    emit message(QString("Proxy Authentication Required"));
}

void PushNotifierClient::onReadyRead()
{
    if (m_receivedHello)
    {
        m_messageReader.read(this);
    }
    else
    {
        if (this->canReadLine())
        {
            QTextStream stream(this);
            QString data = stream.readLine();

            if (NotifierSettings::getInstance()->verbosity > 2)
                emit message(data);

            if (NotifierSettings::getInstance()->verbosity > 3)
                emit message(QString("bytes available after readLine(): %1").arg(this->QIODevice::bytesAvailable()));

            if (data != NotificationConnectionHandler::protocolVersionString())
                emit messageProtocolMismatch(data);
        }
        else if (NotifierSettings::getInstance()->verbosity > 3)
        {   
            emit message(tr("readyRead but not canReadLine. %1 bytes available.").arg(this->QIODevice::bytesAvailable()));
        }

        m_receivedHello = true;
    }
}

void PushNotifierClient::handleIpMessage(IpMessage& ipMessage)
{
    // IpMessageReader cannot emit a signal for the actual type of message, since it only ever has a base class pointer. The reader can call a virtual function
    // on the base class pointer, that in the derived type can emit the signal for that kind of IP message. The virtual function is emitIpMessageReady().
    // First connect all the message signals to the PushNotifierClient because we don't know which one will be used, then call the virtual function.

    connect(&ipMessage, qOverload<LockItemMessage&>(&IpMessage::ipMessageReady), this, &PushNotifierClient::lockItem);
    connect(&ipMessage, qOverload<DeleteItemMessage&>(&IpMessage::ipMessageReady), this, &PushNotifierClient::deleteItem);
    connect(&ipMessage, qOverload<InsertItemMessage&>(&IpMessage::ipMessageReady), this, &PushNotifierClient::insertItem);
    connect(&ipMessage, qOverload<UpdateItemMessage&>(&IpMessage::ipMessageReady), this, &PushNotifierClient::updateItem);
	connect(&ipMessage, qOverload<MoveItemMessage&>(&IpMessage::ipMessageReady), this, &PushNotifierClient::moveItem);

    ipMessage.emitIpMessageReady();
}

// static
const QString PushNotifierClient::stateName(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
    case QAbstractSocket::UnconnectedState: return tr("Unconnected");
    case QAbstractSocket::HostLookupState: return tr("Host Lookup");
    case QAbstractSocket::ConnectingState: return tr("Connecting");
    case QAbstractSocket::ConnectedState: return tr("Connected");
    case QAbstractSocket::BoundState: return tr("Bound");
    case QAbstractSocket::ListeningState: return tr("Listening");
    case QAbstractSocket::ClosingState: return tr("Closing");
    default:
        return tr("Unknown");
    }
}

// static
QString PushNotifierClient::getErrorData(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::ConnectionRefusedError:
        return tr("ConnectionRefusedError: The connection was refused by the peer(or timed out).");
    case QAbstractSocket::RemoteHostClosedError:
        return tr("RemoteHostClosedError: The remote host closed the connection.Note that the client socket(i.e., this socket) will be closed after the remote close notification has been sent.");
    case QAbstractSocket::HostNotFoundError:
        return tr("HostNotFoundError: The host address was not found.");
    case QAbstractSocket::SocketAccessError:
        return tr("SocketAccessError: The socket operation failed because the application lacked the required privileges.");
    case QAbstractSocket::SocketResourceError:
        return tr("SocketResourceError: The local system ran out of resources(e.g., too many sockets).");
    case QAbstractSocket::SocketTimeoutError:
        return tr("SocketTimeoutError: The socket operation timed out.");
    case QAbstractSocket::DatagramTooLargeError:
        return tr("DatagramTooLargeError: The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).");
    case QAbstractSocket::NetworkError:
        return tr("NetworkError: An error occurred with the network(e.g., the network cable was accidentally plugged out).");
    case QAbstractSocket::AddressInUseError:
        return tr("AddressInUseError: The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.");
    case QAbstractSocket::SocketAddressNotAvailableError:
        return tr("SocketAddressNotAvailableError: The address specified to QAbstractSocket::bind() does not belong to the host.");
    case QAbstractSocket::UnsupportedSocketOperationError:
        return tr("UnsupportedSocketOperationError: The requested socket operation is not supported by the local operating system(e.g., lack of IPv6 support).");
    case QAbstractSocket::UnfinishedSocketOperationError:
        return tr("UnfinishedSocketOperationError: Used by QAbstractSocketEngine only, The last operation attempted has not finished yet(still in progress in the background).");
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        return tr("ProxyAuthenticationRequiredError: The socket is using a proxy, and the proxy requires authentication.");
    case QAbstractSocket::SslHandshakeFailedError:
        return tr("SslHandshakeFailedError: The SSL / TLS handshake failed, so the connection was closed(only used in QSslSocket)");
    case QAbstractSocket::ProxyConnectionRefusedError:
        return tr("ProxyConnectionRefusedError: Could not contact the proxy server because the connection to that server was denied");
    case QAbstractSocket::ProxyConnectionClosedError:
        return tr("ProxyConnectionClosedError: The connection to the proxy server was closed unexpectedly(before the connection to the final peer was established)");
    case QAbstractSocket::ProxyConnectionTimeoutError:
        return tr("ProxyConnectionTimeoutError: The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.");
    case QAbstractSocket::ProxyNotFoundError:
        return tr("ProxyNotFoundError: The proxy address set with setProxy() (or the application proxy) was not found.");
    case QAbstractSocket::ProxyProtocolError:
        return tr("ProxyProtocolError: The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.");
    case QAbstractSocket::OperationError:
        return tr("OperationError: An operation was attempted while the socket was in a state that did not permit it.");
    case QAbstractSocket::SslInternalError:
        return tr("SslInternalError: The SSL library being used reported an internal error.This is probably the result of a bad installation or misconfiguration of the library.");
    case QAbstractSocket::SslInvalidUserDataError:
        return tr(":SslInvalidUserDataError: Invalid data(certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.");
    case QAbstractSocket::TemporaryError:
        return tr("TemporaryError: A temporary error occurred(e.g., operation would block and socket is non - blocking).");
    case QAbstractSocket::UnknownSocketError:
        return tr("UnknownSocketError: An unidentified error occurred.");
    default:
        return tr("Unknown error type.");
    }
}
