#ifndef Cryptographer_h__
#define Cryptographer_h__

#include <QByteArray>
#include <QString>

class Cryptographer
{
public:
    static QByteArray encryptFromPasswordAES(const QString text);
    static QString decryptFromPasswordAES(const QByteArray text);
};

#endif // Cryptographer_h__