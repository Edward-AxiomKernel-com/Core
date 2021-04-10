#ifndef CharArray_h__
#define CharArray_h__

#include <QByteArray>
#include <QString>

class CharArray
{
public:
	inline CharArray(QString string) : m_data(string.toLocal8Bit()) {};
	inline char* getCharArray() { return m_data.data(); }
	inline const char* getConstCharArray() { return m_data.constData(); }
	inline operator char* () { return m_data.data(); }
	inline operator const char* () { return m_data.constData(); }

private:
	QByteArray m_data;
};

#endif // CharArray_h__
