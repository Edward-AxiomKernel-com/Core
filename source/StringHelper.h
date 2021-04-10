#ifndef StringHelper_h__
#define StringHelper_h__

#include "windows.h"

#include <QString>

class StringHelper
{
public:
	static inline LPCTSTR toLPCTSTR(const QString inputString)
	{
		return (LPCTSTR)inputString.utf16();
	}

    // #Consider using QDir::toNativeSeparators() instead.
	static inline QString fixSlashes(const QString inputString)
	{
		QString inputStringCopy = inputString;
		QString tempString = inputStringCopy.replace("/","\\");

		QString partlyPath = tempString.right(tempString.length() - 2); // skip the first two chars because it could be a server address

		while (partlyPath.contains("\\\\"))
			partlyPath.replace("\\\\", "\\");

		return tempString.left(2) + partlyPath;
	}
};

#endif // StringHelper_h__
