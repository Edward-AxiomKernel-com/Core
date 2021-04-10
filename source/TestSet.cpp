#include "TestSet.h"

#include <algorithm>
#include <iterator>
#include <assert.h>
using namespace std;

CxTestSet::~CxTestSet() = default;

void CxTestSet::addTest(CxTest* test)
{
    m_testCollection.push_back(test);
}

void CxTestSet::clearFailures()
{
    for (TestCollection::iterator i = m_testCollection.begin();
        i < m_testCollection.end(); ++i)
    {
        (*i)->clearFailures();
    }
}

bool CxTestSet::getFailures(CxTest::FailedAssertions& failures)
{
    for (TestCollection::iterator i = m_testCollection.begin(); i < m_testCollection.end(); ++i)
    {
        CxTest::FailedAssertions failedAssertions = (*i)->getFailedAssertions();
        copy(failedAssertions.begin(), failedAssertions.end(),
            back_inserter(failures));
    }

    if (m_testCollection.empty())
        return false;
    else
        return true;
}

size_t CxTestSet::size()
{
    return m_testCollection.size();
}

CxTestSet& CxTestSet::getInstance(void)
{
    static CxTestSet sTestSet;
    return sTestSet;
}

void CxTestSet::runTests()
{
    for (TestCollection::iterator i = m_testCollection.begin();
        i < m_testCollection.end(); ++i)
        (*i)->test();
}

CxTestSet::CxTestSet()
:   m_testCollection()
{
}
