#ifndef DownloadManager_h__
#define DownloadManager_h__

#include <deque>
#include <map>
#include <memory>

#include <QtNetwork>
#include <QtCore>

#include "RuntimeException.h"

class NetworkTransferManager : public QNetworkAccessManager
{
	class XNetworkTransferError : public RuntimeException { public: XNetworkTransferError(QString message, QUuid errorCode) : RuntimeException(message, errorCode) {} };

	Q_OBJECT

public:
	NetworkTransferManager();
	void queueDownloadFromFtp(const QString& targetFolder, const QString& ftpAddress, const QString& ftpUsername = "anonymous", const QString& ftpPassword = "", const uint ftpPort = 21);
	void queueUploadToFtp(const QString& filePathName, const QString& ftpAddress, const QString& ftpUsername = "anonymous", const QString& ftpPassword = "", const uint ftpPort = 21);

signals:
	void transferFinished(QString filename);
	void transferProgressChanged(QString filename, double progressInPercent); // emits -100 if the progress is unknown

private slots:
	void onTransferFinished(bool errorsOccured = false);
	void onTransferError(QNetworkReply::NetworkError code);
	void onReadyRead();
	void onTransferProgressChanged(qint64 bytesSent, qint64 bytesTotal);
	
private:
	void processQueue();

	enum class TransferMode : int {FtpUpload, FtpDownload};

	struct TransferObject
	{
		TransferMode transferMode;
		QString localAddress;
		QString ftpAddress;
		QString ftpUsername;
		QString ftpPassword;
		uint ftpPort;
	};

	std::deque<TransferObject> m_transferQueue;
	std::unique_ptr<QFile> m_currentFile;
	QNetworkReply* m_currentReply = nullptr;
};

#endif // DownloadManager_h__
