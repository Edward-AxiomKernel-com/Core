#include "RuntimeException.h"

#include "LogMessage.h"

// #IDEA Maybe there is a way to use nested exceptions like here: https://stackoverflow.com/questions/348833/how-to-know-the-exact-line-of-code-where-where-an-exception-has-been-caused
// but couldn't get it till now because of https://stackoverflow.com/questions/25324582/stdthrow-with-nested-expects-default-constructor-for-virtual-base-class-of-exc
// but yeah, find a possibility... Also interesting: https://stackoverflow.com/questions/3026649/getting-information-about-where-c-exceptions-are-thrown-inside-of-catch-block

// Anyway, we should get rid of QUuid and print a file and line
// for the customer this could be encrypted to give us a error number 
// or even better add a function to submit the bug in the window!

RuntimeException::RuntimeException(const QString what, const QUuid errorReference)
	: std::runtime_error(qPrintable(what))
    , m_errorReference(errorReference)
{
	LogMessage::send("Exception: " + what + " (ErrorCode: " + errorReference.toString() + ")");
}

QUuid RuntimeException::getErrorReference() const
{
    return m_errorReference;
}