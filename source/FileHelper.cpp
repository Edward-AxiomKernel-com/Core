#include "FileHelper.h"

#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QTextCodec>

//#include "GlobalPaths.h"

#if TARGET_OS_WIN32
#pragma warning(push)
#pragma warning(disable: 4290)
#endif

namespace
{
	const QString kJsonFileExtension(".json");
}

namespace Core
{

    FileHelper::FileHelper(QString fileName, QIODevice::OpenMode mode)
		: m_filePathName(fileName)
    {
        if (!m_filePathName.open(mode))
            throw XFile(fileName, m_filePathName.error());
    }

    FileHelper::~FileHelper()
    {
        m_filePathName.close();
    }


	FileHelper::XFile::XFile(QString filepath, QFile::FileError error /*= QFile::UnspecifiedError*/) 
		: m_filepath(filepath), m_error(error)
	{
	}

    TextStreamHelper::TextStreamHelper(QString fileName, QIODevice::OpenMode mode)
		: m_fileHelper(fileName, mode | QIODevice::Text),
		m_textStream(m_fileHelper)
    {
    }

    TextStreamHelper::~TextStreamHelper()
    {
    }


    DataStreamHelper::DataStreamHelper(QString fileName, QIODevice::OpenMode mode)
		: m_fileHelper(fileName, mode),
        m_dataStream(m_fileHelper)
    {
    }

    DataStreamHelper::~DataStreamHelper()
    {
    }

    QJsonDocument Json::loadJsonDocumentFromFile(const QString filename)
    {
        TextStreamHelper stream(filename);

//		std::unique_ptr<QFile> file = getFile(filename);

//		if (!file->open(QIODevice::ReadOnly))
//			throw XReadError(tr("Could not open %1 for reading.").arg(filename));

//		QByteArray data = (file->exists() ? file->readAll() : QByteArray());

//        convertToUnicode(data);

        return QJsonDocument::fromJson(stream->readAll().toLocal8Bit());
	}

    void Json::saveJsonObjectToFile(const QString filename, const QJsonObject& object)
	{
        QJsonDocument document(object);
        TextStreamHelper stream(filename);
        stream << QString(document.toJson());

//		std::unique_ptr<QFile> file = getFile(filename);

//		if (!file->open(QIODevice::WriteOnly))
//			throw XWriteError(tr("Could not open %1 for writing.").arg(filename));
		
//		QJsonDocument document(object);
		
//		file->write(document.toJson());
	}

//	std::unique_ptr<QFile> Json::getFile(const QString filename)
//	{
//		QString tempFilename = filename;

//		if (!tempFilename.endsWith(kJsonFileExtension, Qt::CaseInsensitive))
//			tempFilename += kJsonFileExtension;

//		if (tempFilename.contains("\\") || tempFilename.contains("/"))
//			return std::make_unique<QFile>(tempFilename);
//		else // we don't have a path specified so this means we use the standard data path for json files
//			return std::make_unique<QFile>(GlobalPaths::dataPath() + tempFilename);
//	}

//    void Json::convertToUnicode(QByteArray& data)
//    {
//        QTextCodec* codec = QTextCodec::codecForUtfText(data);
//        QString testString = codec->toUnicode(data);
//        data = testString.toUtf8();
//    }
}

#if TARGET_OS_WIN32
#pragma warning(pop)
#endif
