#include "Version.h"

namespace Core
{
    Version::Version(QString version, QString subVersion, QString revision, QString revisionDate, QString revisionModified, QString buildDate, QString minorSubVersion)
    :   m_version(version.toUInt()),
        m_subVersion(subVersion.toUInt()),
        m_minorSubVersion(minorSubVersion),
        m_revision(revision.toUInt()),
        m_buildHourIndicator(QChar(buildDate.mid(11,2).toInt() + 'A')),
            m_revisionDate(QDate::fromString(revisionDate)),
            m_isRevisionModified(!revisionModified.isEmpty()),
            m_buildDate(buildDate.left(10) + "/" + m_buildHourIndicator)
    {
    }

    Version::Version(uint version, uint subVersion, uint revision)
    :   m_version(version),
        m_subVersion(subVersion),
        m_minorSubVersion(),
        m_revision(revision),
        m_buildHourIndicator(),
        m_revisionDate(),
        m_isRevisionModified(false),
        m_buildDate()
    {
    }

    Version::Version(QString versionString)
    :   m_version(0),
        m_subVersion(0),
        m_minorSubVersion(),
        m_revision(versionString.section("R", -1).section(" ", 0, 0).toUInt()),
        m_buildHourIndicator(),
        m_revisionDate(),
        m_isRevisionModified(false),
        m_buildDate()
    {
        QString shortString = versionString.section("R", 0, 0).trimmed();
        m_version = shortString.section(".", 0, 0).toUInt();
        m_subVersion = shortString.section(".", 1, 1).toUInt();
    }

    Version::~Version()
    {
    }


    bool Version::isValid()
    {
        return -1 != m_version && -1 != m_subVersion &&
            0 != m_version && 0 != m_subVersion;
    }

    bool Version::operator<(const Version& other) const
    {
        if (m_version < other.m_version)
            return true;
        else if (m_version == other.m_version)
        {
            if (m_subVersion < other.m_subVersion)
                return true;
            else if (m_subVersion == other.m_subVersion)
            {

                if (this->minorSubversionUsed(other))
                {
                    if (m_minorSubVersion.toUInt() < other.m_minorSubVersion.toUInt())
                        return true;
                    else if (m_minorSubVersion.toUInt() > other.m_minorSubVersion.toUInt())
                        return false;
                }
                // else m_minorSubVersion not used
                // or if fall through, then m_minorSubVersions are equal

                if (-1 != m_revision && -1 != other.m_revision)
                {
                    if (m_revision < other.m_revision)
                        return true;
                    else if (m_revision == other.m_revision)
                    {
                        if (m_revisionDate < other.m_revisionDate)
                            return true;
                        else if (m_revisionDate == other.m_revisionDate)
                        {
                            if (m_buildHourIndicator < other.m_buildHourIndicator)
                                return true;
                            // Would it be practical to check further?
                        }
                    }
                }
            }
        }
        return false;
    }

    bool Version::operator==(const Version& other) const
    {
        return ((m_version == other.m_version && m_subVersion == other.m_subVersion &&
            this->minorSubVersionNotUsedOrEqual(other) &&
            this->revisionNotUsed(other)) ||
            (m_revision == other.m_revision && m_revisionDate == other.m_revisionDate &&
             m_buildHourIndicator == other.m_buildHourIndicator));
    }

    Core::Version Version::shortVersion() const
    {
        return Version(m_version, m_subVersion);
    }

    const QString Version::revisionString(uint padLength) const
    {
        return QString::number(m_revision).rightJustified(padLength, '0');
    }

    const QString Version::buildRevisionString() const
    {
        QString revision = QString::number(m_revision);

        if (m_isRevisionModified)
            revision += "  --- local modified " + m_buildDate + " --- ";

        return revision;
    }

    const QString Version::toString(int superVersion) const
    {
        QString versionString = this->shortString(superVersion);

        if (-1 != m_revision)
            versionString += QString(" R%1").arg(this->buildRevisionString());

        return versionString;
    }

    const QString Version::toNoRevisionString(int superVersion) const
    {
        QString versionString = this->shortString(superVersion) + QString(".%1").arg(m_minorSubVersion);

        return versionString;
    }

    const QString Version::shortString(int superVersion) const
    {
        QString versionString;
        if (superVersion > 0)
            versionString += QString("%1.").arg(superVersion);
        QString paddedSubversion = QString::number(m_subVersion).rightJustified(2, '0');
        versionString += QString("%1.%2").arg(m_version).arg(paddedSubversion);
        return versionString;
    }

    const QString Version::compactString(uint padLength) const
    {
        QString paddedSubversion = QString::number(m_subVersion).rightJustified(2, '0');
        return QString("%1%2%3").arg(m_version).arg(paddedSubversion).arg(revisionString(padLength));
    }

    bool Version::minorSubversionUsed(const Version &other) const
    {
        return !m_minorSubVersion.isEmpty() && !other.m_minorSubVersion.isEmpty();
    }

    bool Version::minorSubVersionNotUsedOrEqual(const Version& other) const
    {
        return (!this->minorSubversionUsed(other) ||
            m_minorSubVersion.toUInt() == other.m_minorSubVersion.toUInt());           // equal
    }

    bool Version::revisionNotUsed(const Version& other) const
    {
        return (-1 == m_revision || -1 == other.m_revision);
    }
}
