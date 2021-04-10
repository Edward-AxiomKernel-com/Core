#ifndef App_h__
#define App_h__

#include <QtCore/QtGlobal>

#ifdef qUseQtSingleApp
#include "qtsingleapplication.h"
#else
#include <QApplication>
#endif

#include <QFont>
#include <QRegExp>
#include <QStringList>

#include <vector>

#include "LogMessage.h"
#include "Version.h"


class App :
#ifdef qUseQtSingleApp
    public QtSingleApplication
#else
    public QApplication
#endif
{
    Q_OBJECT

public:
    static const Core::Version& version();

    struct ApplicationData
    {
        ApplicationData(QString applicationName, QString organizationName, QString organizationDomain, QString singleAppId = "")
        :   m_applicationName(applicationName),
            m_organizationName(organizationName),
            m_organizationDomain(organizationDomain),
            m_singleAppId(singleAppId),
            m_applicationId(applicationName)
        {}
        QString m_applicationName;
        QString m_organizationName;
        QString m_organizationDomain;
        QString m_singleAppId;
        QString m_applicationId;
    };

    class CommandLineParameter
    {
    public:
        virtual ~CommandLineParameter() {}
        virtual bool test(QString argument) const = 0;

        enum ProcessDirective { kNoTerminalError, kTerminateArgumentProcessing };
        virtual CommandLineParameter::ProcessDirective processArgument(QString argument) = 0; // Return false to terminate further processing of arguments

        virtual QString helpText() const;
        virtual QString helpTableRow() const;
        QStringList errorMessages() { return m_errorMessages; }

        QString removeOuterQuotesIfPresent(QString string)
        {
            QRegExp removeQuotes("^\\\"(.*)\\\"$");
            return (removeQuotes.exactMatch(string) ? removeQuotes.cap(1) : string);
        }

    protected:
        virtual QString helpParameter() const = 0;
        virtual QString helpDescription() const = 0;

        QStringList m_errorMessages;
    };

    class ParameterList : public std::vector<App::CommandLineParameter*>
    {
    public:
        ParameterList() {}
        ParameterList(ParameterList&& other)
        {
            this->assign(other.begin(), other.end());
            other.clear();
        }
        ~ParameterList()
        {
            while (!this->empty())
            {
                delete this->back();
                this->pop_back();
            }
        }
    };
    
    static const char* kInvalidParameterMessage;

    App(int& argc, char** argv, ApplicationData applicationData);

    // The function finishInitialization() is required because the functions to prepare data,
    // helpTextFooter() and fillParameterList(), may both use QApplication::translate(), which 
    // cannot be called until the QApplication has been constructed. These functions are both
    // also called as virtual functions from App::displayErrors().

    void finishInitialization(int& argc, char** argv, ParameterList supportedParameters, QString helpFooterText);

    ~App();

    QString applicationId();

    bool allowMultipleInstances() { return m_allowMultipleInstances; }

    void setTranslationFile(const QString& apTrFile);

    bool shouldAbort();

    bool hasCommandLineErrors();
    void displayErrors();
    bool serviceMode() { return m_serviceMode; }
    bool testMode() { return m_testMode; }
    bool alwaysReloadSettings() { return m_reloadSettings; }
    bool quietTestMode() { return m_quietTestMode; }
    bool helpRequested() { return m_helpRequested; }
    bool noSplashScreenMode() { return m_noSplashScreenMode; }

	// this is used to handle exceptions that could not be catched somewhere else due to signals and slots
	bool notify(QObject* receiver, QEvent* event) override;

signals:
	void serviceModeToggled(bool active);

public slots:
    void toggleServiceMode() 
    {
        m_serviceMode = !m_serviceMode; 
        LogMessage::setServiceMode(m_serviceMode);
		emit serviceModeToggled(m_serviceMode);
    }

protected:
    void setApplicationData(ApplicationData applicationData);
    virtual ParameterList fillParameterList() = 0;
    virtual QString helpTextFooter() = 0;

    void checkArguments(int& argc, char** argv, const ParameterList& supportedParameters);
    virtual bool takesSimpleParameter(QString /*argument*/) { return false; }

    void saveCommandLine(int argc, char** argv);

    void displayHelp(const ParameterList& parameterList, QString footerText);
    QString helpHtml(const ParameterList& supportedParameters, QString footerText);

    QString helpText(const ParameterList& supportedParameters, QString footerText);

    void displayVersion();
    void initializeLogging();
    virtual void initializeTranslation() {}

    static const Core::Version s_version;

    QString m_commandLine;


    QString m_translationFile;
    bool m_serviceMode;
    bool m_testMode;
    bool m_quietTestMode;
    bool m_reloadSettings;
    QString m_testSubarguments;

    bool m_noSplashScreenMode;
    bool m_allowMultipleInstances;

    bool m_helpRequested;
    bool m_versionRequested;
    bool m_shouldAbort;
    QStringList m_errorMessages;
};


#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<App*>(QCoreApplication::instance()))

#endif // App_h__
