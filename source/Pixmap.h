#ifndef Pixmap_h__
#define Pixmap_h__

#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>

#include "CxDatabase.h"
#include "Element.h"
#include "FileHelper.h"
#include "Units.h"

class Pixmap
{
public:
	enum class Format : int { PNG, JPEG, BMP };

	Pixmap() = default;
	Pixmap(const QString& filePathName);
	Pixmap(const Pixmap& other);
    Pixmap(const QByteArray data);
	
	Pixmap& operator=(const Pixmap& other);

	operator QPixmap*();
	QPixmap* getPixmap();

	void reinit(const QString& filePathName);
	void reloadPixmapData();

	QString getFilePathName() const;
	QByteArray toQByteArray(const Format& format = Format::PNG, const int qualityInPercent = 90) const;

	bool isNull() const;
	QRect enclosingRectangle() const;
	int height() const;
	int width() const;

	void setRotationTo(const AngleInDeg& rotation);
	[[nodiscard]] Pixmap rotatedBy(const AngleInDeg& rotation);
	void scale(const QSize newSize, const Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, const Qt::TransformationMode transformationMode = Qt::SmoothTransformation);
	void scale(const int width, const int heigth, const Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, const Qt::TransformationMode transformationMode = Qt::SmoothTransformation);
	[[nodiscard]] Pixmap scaled(const QSize newSize, const Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, const Qt::TransformationMode transformationMode = Qt::SmoothTransformation) const;
	[[nodiscard]] Pixmap scaled(const int width, const int heigth, const Qt::AspectRatioMode aspectMode = Qt::KeepAspectRatio, const Qt::TransformationMode transformationMode = Qt::SmoothTransformation) const;

	void setDevicePixelRatio(double scaleFactor);

private:
	QString formatToString(const Format& format) const;

	QPixmap m_pixmap;
	QString m_filePathName = "";
	AngleInDeg m_rotation = 0;
    QByteArray m_pictureData = "";
};

class DatabaseImage : public Model::Element
{
public:
	class XDatabaseError : public RuntimeException { public: XDatabaseError(QString message, QUuid errorCode) : RuntimeException(message, errorCode) {} };

	DatabaseImage()
		: Element("Name", tr(""), "", QSqlDatabase::database("Blob"))
		, p_imageData(this, "Image", tr("Image"), QVariant(), Core::Property::Mode::UseDatabase)
		, p_foreignId(this, "ForeignId", tr("Foreign Id"), QVariant(), Core::Property::Mode::UseDatabase)
		, p_description(this, "Description", tr("Description of the picture"), QVariant(), Core::Property::Mode::UseDatabase)
	{
	}

	DatabaseImage(const DatabaseImage& other)
		: Element(other)
		, p_imageData(this, other.p_imageData)
		, p_foreignId(this, other.p_foreignId)
		, p_description(this, other.p_description)
	{

		p_foreignId = QVariant();

		if (!p_imageData.isValid())
			this->setDirty(false);
	}

	~DatabaseImage() = default;
    std::unique_ptr<ModelItem> clone([[maybe_unused]] ModelItem::UniqueNameMode uniqueNameMode = ModelItem::UniqueNameMode::Generate) const override
    {
        return std::make_unique<DatabaseImage>(*this);
    }

	operator Pixmap* () { return getPixmap(); };
	Pixmap* getPixmap() 
	{ 
		m_pixmap = Pixmap(p_imageData);
		return &m_pixmap;
	};

	virtual void DatabaseImage::load(QSqlRecord& record) override 
	{ 
		Model::ElementSignalBlocker block(this);
		this->Element::load(record); 
	}

	virtual bool DatabaseImage::save() override { return this->Element::save(); }
	virtual QString tableName() const override { return m_tableName; }
	virtual QString typeName() const override { return "Image"; }
	virtual QString DatabaseImage::uniqueFilter() const override { return QString(""); }

	virtual void deleteOnCommit(const bool del)
	{
		Element::deleteOnCommit(del);

		if (del)
			p_imageData = QByteArray();
	}

	Core::PropertyT<QByteArray> p_imageData;
	Core::PropertyT<int> p_foreignId;
	Core::PropertyT<QString> p_description;

	void setTableName(const QString tableName) { m_tableName = tableName; }

	void setImageBy(const QString filePathName)
	{
		Core::FileHelper file(filePathName, QIODevice::ReadOnly);
		p_imageData = file.get().readAll();
	}

	void loadImageByForeignId(const int foreignId)
	{
		bool wasDirty = false;
		if (this->isDirty())
			wasDirty = true;
		
		p_foreignId = foreignId;
		this->setDirty(wasDirty);

		if (!p_imageData.isValid() && p_foreignId.isValid() && !this->isDeleteSet())
			loadImage();
	}

	void forceImageReload()
	{
		loadImage();
	}

	void clearImage()
	{
		p_imageData = QByteArray();
	}

private:
	Pixmap m_pixmap;
	QString m_tableName = "MacroImage";

	void loadImage()
	{
		QSqlQuery query(QString("SELECT * FROM %1 WHERE ForeignId = '%2'").arg(m_tableName).arg(p_foreignId), QSqlDatabase::database("Blob"));
		::handleDatabaseErrors(query);

		query.next();

		if (query.isValid())
			this->load(query.record());
	}
};

#endif // Pixmap_h__