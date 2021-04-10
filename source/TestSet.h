#ifndef TestSet_h__
#define TestSet_h__

#include <vector>

#include "Test.h"

class CxTestSet
{
public:
    virtual ~CxTestSet();

    void addTest(CxTest* test);
    void clearFailures();

    // Appends all test failures to <failures>.
    // Returns true if there are failures, false if no tests failed.
    bool getFailures(CxTest::FailedAssertions& failures);

    size_t size();

    static CxTestSet& getInstance(void);

    void runTests();

protected:
    CxTestSet();

private:
//     CxTestSet(const CxTestSet& source);
//     const CxTestSet& operator= (const CxTestSet& source);

    using TestCollection = std::vector<CxTest*>;
    TestCollection m_testCollection;

};

#endif // TestSet_h__