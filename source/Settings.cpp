#include "Settings.h"

#include <QApplication>
#include <QTextCodec>

#include "GlobalPaths.h"

#include <QApplication>
#include <QJsonDocument>
#include <QDebug>
#include <QVariant>
#include <QSettings>

static QStringList s_deletedSettings; // will be skipped during output

static void readYamlFile(QString& yamlString, QSettings::SettingsMap& map)
{
    QStringList entries = yamlString.split(QRegExp("[\r\n]"), Qt::SkipEmptyParts); // Will split on newlines no matter which platform. Works with LF or CR,LF line endings.

	QStringList groupList;

	for (int i = 0; i < entries.size(); ++i)
	{
		auto entry = entries[i];
//		auto nextEntry = entries[i + 1 < entries.size() ? i + 1 : i];

		QString trimmedEntry = entry.trimmed();

		if (trimmedEntry.startsWith("#")) // ignore comments
			continue;

		int spaceCount = entry.indexOf(QRegularExpression("[^\\s]"), 0); // position of first non whitespace char

		if (spaceCount < 0)
			continue;

		int currentLevel = spaceCount / 2;

		while (currentLevel < groupList.size() && !groupList.isEmpty())
			groupList.pop_back();

		QString key = trimmedEntry.left(trimmedEntry.indexOf(":"));
		groupList.push_back(key);

		if (!entry.endsWith(":"))
		{
			QString value = entry.mid(entry.indexOf(": ") + 2);

			bool okayDouble = false;
			double doubleVal = value.toDouble(&okayDouble);
			bool okayInt = false;
			int intVal = value.toInt(&okayInt);

			if (doubleVal)
				map[groupList.join("/")] = doubleVal;
			else if (okayInt)
				map[groupList.join("/")] = intVal;
			else if (value.toLower() == "true")
				map[groupList.join("/")] = true;
			else if (value.toLower() == "false")
				map[groupList.join("/")] = false;
			else
				map[groupList.join("/")] = value;

			groupList.pop_back();
		}
	}
}

static void applyUpdateSettingsYaml(QSettings::SettingsMap& map)
{
	QString yamlUpdateFilePathName(GlobalPaths::exePath() + "/updateSettings.yaml");
	QFile yamlUpdateFile(yamlUpdateFilePathName);
	
	if (yamlUpdateFile.exists())
	{
		if (yamlUpdateFile.open(QIODevice::ReadOnly))
		{
			QTextStream inputStream(&yamlUpdateFile);
			QString updateData = inputStream.readAll();

			QSettings::SettingsMap updateMap;
			readYamlFile(updateData, updateMap);

			for (auto [key, value] : updateMap.toStdMap())
				map[key] = value; // #TODO write this to log file.

			yamlUpdateFile.close();
			yamlUpdateFile.remove();
//			if (yamlUpdateFile.remove())
//				throw Settings::UpdateSettingsException();
		}
		else
		{
//			throw Settings::UpdateSettingsException();
		}
	}
}

static void applyUpdateSettingsRegistry()
{
	QString regUpdateFilePathName(GlobalPaths::exePath() + "/updateSettings.reg");
	QFile regUpdateFile(regUpdateFilePathName);

	if (regUpdateFile.exists())
	{
		// Signal a message box here in order to inform the user about the upcoming administrator rights dialog
		// write the content of the reg file to the log file. 
		int returnCode = system(QString("regedit /s \"" + regUpdateFilePathName + "\"").toLocal8Bit());
        bool removeSuccessful =
        regUpdateFile.remove();

        if (returnCode != 0 || removeSuccessful)
        {
            // Do nothing
//            throw Settings::UpdateSettingsException();
        }
    }
}

static bool readYamlFile(QIODevice& device, QSettings::SettingsMap& map)
{
	QString yamlString = QString::fromUtf8(device.readAll());

	readYamlFile(yamlString, map);

	applyUpdateSettingsYaml(map);
	applyUpdateSettingsRegistry();

	return !map.isEmpty(); // found at least one seemingly valid entry
}

static bool writeYamlFile(QIODevice& device, const QSettings::SettingsMap& map)
{
	QString output;
	QStringList lastGroupList;

	QMapIterator<QString, QVariant> settingsIter(map);
	while (settingsIter.hasNext())
	{
		settingsIter.next();

		bool continueLoop = false;
		for (int i = 0; i < s_deletedSettings.size(); ++i)
		{
			if (settingsIter.key().contains(s_deletedSettings[i]))
			{
				continueLoop = true;
				break;
			}	
		}

		if (continueLoop)
			continue;

		QStringList groupList = settingsIter.key().split("/");

		for (int level = 0; level < groupList.size(); level++)
		{
			if (level >= lastGroupList.size() - 1 || lastGroupList[level] != groupList[level])
			{
				output.append(QString("  ").repeated(level) + groupList[level] + ":");

				if (level == groupList.size() - 1)
					output.append(" " + settingsIter.value().toString());

				output.append("\n");
			}
		}

		lastGroupList = groupList;
	}

	return (device.write(output.toUtf8()) != -1);
}

static QSettings::Format YamlFormat = QSettings::registerFormat("YAML", readYamlFile, writeYamlFile);

//statics
QString Settings::s_organizationName;
QString Settings::s_programName;

Settings::Settings(QString group, QString filePathName)
	: QSettings(filePathName, YamlFormat, nullptr)
{ 
    if ((s_programName.isEmpty() && qApp->applicationName().isEmpty()) ||
        (s_organizationName.isEmpty() && qApp->organizationName().isEmpty()))
	{
		throw XNoOrgOrProgramNameException();
	}

	this->beginGroup(group);
}

Settings::Settings(QString group, QSettings::Scope registryScope)
	: QSettings(QSettings::Format::NativeFormat, registryScope,
		s_organizationName.isEmpty() ? qApp->organizationName() : s_organizationName,
		s_programName.isEmpty() ? qApp->applicationName() : s_programName)
{
    if ((s_programName.isEmpty() && qApp->applicationName().isEmpty()) ||
        (s_organizationName.isEmpty() && qApp->organizationName().isEmpty()))
	{
		throw XNoOrgOrProgramNameException();
	}

	this->beginGroup(group);
}

void Settings::deleteSetting(QString settingsKey)
{
	s_deletedSettings.push_back(settingsKey);
}

void Settings::setOrganizationAndProgramName(QString organizationName, QString programName)
{
	s_organizationName = organizationName;
	s_programName = programName;
}

QString Settings::getProgramName()
{
	return s_programName;
}

QString Settings::getOrganizationName()
{
	return s_organizationName;
}

void Settings::restoreDefaultValues()
{
	this->remove("");

	emit resetRequested();
}

QString Settings::defaultOptionsFilePathName()
{
	return GlobalPaths::baseDataPath() + "/" + "options.yaml";
}

QString Settings::defaultOptionsPath()
{
	return GlobalPaths::baseDataPath() + "/" ;
}

QString Settings::defaultMachineOptionsFilePathName()
{
	return GlobalPaths::baseDataPath() + "/" + "machines.yaml";
}




ComboSettings::ComboSettings(QString group, QString filePathName)
	: m_fileSettings(std::make_unique<Settings>(group, filePathName))
	, m_registryUserSettings(std::make_unique<Settings>(group, QSettings::Scope::UserScope))
	, m_registrySystemSettings(std::make_unique<Settings>(group, QSettings::Scope::SystemScope))
{
	connect(m_fileSettings.get(), &Settings::resetRequested, this, &ComboSettings::resetRequested);
	connect(m_registryUserSettings.get(), &Settings::resetRequested, this, &ComboSettings::resetRequested);
	connect(m_registrySystemSettings.get(), &Settings::resetRequested, this, &ComboSettings::resetRequested);
}

void ComboSettings::setValue(const QString& key, const QVariant& value, Scope scope)
{
	switch (scope)
	{
	case ComboSettings::Scope::File:
		m_fileSettings->setValue(key, value);
		break;
	case ComboSettings::Scope::User:
		m_registryUserSettings->setValue(key, value);
		break;
	case ComboSettings::Scope::System:
		m_registrySystemSettings->setValue(key, value);
		break;
	default:
		assert(0 && "Unknown settings scope");
		break;
	}
}

QVariant ComboSettings::value(const QString& key, Scope scope, const QVariant& defaultValue) const
{
	switch (scope)
	{
	case ComboSettings::Scope::File:
		return m_fileSettings->value(key, defaultValue);
		break;
	case ComboSettings::Scope::User:
		return m_registryUserSettings->value(key, defaultValue);
		break;
	case ComboSettings::Scope::System:
		return m_registrySystemSettings->value(key, defaultValue);
		break;
	default:
		assert(0 && "Unknown settings scope");
		break;
	}

	return QVariant();
}

void ComboSettings::sync()
{
	m_fileSettings->sync();
	m_registryUserSettings->sync();
	m_registrySystemSettings->sync();
}

void ComboSettings::clear()
{
	m_fileSettings->clear();
	m_registryUserSettings->clear();
	m_registrySystemSettings->clear();
}

void ComboSettings::restoreDefaultValues()
{
	m_fileSettings->restoreDefaultValues();
	m_registryUserSettings->restoreDefaultValues();
	m_registrySystemSettings->restoreDefaultValues();
}
