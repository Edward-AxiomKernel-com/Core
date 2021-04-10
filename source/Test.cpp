#include "Test.h"

#include <assert.h>
#include <iostream>
#include <sstream>

std::string CxTest::s_testFilePath = "";

CxTest::CxTest()
:   m_failedAssertions()
{ }

CxTest::CxTest(const CxTest& source)
{
    *this = source;
}

CxTest::~CxTest() = default;

const CxTest& CxTest::operator=(const CxTest& source)
{
    if (&source != this)
        m_failedAssertions = source.m_failedAssertions;

    return *this;
}

void CxTest::testAssert(bool assertion, bool abort, std::string expression, long lineNumber, std::string fileName)
{
    if (false == assertion)
    {
        std::stringstream assertion;
        assertion << "Test assertion: (" << expression << ") FAILED" << std::endl;
        assertion << "   Line: " << lineNumber << ", File: " << fileName;
        m_failedAssertions.push_back(assertion.str());
        if (abort)
            assert(false);
    }
}

void CxTest::clearFailures()
{
    m_failedAssertions.clear();
}

const CxTest::FailedAssertions& CxTest::getFailedAssertions()
{
    return m_failedAssertions;
}

#ifdef TEST
#include "TestSet.h"
class CxTestTest : public CxTest
{
public:
	CxTestTest()
    {
        CxTestSet::getInstance().addTest(this);
    }

    void test()
    {
        // Normal usage:
        testAssert(true);
        testAssert(1 == 5); // this should be first (and only) failure

        // Actual CxTest tests:
        // Take note of the single failure produced above:
        bool firstRunSuccess = (1 == this->getFailedAssertions().size());
        this->clearFailures();
        // Assert that there was indeed an earlier failure:
        testAssert(firstRunSuccess);
        // Assert that there are now no failures:
        testAssert(0 == this->getFailedAssertions().size());
    }
};

static CxTestTest s_cxTestTest;

#endif