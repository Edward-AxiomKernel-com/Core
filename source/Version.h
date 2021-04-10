#ifndef Version_h__
#define Version_h__

#include <qglobal.h>

#if TARGET_OS_WIN32
#pragma warning(push, 2)
#endif

#include <QDateTime>
#include <QString>

#if TARGET_OS_WIN32
#pragma warning(pop)
#endif

namespace Core
{
    class Version
    {
    public:
        Version(QString version, QString subVersion, QString revision, QString revisionDate, QString revisionModifiedString, QString buildDate, QString minorSubVersion = "");
        Version(uint version = -1, uint subVersion = -1, uint revision = -1);
        Version(QString versionString);
        ~Version();

        bool isValid();

        bool operator==(const Version& other) const;
        bool operator<(const Version& other) const;

        friend bool operator!=(const Version& lhs, const Version& rhs);
        friend bool operator> (const Version& lhs, const Version& rhs);
        friend bool operator<=(const Version& lhs, const Version& rhs);
        friend bool operator>=(const Version& lhs, const Version& rhs);

        uint revision() const { return m_revision; }
        const QString minorSubVersion() const { return m_minorSubVersion; }

        Version shortVersion() const;

        const QString revisionString(uint padLength = 0) const;
        const QString buildRevisionString() const;
        const QString toString(int superVersion = 0) const;
        const QString toNoRevisionString(int superVersion) const;
        const QString shortString(int superVersion = 0) const;
        const QString compactString(uint padLength) const;

    private:
        bool minorSubversionUsed(const Version &other) const;
        bool minorSubVersionNotUsedOrEqual(const Version& other) const;
        bool revisionNotUsed(const Version& other) const;
        int m_version;
        int m_subVersion;
        QString m_minorSubVersion;
        int m_revision;
        QChar m_buildHourIndicator;
        QDate m_revisionDate;
        bool m_isRevisionModified;
        QString m_buildDate;
    };

    inline bool operator!=(const Version& lhs, const Version& rhs) { return !lhs.operator==(rhs); }
    inline bool operator> (const Version& lhs, const Version& rhs) { return  rhs < lhs; }
    inline bool operator<=(const Version& lhs, const Version& rhs) { return !operator> (lhs,rhs); }
    inline bool operator>=(const Version& lhs, const Version& rhs) { return !lhs.operator<(rhs); }
}

#endif // Version_h__


