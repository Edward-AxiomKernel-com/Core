#include "App.h"
// App provides services that are related to the entire application: Provides the application
// name and version, processes command-line parameters and provides the related settings, and 
// initializes application-wide services such as logging.

// App creates a list of CommandLineParameter objects to handle everything having to do
// with command line parameters. Each CommandLineParameter derived class can test an actual
// argument to see if the CommandLineParameter should be used to process the actual argument,
// provides a function to handle the actual argument, and provides help text. App searches
// the list for a CommandLineParameter that can handle an actual argument and calls
// processArgument() when a CommandLineParameter is found.

#define qNoTestingFramework

#include <QDir>
#include <QMessageBox>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#ifndef qNoTestingFramework
#include <QTest>
#endif
#include <QtDebug>

#include <algorithm>

#include "LogMessage.h"
#include "Settings.h"

const char* kNoLogDirMessage = QT_TRANSLATE_NOOP("App", "Cannot create log file directory: '%1'.");
const char* kLogFileOpenMessage = QT_TRANSLATE_NOOP("App", "Cannot open log file: '%1'.");
const char* kAlreadyRunningMessage = QT_TRANSLATE_NOOP("App", "%1 is running already!");

const char* App::kInvalidParameterMessage = QT_TRANSLATE_NOOP("App", "Invalid parameter. Argument ignored: '%1'");


// App::s_version must be initialized in a derived class' cpp file.
// const Core::Version App::s_version(1, 0);

// static
const Core::Version& App::version() { return  App::s_version; }

App::App(int& argc, char** argv, ApplicationData applicationData)
#if defined qUseQtSingleApp
:   QtSingleApplication(applicationData.m_singleAppId, argc, argv),
#else
:   QApplication(argc, argv),
#endif
    m_serviceMode(false),
    m_testMode(false),
    m_noSplashScreenMode(false),
#ifdef qUseQtSingleApp
    m_allowMultipleInstances(false),
#else
    m_allowMultipleInstances(true),
#endif
    m_helpRequested(false),
    m_versionRequested(false),
    m_shouldAbort(false)
{
    this->saveCommandLine(argc, argv);
    this->setApplicationData(applicationData);
    Settings::setOrganizationAndProgramName(this->organizationName(), applicationData.m_applicationId);
}

// See note in header file regarding the reason for finishInitialization() being required.
void App::finishInitialization(int& argc, char** argv, ParameterList supportedParameters, QString helpFooterText)
{
    this->initializeLogging();

    this->checkArguments(argc, argv, supportedParameters);
    
    // Install translator here to display translated help text.
    this->initializeTranslation();

    if (m_helpRequested || this->hasCommandLineErrors())
        this->displayHelp(supportedParameters, helpFooterText);
    if (m_versionRequested)
        this->displayVersion();

#ifdef qUseQtSingleApp
    if (!this->allowMultipleInstances() && (this->sendMessage("Wake up!")))
    {
        QMessageBox::information(NULL, this->applicationName(), qApp->translate("App", kAlreadyRunningMessage).arg(this->applicationName()));
        ::exit(0);
    }

    QtSingleApplication::initialize(true);
#endif

    LogMessage::send(QString("Command line parameters:'%1'").arg(m_commandLine));

    if (m_helpRequested)
        LogMessage::send(this->helpText(supportedParameters, helpFooterText));

    if (m_versionRequested)
        LogMessage::send(s_version.toString());

    LogMessage::setServiceMode(m_serviceMode);
}

App::~App()
{
}

void App::saveCommandLine(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i)
    {
        m_commandLine += QString("%1%2").arg(i > 1 ? " " : "").arg(argv[i]);
    }
}

namespace
{
    class ArgumentMatcher
    {
    public:
        ArgumentMatcher(QString argument) : m_argument(argument) {}
        bool operator()(App::CommandLineParameter* parameter)
        {
            return parameter->test(m_argument);
        }
    private:
        QString m_argument;
    };
}

void App::checkArguments(int& argc, char** argv, const ParameterList& supportedParameters)
{
    //QString remainingArguments;
    for (int argumentIndex = 1; argumentIndex < argc; ++argumentIndex)
    {
        QString argument = argv[argumentIndex];
        ParameterList::const_iterator it =
            std::find_if(supportedParameters.begin(), supportedParameters.end(), ArgumentMatcher(argument));
        if (supportedParameters.end() != it)
        {
            CommandLineParameter::ProcessDirective directive = (*it)->processArgument(argument);
            m_errorMessages += (*it)->errorMessages();

            if (CommandLineParameter::kTerminateArgumentProcessing == directive)
                break;
        }
        // Couldn't find a matching parameter, check to see if a derived class takes a parameter without parameter identifier
        else if (this->takesSimpleParameter(argument))
        {
            // handled in derived class
        }
        else
        {
            m_errorMessages << qApp->translate("App", kInvalidParameterMessage);
            m_errorMessages << argument;
        }
    }
}

bool App::shouldAbort()
{
    return m_shouldAbort || this->hasCommandLineErrors();
}

void App::setTranslationFile(const QString& translationFile)
{
    m_translationFile = translationFile;
}

void App::displayVersion()
{
    QMessageBox::information(NULL, this->applicationName(), s_version.toString(), QMessageBox::Ok);
}

namespace
{
	class HtmlConcatinator
	{
	public:
	    HtmlConcatinator(QString& string) : m_string(string) {}
	    void operator()(App::CommandLineParameter* parameter)
	    {
	        m_string += parameter->helpTableRow();
	    }
	private:
	    QString& m_string;
	};
    class StringConcatinator
    {
    public:
        StringConcatinator(QString& string) : m_string(string) {}
        void operator()(App::CommandLineParameter* parameter)
        {
            m_string += parameter->helpText();
        }
    private:
        QString& m_string;
    };
}


void App::displayHelp(const ParameterList& parameterList, QString footerText)
{
    QMessageBox::information(NULL, qApp->translate("App", "%1 command line help").
        arg(this->applicationName()), this->helpHtml(parameterList, footerText), QMessageBox::Ok);
}

QString App::helpHtml(const ParameterList& supportedParameters, QString footerText)
{
    QString helpText = qApp->translate("App", "<TABLE WIDTH=700 BORDER=1 CELLPADDING=7 CELLSPACING=0>"
        "<TR><TH COLSPAN=2><h2>%1 command line parameters</h2></TH></TR>").arg(this->applicationName());

    std::for_each(supportedParameters.begin(), supportedParameters.end(), HtmlConcatinator(helpText));

    helpText += QString("<TR><TD COLSPAN=2>%1</TD></TR></TABLE>").arg(footerText);

    qDebug() << helpText;

    return helpText;
}

QString App::helpText(const ParameterList& supportedParameters, QString footerText)
{
    QString helpText = qApp->translate("App", "%1 command line help").arg(this->applicationName());

    std::for_each(supportedParameters.begin(), supportedParameters.end(), StringConcatinator(helpText));

    helpText += footerText;

    qDebug() << helpText;

    return helpText;
}

bool App::hasCommandLineErrors()
{
    return !m_errorMessages.isEmpty();
}

void App::displayErrors()
{
    QString errorReport = tr("<h1>Command line parameter errors found:</h1>");
    for (int i = 0; i < m_errorMessages.size(); i += 2)
    {
        errorReport += tr(qPrintable(m_errorMessages[i])).arg(m_errorMessages[i + 1]) + "<br>" ;
    }
    errorReport += "<p> </p><p> </p>";

    ParameterList supportedParameters(this->fillParameterList());
    errorReport += this->helpHtml(supportedParameters, this->helpTextFooter());

    QMessageBox::critical(nullptr, qApp->translate("App", "%1 command line errors").arg(this->applicationName()),
        errorReport, QMessageBox::Ok, QMessageBox::NoButton);
    LogMessage::send(errorReport);
}

bool App::notify(QObject* receiver, QEvent* event)
{
	bool done = true;
	try 
	{
		done = QApplication::notify(receiver, event);
	}
	catch (const std::exception& e) // this handles unexpected exceptions from other stacks, threads and processes
	{
		QMessageBox::critical(nullptr, applicationName(), e.what(), QMessageBox::Button::Close);
	}
	catch (...) 
	{
	}
	return done;
}

void App::initializeLogging()
{
    try
    {
        LogMessage::initialize(this->applicationId(), this->version().toString(), true);
    }
    catch(LogMessage::XNoLogDirException x)
    {
#ifdef _DEBUG
        QMessageBox::critical(0, App::applicationName(), qApp->translate("App", kNoLogDirMessage).arg(x.m_directoryPathName));
        exit(-1);
#endif
    }
    catch(LogMessage::XLogOpenException x)
    {
#ifdef _DEBUG
        QMessageBox::critical(0, App::applicationName(), qApp->translate("App", kLogFileOpenMessage).arg(x.m_filePathName));
        exit(-1);
#endif
    }
}

void App::setApplicationData(ApplicationData applicationData)
{
    this->setApplicationName(applicationData.m_applicationName);
    this->setOrganizationName(applicationData.m_organizationName);
    this->setOrganizationDomain(applicationData.m_organizationDomain);
}

QString App::applicationId()
{
    return Settings::getProgramName();
}

QString App::CommandLineParameter::helpText() const
{
    QString helpParameter = this->helpParameter();

    QStringList parameterlist = helpParameter.split('|');

    QString parameterString = parameterlist.join("\n");

    return QString("%1\t%2\n\n").arg(parameterString.leftJustified(20), this->helpDescription());
}

QString App::CommandLineParameter::helpTableRow() const
{
    QString helpParameter = this->helpParameter().toHtmlEscaped();
    QStringList parameterlist = helpParameter.split('|');
    QString parameterString = parameterlist.join("<br>");

    QString helpDescription = this->helpDescription().toHtmlEscaped();
    QStringList descriptionlist = helpDescription.split('|');
    QString helpString = descriptionlist.join("<br>");

    return QString("<TR><TD WIDTH=35%%>%1</TD><TD WIDTH=65%% VALIGN=BOTTOM>%2</TD></TR>").
        arg(parameterString, helpString);
}

