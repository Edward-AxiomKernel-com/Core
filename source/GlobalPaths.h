#ifndef GlobalPaths_h__
#define GlobalPaths_h__

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSharedMemory>

#include "App.h"

namespace
{
	const QString kLogFolder = "/log/";
	const QString kTempLogFolder = "/temp/";
	const QString kConfigFolder = "/config/";
	const QString kDataFolder = "/data/";	
	const QString kDebugFolder = "/debug/";
}

class GlobalPaths
{
public:
	static inline QString logPath(){ return baseDataPath() + kLogFolder; }
	static inline QString tempPath(){ return baseDataPath() + kTempLogFolder; }
	static inline QString configPath(){ return baseDataPath() + kConfigFolder; }
	static inline QString dataPath(){ return baseDataPath() + kDataFolder; }
	static inline QString debugPath(){ return baseDataPath() + kDebugFolder; }
	static inline QString baseDataPath() { return QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + qApp->organizationName() + "/" + qApp->applicationName()); }

	static inline QString exePath(){ return qApp->applicationDirPath(); }

	static bool initializeDirectories()
	{
		if (!otherInstanceRunning()) // do not delete anything if another instance is currently running to avoid deleting its files
			QDir(baseDataPath() + kTempLogFolder).removeRecursively(); // otherwise make sure we have a fresh temp dir on every start.

		bool success = true;
		success &= QDir().mkpath(baseDataPath() + kLogFolder);
		success &= QDir().mkpath(baseDataPath() + kTempLogFolder);
		success &= QDir().mkpath(baseDataPath() + kConfigFolder);
		success &= QDir().mkpath(baseDataPath() + kDataFolder);
		success &= QDir().mkpath(baseDataPath() + kDebugFolder);

		return !success;
	}

private:
	static bool otherInstanceRunning()
	{
		QString memoryName = qApp->organizationName() + "_" + qApp->applicationName() + "_Shared_Mem";
		static QSharedMemory sharedMem(memoryName);
		if (sharedMem.create(8, QSharedMemory::ReadWrite)) // will fail if another instance already allocated the memory with the memoryName
			return false;
		else
			return true;
	}
};

#endif // GlobalPaths_h__
