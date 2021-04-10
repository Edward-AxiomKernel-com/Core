#ifndef Settings_h__
#define Settings_h__

#include <qglobal.h>
#include <QSettings>
#include<exception>

#include <QColor>
#include <QVariant>

//#include "CxApp.h"
//#include "RuntimeException.h"

enum class DefaultValue { Visible, Hidden };

class Settings : public QSettings
{
	Q_OBJECT

// See http://blog.qt.io/blog/2014/06/13/qt-weekly-13-qstringliteral/ for discussion of the benefits of QStringLiteral.
// TL;DR: QStringLiteral speeds up the creation of QStrings from literals, at the expense of bigger binary sizes.
// **********  Do not use for empty strings - use QString(), avoid declaring the same literal in multiple places.
#define QSL QStringLiteral

public:
    class XNoOrgOrProgramNameException : public std::exception { public: XNoOrgOrProgramNameException() : std::exception() {} };

	static void setOrganizationAndProgramName(QString organizationName, QString programName);
	static QString getOrganizationName();
	static QString getProgramName();

	static QString defaultOptionsPath();
	static QString defaultOptionsFilePathName();
	static QString defaultMachineOptionsFilePathName();

	virtual ~Settings() = default;

	Settings(Settings const&) = delete;
	Settings(Settings&&) = delete;
	Settings& operator=(Settings const&) = delete;
	Settings& operator=(Settings&&) = delete;

	Settings(QString group, QSettings::Scope scope);
	Settings(QString group, QString filePathName = defaultOptionsFilePathName());

	void restoreDefaultValues();

signals:
	void resetRequested();

protected:
	static void deleteSetting(QString settingsKey); // skipped when saving
	static QString s_organizationName;
	static QString s_programName;
};


// ComboSettings allows to store different Setting objects in different places.
// One Setting for example can be stored in the registry, another in a file.
class ComboSettings : public QObject
{
	Q_OBJECT

public: 
	enum class Scope { File, User, System };

	ComboSettings(QString group, QString filePathName = Settings::defaultOptionsFilePathName());

	void setValue(const QString& key, const QVariant& value, ComboSettings::Scope scope);
	QVariant value(const QString& key, ComboSettings::Scope scope, const QVariant& defaultValue = QVariant()) const;

	void sync();
	void clear();

	void restoreDefaultValues();

signals:
	void resetRequested();

private:
	std::unique_ptr<Settings> m_fileSettings;
	std::unique_ptr<Settings> m_registryUserSettings;
	std::unique_ptr<Settings> m_registrySystemSettings;

	QString m_group;
};



template <class T>
class Setting : public QObject
{

public:
	Setting(Settings* settingsObject = nullptr, QString name = "", T defaultValue = T(), DefaultValue visibilityMode = DefaultValue::Visible)
        : m_settings(settingsObject), m_settingName(name), m_defaultValue(defaultValue), m_value(defaultValue)
	{
		if (m_settings)
		{
            m_value = QVariant(m_settings->value(m_settingName, m_defaultValue)).value<T>();

			if (visibilityMode == DefaultValue::Visible)
				m_settings->setValue(m_settingName, m_value);

			// this connect to a non-slot function works as the new syntax uses function pointers and not names like the old one.
			connect(m_settings, &Settings::resetRequested, this, &Setting::resetToDefault);
		}
	}

	Setting(ComboSettings* settingsObject = nullptr, ComboSettings::Scope scope = ComboSettings::Scope::File, QString name = "", T defaultValue = T(), DefaultValue visibilityMode = DefaultValue::Visible)
		: m_comboSettings(settingsObject), m_scope(scope), m_settingName(name), m_value(defaultValue), m_defaultValue(defaultValue)
	{
		if (m_comboSettings)
		{	
            m_value = QVariant(m_comboSettings->value(m_settingName, m_scope, m_defaultValue)).value<T>();

			if(visibilityMode == DefaultValue::Visible)
				m_comboSettings->setValue(m_settingName, m_value, m_scope);

			// this connect to a non-slot function works as the new syntax uses function pointers and not names like the old one.
			connect(m_comboSettings, &ComboSettings::resetRequested, this, &Setting::resetToDefault);
		}		
	}

    Setting& operator=(const Setting& other)
    {
        m_value = other.m_value;

        if (m_settings)
            m_settings->setValue(m_settingName, m_value);
		else if (m_comboSettings)
			m_comboSettings->setValue(m_settingName, m_value, m_scope);

        return *this;
    }

	inline T value() { return m_value; }
	inline T defaultValue() { return m_defaultValue; }

	inline operator T() { return value(); }
	
	inline void operator= (T value)
	{
		m_value = value;

		if (m_settings)
			m_settings->setValue(m_settingName, m_value);
		else if (m_comboSettings)
			m_comboSettings->setValue(m_settingName, m_value, m_scope);
	}

	void resetToDefault()
	{
		m_value = m_defaultValue;

		if(m_settings)
			m_settings->setValue(m_settingName, m_value);
		else if (m_comboSettings)
			m_comboSettings->setValue(m_settingName, m_value, m_scope);
	}

private:
	Settings* m_settings = nullptr; // Should never have a pointer to m_settings and m_comboSettings!
	ComboSettings* m_comboSettings = nullptr; 
	ComboSettings::Scope m_scope;
    QString m_settingName;
	T m_defaultValue;
	T m_value;
};


using StringSetting = Setting<QString>;
using IntSetting = Setting<int>;
using BoolSetting = Setting<bool>;
using ByteArraySetting = Setting<QByteArray>;
using ColorSetting = Setting<QColor>;

#endif // Settings_h__
