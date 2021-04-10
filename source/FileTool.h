#ifndef FileTool_h__
#define FileTool_h__

#include <QObject>
#include <QString>

#include "RuntimeException.h"

class FileTool : public QObject
{
	Q_OBJECT

public:
	class XFilesystemAccessError : public RuntimeException { public: XFilesystemAccessError(QString message, QUuid errorCode) : RuntimeException(message, errorCode) {} };

	enum class ErrorAction : int { SkipOperand, CancelOperation, NoInfo };

	static QString getErrorActionString(const ErrorAction errorAction);
	static QString createErrorMessage(const QString errorText, const ErrorAction errorAction, const QString customInfoText);
	static QString pathSanitizer(const QString path);
	
	static void checkIfFileExists(const QString filePathName, const ErrorAction errorAction, const QUuid errorCode, const QString customErrorInfoText = QString());
	static void checkIfFilesExist(const QStringList filePathNames, const ErrorAction errorAction, const QString customErrorInfoText = QString());
	static void checkIfDirExists(const QString directoryName, const ErrorAction errorAction, const QUuid errorCode, const QString customErrorInfoText = QString());

	static void createSubfolder(const QString baseDirectory, const QString subFolderName, ErrorAction errorAction, const QUuid errorCode, const QString customInfoTest = QString());

	static void removeMissingFiles(QStringList& filePathNames, QStringList& missingFiles);

private:
	static QString operandWillBeSkippedText() { return tr("This will be skipped."); }
	static QString operationWillBeCanceledText() { return tr("The operation will be canceled."); }
};

#endif // FileTool_h__
