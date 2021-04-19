#ifndef GlobalSettings_h__
#define GlobalSettings_h__

#include "Settings.h"

#include <QCoreApplication>
#include <QtDebug>

class GlobalSettings : public Settings // ComboSettings
{
public:
    GlobalSettings()
        : Settings(QSL("Global"))
//        : ComboSettings(QSL("Global"))
    {
        qDebug() << "GlobalSettings:";
        qDebug() << "  fileName:" << fileName();
        qDebug() << "  format:" << format();
        qDebug() << "  group:" << group();
        qDebug() << "  fallbacksEnabled:" << fallbacksEnabled();
    }

//	static GlobalSettings* getInstance()
//	{
//		// static std::unique_ptr<GlobalSettings> s_settingsInstance = std::make_unique<GlobalSettings>(); // #UNCLEAR why is this not working?

//		static std::unique_ptr<GlobalSettings> s_settingsInstance(new GlobalSettings); // unique_ptr deletes

//		if (qApp->alwaysReloadSettings())
//			s_settingsInstance.reset(new GlobalSettings); // unique_ptr deletes

//		return s_settingsInstance.get();
//	}


#if 1
    Setting<QString> lastAccessToken{ this, QSL("Last access token"), QSL("No access token") };
    Setting<QString> lastTokenType{ this, QSL("Last token type"), QSL("Invalid") };
    Setting<int> lastValidSeconds{ this, QSL("Last valid seconds") };
    Setting<QDateTime> lastValidFrom{ this, QSL("Last valid from") };

    Setting<QString> language{ this, QSL("Language"), "" };

#else
    Setting<QString> lastAccessToken{ this, Scope::User, QSL("Last access token") };
    Setting<QString> lastTokenType{ this, Scope::User, QSL("Last token type") };
    Setting<int> lastValidSeconds{ this, Scope::User, QSL("Last valid seconds") };
    Setting<QDateTime> lastValidFrom{ this, Scope::User, QSL("Last valid from") };
#endif

//	Setting<QString> language{ this, Scope::File, QSL("Language"), "" };

//	Setting<QString> lastCustomerState{ this, Scope::User, QSL("Last customer state"), QSL("C:\\_cxKunden") };
//	Setting<bool> autoLoginOnStart{ this, Scope::User, QSL("Login on start"), false };
//	Setting<QString> lastUsername{ this, Scope::User, QSL("Last logged in user"), QSL("") };
//	Setting<QString> autoLoginPasswordHash{ this, Scope::User, QSL("Auto Login Password Hash"), QSL("") };

#if DEBUG
//	Setting<QString> stylesheetFilePathName{ this, Scope::File, QSL("Stylesheet File"), QCoreApplication::applicationDirPath().toLatin1() + "/../../_styles/default.css", DefaultValue::Hidden };
//#else
//	Setting<QString> stylesheetFilePathName{ this, Scope::File, QSL("Stylesheet File"), QCoreApplication::applicationDirPath().toLatin1() + "/default.css", DefaultValue::Hidden };
#endif

protected:
	GlobalSettings(GlobalSettings const&) = delete;
	GlobalSettings(GlobalSettings&&) = delete;
	GlobalSettings& operator=(GlobalSettings const&) = delete;
	GlobalSettings& operator=(GlobalSettings &&) = delete;

};

/*
class DebugSettings : public Settings
{
public:
	static DebugSettings* getInstance()
	{
		static std::unique_ptr<DebugSettings> s_settingsInstance(new DebugSettings); // unique_ptr deletes

		if (qApp->alwaysReloadSettings())
			s_settingsInstance.reset(new DebugSettings); // unique_ptr deletes

		return s_settingsInstance.get();
	}

	Setting<bool> modelItemDebugOutput{ this, QSL("ModelItem Debug output"), false, DefaultValue::Hidden };
	Setting<bool> macroDragDebugOutput{ this, QSL("Macro Drag Debug output"), false, DefaultValue::Hidden };
	Setting<bool> macroDropDebugOutput{ this, QSL("Macro Drop Debug output"), false, DefaultValue::Hidden };
	Setting<bool> cxDatabaseDebugOutput{ this, QSL("CxDatabase Debug output"), false, DefaultValue::Hidden };
	Setting<bool> profileContourDebugOutput{ this, QSL("ProfileDatabase Debug output"), false, DefaultValue::Hidden };
	Setting<bool> profileDbDebugOutput{ this, QSL("ProfileContour Debug output"), false, DefaultValue::Hidden };
	Setting<bool> formulaPropertyDebugOutput{ this, QSL("FormulaProperty Debug output"), false, DefaultValue::Hidden };
	Setting<bool> jsEngineDebugOutput{ this, QSL("JSEngine Debug output"), false, DefaultValue::Hidden };
	Setting<bool> ncxExportDebugOutput{ this, QSL("NcxExport Debug output"), false, DefaultValue::Hidden };
	Setting<bool> recordLockerDebugOutput{ this, QSL("RecordLocker Debug output"), false, DefaultValue::Hidden };
	Setting<bool> relationalTablePropertyDebugOutput{ this, QSL("RelationalTableProperty Debug output"), false, DefaultValue::Hidden };
	Setting<bool> elementDbDebugOutput{ this, QSL("Element DB Debug output"), false, DefaultValue::Hidden };

	Setting<bool> showCadXtractSimilarityPolygons{ this, QSL("CadXtract Show Similarity Polygons"), false, DefaultValue::Hidden };

protected:
	DebugSettings(DebugSettings const&) = delete;
	DebugSettings(DebugSettings&&) = delete;
	DebugSettings& operator=(DebugSettings const&) = delete;
	DebugSettings& operator=(DebugSettings&&) = delete;

	DebugSettings::DebugSettings()
		: Settings(QSL("Debug"))
	{ }
};
*/

#endif // GlobalSettings_h__
