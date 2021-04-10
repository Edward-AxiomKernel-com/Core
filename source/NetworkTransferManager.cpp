#include "NetworkTransferManager.h"

#include "FileHelper.h"
#include "StringHelper.h"

NetworkTransferManager::NetworkTransferManager()
{
	connect(this, &QNetworkAccessManager::finished, this, &NetworkTransferManager::onTransferFinished);
}

void NetworkTransferManager::queueDownloadFromFtp(const QString& targetFolder, const QString& ftpAddress, const QString& ftpUsername, const QString& ftpPassword, const uint ftpPort)
{
	if (m_currentReply) // connection busy
	{
		m_transferQueue.push_back({ TransferMode::FtpDownload, targetFolder, ftpAddress, ftpUsername, ftpPassword, ftpPort });
		return;
	}

	QUrl url(ftpAddress);
	url.setUserName(ftpUsername);
	url.setPassword(ftpPassword);
	url.setPort(ftpPort);

	QString filename = url.fileName().right(StringHelper::fixSlashes(url.fileName()).lastIndexOf("/"));
	m_currentFile = std::make_unique<QFile>(targetFolder + QDir::separator() + url.fileName());

	if (m_currentFile->open(QIODevice::WriteOnly))
	{
		QNetworkRequest request(url);
		request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
		
		m_currentReply = this->get(request);

		connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &NetworkTransferManager::onTransferError);
		connect(m_currentReply, &QNetworkReply::readyRead, this, [&]() { this->onReadyRead(); });
		connect(m_currentReply, &QNetworkReply::downloadProgress, this, &NetworkTransferManager::onTransferProgressChanged);
	}
	else
	{
		throw XNetworkTransferError(tr("Could not open %1 for writing.\nPlease make sure that you have write permissions or an existing file is not write protected.").arg(filename), QUuid("{99D07010-7340-49B5-8FD8-154D63977973}"));
	}
}

void NetworkTransferManager::queueUploadToFtp(const QString& filePathName, const QString& ftpAddress, const QString& ftpUsername, const QString& ftpPassword, const uint ftpPort)
{
	if (m_currentReply) // connection busy
	{
		m_transferQueue.push_back({ TransferMode::FtpUpload, filePathName, ftpAddress, ftpUsername, ftpPassword, ftpPort });
		return;
	}

	QFileInfo fileInfo(filePathName);
	QUrl url(ftpAddress + fileInfo.fileName());
	url.setUserName(ftpUsername);   
	url.setPassword(ftpPassword); 
	url.setPort(ftpPort);

	m_currentFile = std::make_unique<QFile>(filePathName);

	if (m_currentFile->open(QIODevice::ReadOnly))
	{
		m_currentReply = this->put(QNetworkRequest(url), m_currentFile.get());
		connect(m_currentReply, &QNetworkReply::uploadProgress, this, &NetworkTransferManager::onTransferProgressChanged);
		connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &NetworkTransferManager::onTransferError);
	}
	else
	{
		throw XNetworkTransferError(tr("Could not read %1. Please make sure that the path is correct and you have read permissions.").arg(filePathName), QUuid("{D9AD82ED-9897-4684-AD31-B3472EB5D4E8}"));
	}
}

void NetworkTransferManager::processQueue()
{
	if (m_transferQueue.size() == 0 || m_currentReply != nullptr)
		return;

	auto transferObject = m_transferQueue.front();
	m_transferQueue.pop_front();

	switch (transferObject.transferMode)
	{
	case TransferMode::FtpDownload:
		queueDownloadFromFtp(transferObject.localAddress, transferObject.ftpAddress,
			transferObject.ftpUsername, transferObject.ftpPassword, transferObject.ftpPort);
		break;
	case TransferMode::FtpUpload:
		queueUploadToFtp(transferObject.localAddress, transferObject.ftpAddress,
			transferObject.ftpUsername, transferObject.ftpPassword, transferObject.ftpPort);
		break;
	}
}

void NetworkTransferManager::onTransferProgressChanged(qint64 bytesSent, qint64 bytesTotal)
{
	double progressInPercent = bytesTotal > 0 ? 100 * bytesSent / bytesTotal : -100;
	emit transferProgressChanged(m_currentFile->fileName(), progressInPercent);
}

void NetworkTransferManager::onReadyRead()
{
	if (m_currentFile->exists())
		m_currentFile->write(m_currentReply->readAll());
}

void NetworkTransferManager::onTransferFinished(bool errorsOccured)
{
	m_currentFile->flush();
	m_currentFile->close();
	m_currentFile->deleteLater(); // #UNCLEAR do we need this? Especially with unique_ptr?
	m_currentReply->deleteLater(); // #UNCLEAR
	m_currentReply = nullptr;

	if(!errorsOccured)
		emit transferFinished(m_currentFile->fileName());

	processQueue();
}

void NetworkTransferManager::onTransferError(QNetworkReply::NetworkError code)
{
	onTransferFinished(true); // try to continue with the next file before we throw an error.

	switch (code)
	{
	case QNetworkReply::ConnectionRefusedError:
		throw XNetworkTransferError(tr("The connection to the server was refused."), QUuid("{BD530AC7-66CE-4327-8FC8-4703332268CD}"));
	case QNetworkReply::RemoteHostClosedError:
		throw XNetworkTransferError(tr("The remote host seems to be closed."), QUuid("{CC8AE03F-490F-4EE8-87C8-DF99624ECD95}"));
	case QNetworkReply::HostNotFoundError:
		throw XNetworkTransferError(tr("Host could not be found."), QUuid("{73EA09BC-3B26-41F0-9C9E-CBBA150D62FF}"));
	case QNetworkReply::TimeoutError:
		throw XNetworkTransferError(tr("The connection timed out."), QUuid("{5566F713-0B7E-4EC4-B8F8-C6B3AE02F4E7}"));
	case QNetworkReply::OperationCanceledError:
		throw XNetworkTransferError(tr("The operation was canceled."), QUuid("{40775DA8-6C44-4979-8295-4246D3690768}"));
	case QNetworkReply::SslHandshakeFailedError:
		throw XNetworkTransferError(tr("SSL handshake failed."), QUuid("{DBF11DA0-FD75-461E-A255-51900D624B09}"));
	case QNetworkReply::TemporaryNetworkFailureError:
		throw XNetworkTransferError(tr("A temporary network failure occurred."), QUuid("{B5709BEA-40DA-4316-B7BC-D136C921B099}"));
	case QNetworkReply::NetworkSessionFailedError:
		throw XNetworkTransferError(tr("The network session failed."), QUuid("{87EE26BF-8EAB-4D89-B525-7CF5EB9F9EEE}"));
	case QNetworkReply::BackgroundRequestNotAllowedError:
		throw XNetworkTransferError(tr("Background request was not allowed"), QUuid("{4E9F1561-38E7-45F8-9929-A9A77AA6D971}"));
	case QNetworkReply::TooManyRedirectsError:
		throw XNetworkTransferError(tr("Too many redirects."), QUuid("{0446A819-A13A-423C-9161-9C0178F40411}"));
	case QNetworkReply::InsecureRedirectError:
		throw XNetworkTransferError(tr("An insecure redirect occurred."), QUuid("{E856F5C1-1C9F-49E4-930F-247FF2B09F87}"));
	case QNetworkReply::UnknownNetworkError:
		throw XNetworkTransferError(tr("An unknown network error occurred."), QUuid("{861FBC79-9BD0-4318-91AC-8497C4D4094C}"));
	case QNetworkReply::ProxyConnectionRefusedError:
		throw XNetworkTransferError(tr("The proxy connection was refused."), QUuid("{F2EA962C-BF75-4EBF-AEAE-21D5F78974C7}"));
	case QNetworkReply::ProxyConnectionClosedError:
		throw XNetworkTransferError(tr("The proxy connection was closed."), QUuid("{8CAC45EE-4EAD-4FD8-AAA8-16F5111C70E0}"));
	case QNetworkReply::ProxyNotFoundError:
		throw XNetworkTransferError(tr("The proxy was not found."), QUuid("{89DB4153-D120-4EAF-A30C-9CF36710E6C8}"));
	case QNetworkReply::ProxyTimeoutError:
		throw XNetworkTransferError(tr("The proxy connection timed out."), QUuid("{A3172D57-56FE-4B3F-B24E-2602EA79A4DF}"));
	case QNetworkReply::ProxyAuthenticationRequiredError:
		throw XNetworkTransferError(tr("The proxy server requires authentication."), QUuid("{865CAA06-114A-4907-8E44-7EC37B49D4C4}"));
	case QNetworkReply::UnknownProxyError:
		throw XNetworkTransferError(tr("An unknown proxy server error occurred."), QUuid("{65942C76-8FAB-4568-A78B-DE89B7B7BC9D}"));
	case QNetworkReply::ContentAccessDenied:
		throw XNetworkTransferError(tr("The access to the desired content on the server was denied."), QUuid("{5C4A77DC-DCD6-485D-8A3F-A399031EA2E4}"));
	case QNetworkReply::ContentOperationNotPermittedError:
		throw XNetworkTransferError(tr("The operation on the server was not permitted."), QUuid("{68E7EA94-F2E4-4C1B-B8B0-9D52F2DF1133}"));
	case QNetworkReply::ContentNotFoundError:
		throw XNetworkTransferError(tr("The content could not be found on the server."), QUuid("{19F2181D-0A76-4718-B534-E990FEEADE1A}"));
	case QNetworkReply::AuthenticationRequiredError:
		throw XNetworkTransferError(tr("The login data was not correct."), QUuid("{EBBEFE96-AFEB-4EE0-87E8-F4D6413837C2}"));
	case QNetworkReply::ContentReSendError:
		throw XNetworkTransferError(tr("The content could not be resend."), QUuid("{6FFE6643-C761-4B48-A9F5-855A801A7EDD}"));
	case QNetworkReply::ContentConflictError:
		throw XNetworkTransferError(tr("A content conflict occurred."), QUuid("{76E05A58-417D-447E-BBF7-16CF722424EB}"));
	case QNetworkReply::ContentGoneError:
		throw XNetworkTransferError(tr("The content on the server is gone."), QUuid("{CA7F2765-BF3D-47DA-A79A-8AC2A0322BD9}"));
	case QNetworkReply::UnknownContentError:
		throw XNetworkTransferError(tr("An unknown content error occurred."), QUuid("{4B018EE7-90B5-4DA0-BC5B-8E347FCE4B35}"));
	case QNetworkReply::ProtocolUnknownError:
		throw XNetworkTransferError(tr("The protocol is unknown."), QUuid("{F46856D1-8EB5-4935-B347-9EBC0DB98815}"));
	case QNetworkReply::ProtocolInvalidOperationError:
		throw XNetworkTransferError(tr("An invalid protocol operation occurred."), QUuid("{349674F2-2C4C-4AE9-BC2D-CE132A5947CE}"));
	case QNetworkReply::ProtocolFailure:
		throw XNetworkTransferError(tr("A protocol failure occurred."), QUuid("{249E9B83-1B0A-4608-8FE5-436FFEB81D09}"));
	case QNetworkReply::InternalServerError:
		throw XNetworkTransferError(tr("An internal server error occurred."), QUuid("{4E30159D-5503-4499-8BA6-17F1258D287C}"));
	case QNetworkReply::OperationNotImplementedError:
		throw XNetworkTransferError(tr("The desired operation is not implemented."), QUuid("{ABBB2F3F-17CB-4BA9-AD39-B304CEA6E29B}"));
	case QNetworkReply::ServiceUnavailableError:
		throw XNetworkTransferError(tr("The service is unavailable."), QUuid("{8D258A31-7AF0-4716-98B0-F756D2CFE98D}"));
	case QNetworkReply::UnknownServerError:
		throw XNetworkTransferError(tr("An unknown server error occurred."), QUuid("{EFBD4667-5701-423D-BB05-578867D52D8D}"));
	}
}
