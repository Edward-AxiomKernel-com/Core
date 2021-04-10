#include "FileTool.h"

#include <QDir>
#include <QMessageBox>

#include "CxApp.h"
#include "StringHelper.h"

QString FileTool::getErrorActionString(ErrorAction errorAction)
{
	switch (errorAction)
	{
	case FileTool::ErrorAction::SkipOperand:
		return operandWillBeSkippedText();
		break;
	case FileTool::ErrorAction::CancelOperation:
		return operationWillBeCanceledText();
		break;
	case FileTool::ErrorAction::NoInfo:
	default:
		return QString();
		break;
	}
}

QString FileTool::createErrorMessage(QString errorText, ErrorAction errorAction, QString customErrorInfoText)
{
	return QString(errorText + " " + customErrorInfoText + "\n" + getErrorActionString(errorAction));
}

QString FileTool::pathSanitizer(QString path)
{
	QString partlyPath = path.right(path.length() - 2); // skip the first two chars because it could be a server address

	while (partlyPath.contains("//"))
		partlyPath.replace("//", "/");

	return path.left(2) + partlyPath;
}

void FileTool::checkIfFileExists(const QString filePathName, const ErrorAction errorAction, const QUuid errorCode, const QString customErrorInfoText)
{ 
	QString filePathNameSanitized = FileTool::pathSanitizer(filePathName);

	if (!QFile(filePathNameSanitized).exists())
		throw XFilesystemAccessError(createErrorMessage(tr("File %1 does not exist.")
			.arg(StringHelper::fixSlashes(filePathNameSanitized)), errorAction, customErrorInfoText), errorCode);
}

void FileTool::checkIfFilesExist(const QStringList filePathNames, const ErrorAction errorAction, const QString customErrorInfoText) 
{
	QStringList tempFileList = filePathNames;
	QStringList missingFileList;

	removeMissingFiles(tempFileList, missingFileList);

	if (missingFileList.isEmpty())
		return;

	QString errorText;
	if (missingFileList.size() == 1)
		errorText = tr("1 file could not be found:\n");
	else
		errorText = tr("%1 files could not be found:\n").arg(missingFileList.size());

	int maxFilesToPrint = 5;
	int fileCounter = 0;
	for (auto file : missingFileList)
	{
		errorText.append(StringHelper::fixSlashes(file) + "\n");

		if (++fileCounter > maxFilesToPrint)
		{
			errorText.append(tr("[%1 other files]").arg(missingFileList.size() - fileCounter));
			break;
		}
	}
	// #TODO: Repair with Edward
	//	throw XFilesystemAccessError(createErrorMessage(errorText, errorAction, customErrorInfoText));
}

void FileTool::removeMissingFiles(QStringList& filePathNames, QStringList& missingFileList)
{
	for (int i = 0; i < filePathNames.size(); ++i)
	{
		QString currentFilePath = filePathNames[i];
		QString filePathNameSanitized = FileTool::pathSanitizer(currentFilePath);

		if (!QFile(filePathNameSanitized).exists())
		{
			filePathNames.removeAll(currentFilePath);
			missingFileList.append(filePathNameSanitized);
			--i;
		}	
	}
}

void FileTool::checkIfDirExists(const QString directoryName, const ErrorAction errorAction, const QUuid errorCode, const QString customErrorInfoText)
{
	QString dirNameSanitized = FileTool::pathSanitizer(directoryName);

	if (!QDir(dirNameSanitized).exists())
		throw XFilesystemAccessError(createErrorMessage(tr("Directory %1 does not exist.")
			.arg(StringHelper::fixSlashes(dirNameSanitized)), errorAction, customErrorInfoText), errorCode);

}

void FileTool::createSubfolder(const QString baseDirectory, const QString subFolderName, ErrorAction errorAction, const QUuid errorCode, const QString customErrorInfoText)
{
	QString directory = baseDirectory + subFolderName;
	if (!QDir(directory).exists())
	{
		if (!QDir().mkdir(directory))
			throw XFilesystemAccessError(createErrorMessage(
				tr("Could not create subdirectory in %1. Please ensure that you have read and write permissions.").arg(baseDirectory), 
				errorAction, customErrorInfoText), errorCode);
	}
}
