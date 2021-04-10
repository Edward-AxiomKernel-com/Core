#include "Pixmap.h"

#include <QBuffer>
#include <QImageReader>

Pixmap::Pixmap(const QString& filePathName)
	: m_filePathName(filePathName)
{
	m_pixmap = QPixmap(filePathName);
}

Pixmap::Pixmap(const Pixmap& other)
{
	m_pixmap = other.m_pixmap;
	m_filePathName = other.m_filePathName;
	m_rotation = other.m_rotation;
}

Pixmap::Pixmap(const QByteArray pictureData)
    : m_pictureData(pictureData)
{
    m_pixmap.loadFromData(pictureData);
}

Pixmap& Pixmap::operator=(const Pixmap& other)
{
	m_pixmap = other.m_pixmap;
	m_filePathName = other.m_filePathName;
	m_rotation = other.m_rotation;
    m_pictureData = other.m_pictureData;

	return *this;
}

QPixmap* Pixmap::getPixmap()
{
	return &m_pixmap;
}

Pixmap::operator QPixmap*()
{
	return getPixmap();
}

void Pixmap::reinit(const QString& filePathName)
{
	m_pixmap = QPixmap(filePathName);
	m_filePathName = filePathName;
	m_rotation = 0;
    m_pictureData = "";
}

void Pixmap::reloadPixmapData()
{
    if (m_filePathName != "")
        m_pixmap = QPixmap(m_filePathName);
    else if (m_pictureData != "")
        m_pixmap.loadFromData(m_pictureData);
}

QString Pixmap::getFilePathName() const
{
	return m_filePathName;
}

QByteArray Pixmap::toQByteArray(const Format& format, const int qualityInPercent) const
{
	QByteArray byteArray;

	QBuffer buffer(&byteArray);
	buffer.open(QIODevice::WriteOnly);
	QString fileformat = formatToString(format);
	m_pixmap.save(&buffer, fileformat.toLocal8Bit(), qualityInPercent);

	return byteArray;
}

bool Pixmap::isNull() const
{
	return m_pixmap.isNull();
}

QRect Pixmap::enclosingRectangle() const
{
	return m_pixmap.rect();
}

int Pixmap::height() const
{
	return m_pixmap.height();
}

int Pixmap::width() const
{
	return m_pixmap.width();
}

void Pixmap::setRotationTo(const AngleInDeg& rotation)
{
    if (rotation != m_rotation)
    {
        reloadPixmapData();
        m_pixmap = this->rotatedBy(rotation).m_pixmap;
    }
}

Pixmap Pixmap::rotatedBy(const AngleInDeg& rotation)
{
	if (rotation == 0)
		return *this;

	m_rotation = rotation; 

	QMatrix rotationMatrix;
	rotationMatrix.rotate(m_rotation * -1); // rotate in the mathematical way counterclockwise

	Pixmap rotatedPixmap(*this);
	rotatedPixmap.m_pixmap = m_pixmap.transformed(rotationMatrix);
	
	return rotatedPixmap;
}

void Pixmap::scale(const QSize newSize, const Qt::AspectRatioMode aspectMode, const Qt::TransformationMode transformationMode)
{
	scale(newSize.width(), newSize.height(), aspectMode, transformationMode);
}

void Pixmap::scale(const int width, const int heigth, const Qt::AspectRatioMode aspectMode, const Qt::TransformationMode transformationMode)
{
	m_pixmap = this->scaled(width, heigth, aspectMode).m_pixmap;
}

Pixmap Pixmap::scaled(const QSize newSize, const Qt::AspectRatioMode aspectMode, const Qt::TransformationMode transformationMode) const
{
	return scaled(newSize.width(), newSize.height(), aspectMode, transformationMode);
}

Pixmap Pixmap::scaled(const int width, const int heigth, const Qt::AspectRatioMode aspectMode, const Qt::TransformationMode transformationMode) const
{
	Pixmap scaledPixmap(*this);
	scaledPixmap.m_pixmap = m_pixmap.scaled(width, heigth, aspectMode);
	return scaledPixmap;
}

void Pixmap::setDevicePixelRatio(double scaleFactor)
{
	m_pixmap.setDevicePixelRatio(scaleFactor);
}

QString Pixmap::formatToString(const Format& format) const
{
	switch (format)
	{
	case Format::PNG:
		return QString("PNG");
	case Format::BMP:
		return QString("BMP");
	case Format::JPEG:
		return QString("JPEG");
	default:
		return QString("PNG");
	}
}
