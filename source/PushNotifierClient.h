#ifndef PushNotifierClient_h__
#define PushNotifierClient_h__

#include <QTcpSocket>
#include <QTimer>

#include "IpMessage.h"
#include "IpMessageReader.h"

class QAuthenticator;
class QNetworkProxy;

class PushNotifierClient : public QTcpSocket
{
    Q_OBJECT

public:
    PushNotifierClient(QObject* parent);
    ~PushNotifierClient();

    void disconnectFromHost();
    void connectToServer();
    QString  stateString();

    void send(std::unique_ptr<IpMessage> message);

signals:
    void stateChanged(const QString& state);
    void message(const QString message);
    void messageProtocolMismatch(const QString message);
    void lockItem(LockItemMessage& message);
    void deleteItem(DeleteItemMessage& message);
    void insertItem(InsertItemMessage& message);
    void updateItem(UpdateItemMessage& message);
    void moveItem(MoveItemMessage& message);

private slots:
    void onStateChanged(QAbstractSocket::SocketState socketState);
    void onRetryTimerExpired();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onProxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);

    void onReadyRead();
    void handleIpMessage(IpMessage& message);

private:
    static const QString stateName(QAbstractSocket::SocketState socketState);
    static QString getErrorData(QAbstractSocket::SocketError socketError);

    bool m_receivedHello = false;
    QTimer m_retryTimer;
    IpMessage::BodySize m_messageBodySize;
    IpMessageReader m_messageReader;
};

#endif // PushNotifierClient_h__
