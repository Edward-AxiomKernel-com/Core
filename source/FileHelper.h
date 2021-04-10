#ifndef FileHelper_h
#define FileHelper_h

#include <qglobal.h>

#if TARGET_OS_WIN32
#pragma warning(push, 2) 
#endif

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QDataStream>

#if TARGET_OS_WIN32
#pragma warning(pop)

#pragma warning(push) 
#pragma warning(disable: 4290)
#endif

#include <memory>

#include <stdexcept>

namespace Core // #TODO: Should we use namespace core? Core should be easily accessible
{
	// #TODO: Move to FileTool
	class Json : public QObject
	{
	public:
        class XReadError : public std::runtime_error { public: XReadError(QString message) : std::runtime_error(message.toStdString()) {} };
        class XWriteError : public std::runtime_error { public: XWriteError(QString message) : std::runtime_error(message.toStdString()) {} };

		// RECOMMENDED: use a filename without a path to use the standard data folder
        static QJsonDocument loadJsonDocumentFromFile(const QString filename);

        // RECOMMENDED: use a filename without a path to use the standard data folder
        static void saveJsonObjectToFile(const QString filename, const QJsonObject& object);

	private: 
		static std::unique_ptr<QFile> getFile(const QString filename);
        static void convertToUnicode(QByteArray& data);
	};

    class FileHelper
    {
    public:
        class XFile 
        {
        public:
            XFile(QString filepath, QFile::FileError error = QFile::UnspecifiedError);
			QString errorText() { return errorText(m_error); }
			static QString errorText(QFile::FileError error)
			{
				switch (error)
				{
				case QFile::NoError: return qApp->translate("XFile", "No error occurred.");
				case QFile::ReadError: return qApp->translate("XFile", "An error occurred when reading from the file.");
				case QFile::WriteError: return qApp->translate("XFile", "An error occurred when writing to the file.");
				case QFile::FatalError: return qApp->translate("XFile", "A fatal error occurred.");
				case QFile::ResourceError: return qApp->translate("XFile", "Resource error.");
				case QFile::OpenError: return qApp->translate("XFile", "The file could not be opened.");
				case QFile::AbortError: return qApp->translate("XFile", "The operation was aborted.");
				case QFile::TimeOutError: return qApp->translate("XFile", "A timeout occurred.");
				case QFile::UnspecifiedError: return qApp->translate("XFile", "An unspecified error occurred.");
				case QFile::RemoveError: return qApp->translate("XFile", "The file could not be removed.");
				case QFile::PositionError: return qApp->translate("XFile", "The position in the file could not be changed.");
				case QFile::ResizeError: return qApp->translate("XFile", "The file could not be resized.");
				case QFile::PermissionsError: return qApp->translate("XFile", "The file could not be accessed.");
				case QFile::CopyError: return qApp->translate("XFile", "The file could not be copied.");
				default: return qApp->translate("XFile", "Unknown Error.");
				}
			}

			QString m_filepath;
			QFile::FileError m_error;
		};

		FileHelper(QString filePathName, QIODevice::OpenMode mode = QIODevice::ReadOnly);
		virtual ~FileHelper();

		QFile* operator->() { return &m_filePathName; }
		QFile& get() { return m_filePathName; }
		operator QFile&() { return m_filePathName; }
		operator QFile*() { return &m_filePathName; }

    private:
        QFile m_filePathName;
    };

    class TextStreamHelper
    {
    public:
        TextStreamHelper(QString filePathName, QIODevice::OpenMode mode = QIODevice::ReadOnly);
        virtual ~TextStreamHelper();

        QTextStream* operator->() { return &m_textStream; }
        QTextStream& get() { return m_textStream; }
		QTextStream& operator<< (const QString string) { m_textStream << string; return m_textStream; }
        operator QTextStream&() { return m_textStream; }
        operator QTextStream*() { return &m_textStream; }
        operator QFile&() { return m_fileHelper.get(); }
        operator QFile*() { return &m_fileHelper.get(); }

    private:
        FileHelper m_fileHelper;
        QTextStream m_textStream;
    };

    class DataStreamHelper
    {
    public:
        DataStreamHelper(QString fileName, QIODevice::OpenMode mode = QIODevice::ReadOnly);
        virtual ~DataStreamHelper();

        QDataStream* operator->() { return &m_dataStream; }
        QDataStream& get() { return m_dataStream; }
        operator QDataStream&() { return m_dataStream; }
        operator QDataStream*() { return &m_dataStream; }
        operator QFile&() { return m_fileHelper.get(); }
        operator QFile*() { return &m_fileHelper.get(); }

    private:
        FileHelper m_fileHelper;
        QDataStream m_dataStream;
    };

    class StreamPositionResetter
    {
    public:
        StreamPositionResetter(QTextStream& fileStream) 
        :   m_fileStream(fileStream), 
            m_streamPosition(fileStream.pos())
        {}

    	virtual ~StreamPositionResetter()
        {
            m_fileStream.seek(m_streamPosition);
        }

    protected:
        QTextStream& m_fileStream;
    	qint64 m_streamPosition;
    };
}

#if TARGET_OS_WIN32
#pragma warning(pop)
#endif

#endif

