#ifndef LogMessage_h
#define LogMessage_h

#include <qglobal.h>

#if TARGET_OS_WIN32
#pragma warning(push, 2) 
#endif

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QObject>

#if TARGET_OS_WIN32
#pragma warning(pop) 
#endif

#include "TrString.h"

class LogMessage : public QObject
{
    Q_OBJECT

public:
    class XReadError : public std::runtime_error { public: XReadError(QString message) : std::runtime_error(message.toStdString()) {} };

    class XNoLogDirException : public std::runtime_error
	{
	public:
        XNoLogDirException(QString directoryPathName)
            : std::runtime_error(tr("Could not find the log dir %1.").arg(directoryPathName).toStdString()),
			m_directoryPathName(directoryPathName)
		{}

		QString m_directoryPathName;
	};

    class XLogOpenException : public std::runtime_error
	{
	public:
        XLogOpenException(QString filePathName)
            : std::runtime_error(tr("Could not open the log %1.").arg(filePathName).toStdString()),
			m_filePathName(filePathName)
		{}

		QString m_filePathName;
	};

	enum class LogMode : int { AlwaysLog, OnlyDebug }; // #TODO Remove this. Use always MessageType!
    enum class MessageType : int { Info, Warning, Error, Debug, Timing };

	static void send(const QString text, const LogMode mode = LogMode::AlwaysLog);

	template <typename T>
    static void sendUserMessage(const TrString<T> logMessage, const MessageType messageType = MessageType::Info)
	{
		send(logMessage.native());

		emit LogMessage::get()->signalLogMessage(logMessage.native(), messageType);
		QCoreApplication::processEvents();
	}

    static LogMessage* get();
    static void initialize(const QString& appName, const QString& version, const bool serviceMode = false, const QString& additionalText = "");
    static void connectWithMessageSink(QObject* messageSink);
    static void setServiceMode(const bool serviceMode);

    static void printStack(QString text, const LogMode mode);

signals:
    void signalLogMessage(const QString& logMessage, const LogMessage::MessageType messageType = MessageType::Info);

protected:
    LogMessage();
    void writeLogMessage(const QString& logMessage, const bool useTimeStamp = true, const LogMode mode = LogMode::OnlyDebug);
    void writeToLogFile(const bool useTimeStamp, const QString& logMessage);

    QDir getLogFileDirectory();

private:
    static QString s_applicationName;
    QFile m_logFile;
};

#endif
