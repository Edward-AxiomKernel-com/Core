#ifndef ConnectionHandler_h
#define ConnectionHandler_h

#include <QObject>
#include <QAbstractSocket>

class QTcpSocket;


/*
  The ConnectionHandler class provides an interface definition for encapsulation for a 
  socket that is connected with a client, handling client communication. For every client 
  that connects to the server, the server creates a new instance derived from this class.
*/
class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    ConnectionHandler(QTcpSocket* socket);
    virtual ~ConnectionHandler();
    void disconnectFromHost();

signals:
    void logText(const QString);
    void disconnected();
    void progressMessage(const QString message);
    void closing(QString ipAddress);

private slots:
    virtual void readMessage() = 0;
    void onStateChanged(QAbstractSocket::SocketState socketState);

protected:
    QTcpSocket* m_clientConnection;
};

#endif
