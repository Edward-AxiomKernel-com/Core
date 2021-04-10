#ifndef RuntimeException_h__
#define RuntimeException_h__

#include <stdexcept>

#include <QString>
#include <QUuid>

class RuntimeException : public std::runtime_error
{
public:
	RuntimeException(const QString what, const QUuid errorReference); // #TODO: we need a function to change the what() string

    QUuid getErrorReference() const;

protected:
    QUuid m_errorReference;
};

#endif // RuntimeException_h__