#include "LogMessage.h"

#include <memory>

#if TARGET_OS_WIN32
#pragma warning(push, 1)
#endif

#include <QDateTime>
#include <QDebug>
#include <QMutex>
#include <QTextStream>

#include "GlobalPaths.h"

#if TARGET_OS_WIN32
#pragma warning(pop)
#endif

static std::unique_ptr<LogMessage> sLogMessagesInstance;

#if defined(QT_THREAD_SUPPORT)
static QMutex sMutex;
#endif

QString LogMessage::s_applicationName;
static bool sServiceMode = false;

LogMessage* LogMessage::get()
{
    if (NULL == sLogMessagesInstance.get())
        sLogMessagesInstance = std::unique_ptr<LogMessage>(new LogMessage());

    return sLogMessagesInstance.get();
}

void LogMessage::send(const QString text, const LogMode mode)
{
#if defined(QT_THREAD_SUPPORT)
    QMutexLocker locker(&sMutex);
#endif

    LogMessage::get()->writeLogMessage(text, true, mode);
}

void LogMessage::initialize(const QString& appName, const QString& version, const bool serviceMode, const QString& additionalText)
{
    s_applicationName = appName;
    sServiceMode = serviceMode;

    if (!serviceMode)
        return;

    QString startupMessage;

    QTextStream textStream(&startupMessage, QIODevice::WriteOnly);

    textStream << "----------------------------------------" << Qt::endl;
    textStream << "Program started:\t" << appName << Qt::endl;
    textStream << "Version:\t\t" << version << Qt::endl;
    textStream << "Start date:\t\t" << QDate::currentDate().toString("dd.MM.yyyy") << Qt::endl;
    textStream << "Start time:\t\t" << QTime::currentTime().toString("hh:mm:ss:zzz") << Qt::endl;

    if (!additionalText.isEmpty())
    {
        textStream << Qt::endl << additionalText << Qt::endl;
    }

    textStream << "----------------------------------------" << Qt::endl;

    LogMessage::get()->writeLogMessage(startupMessage, false);
}

void LogMessage::connectWithMessageSink(QObject* messageSink)
{
	// #TODO: We need an interface class for messageSinks that provides addText so we can get rid of this slot!
    connect(LogMessage::get(), SIGNAL(signalLogMessage(const QString&, LogMessage::MessageType)), messageSink, SLOT(addText(const QString&, LogMessage::MessageType)));
}

LogMessage::LogMessage()
{
    QFile logFile(this->getLogFileDirectory().absoluteFilePath("%1 DebugLog.txt").arg(s_applicationName));

    qDebug() << "logFile: " << logFile.fileName();

    if (logFile.exists()) // create a new one each time the app runs
        logFile.remove();

    m_logFile.setFileName(logFile.fileName());
}

void LogMessage::writeLogMessage(const QString& logMessage, const bool useTimeStamp, const LogMode mode)
{
    qDebug().noquote() << logMessage;

	if (mode != LogMode::AlwaysLog)
	{
		if (!sServiceMode)
			return;
	}

    writeToLogFile(useTimeStamp, logMessage);
}

void LogMessage::writeToLogFile(const bool useTimeStamp, const QString& logMessage)
{
    if (m_logFile.open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text))
    {
        if (useTimeStamp)
            QTextStream(&m_logFile) << QTime::currentTime().toString("hh:mm:ss:zzz") << " | " << logMessage << Qt::endl;
        else
            QTextStream(&m_logFile) << "             | " << logMessage << Qt::endl;

        m_logFile.close();
    }
    else
    {
#ifdef _DEBUG
        qDebug() << QString("Can not open log file %1!").arg(m_logFile.fileName());
        throw XLogOpenException(m_logFile.fileName(), "{CF7993CD-FEC5-4056-8D09-396DABBED6FA}");
#endif // _DEBUG
    }
}

QDir LogMessage::getLogFileDirectory()
{
    return GlobalPaths::logPath();
}

void LogMessage::setServiceMode(const bool serviceMode)
{
    sServiceMode = serviceMode;
}

void LogMessage::printStack(QString text, const LogMode mode)
{
    if (mode != LogMode::AlwaysLog)
    {
        if (!sServiceMode)
            return;
    }

    text = text.simplified();

    LogMessage::get()->writeToLogFile(false, text);
}
