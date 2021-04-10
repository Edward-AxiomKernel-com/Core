#ifndef NotifierSettings_h__
#define NotifierSettings_h__

#include "Settings.h"

class NotifierSettings : public ComboSettings
{
public:
	static NotifierSettings* getInstance()
	{
		static std::unique_ptr<NotifierSettings> s_settingsInstance(new NotifierSettings); // unique_ptr deletes

		if (qApp->alwaysReloadSettings())
			s_settingsInstance.reset(new NotifierSettings); // unique_ptr deletes

		return s_settingsInstance.get();
	}

    BoolSetting useNotifier{ this, Scope::File, QSL("Use PushNotifier"), true };
	StringSetting ipAddress{ this, Scope::File, QSL("IP Address"), QSL("") };
	IntSetting port{ this, Scope::File, QSL("Port"), 0 };
    IntSetting verbosity{ this, Scope::File, QSL("Status message verbosity (0-4)"), 1 };

protected:
	NotifierSettings(NotifierSettings const&) = delete;
	NotifierSettings(NotifierSettings&&) = delete;
	NotifierSettings& operator=(NotifierSettings&&) = delete;
	NotifierSettings& operator=(const NotifierSettings& other) = delete;

	NotifierSettings::NotifierSettings()
		: ComboSettings(QSL("PushNotifier"))
	{
	}
};

#endif // NotifierSettings_h__