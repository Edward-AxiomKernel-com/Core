#ifndef Test_h
#define Test_h

#include <string>
#include <vector>

#define TestAssert(assertion)\
    (this->testAssert((assertion), false, (#assertion), __LINE__, __FILE__)) // #TODO rename this to VerifyEqual() and also add VerifyDifferent() 
#define TestAssertFatal(assertion)\
    (this->testAssert((assertion), true, (#assertion), __LINE__, __FILE__)) // #TODO maybe this could be a valid alternative to the current Guid solution for exceptions that we use a similar macro there!


// ----------------------------------------------------------------------------
// Using CxTest: 
// 1) #include Test.h and TestSet.h (in your .cpp file or project header).
// 2) place the following at the bottom of your .cpp file:
//    #ifdef TEST
//    class <YourClassName>Test : public CxTest
//    {
//    public:
//       <YourClassName>Test()
//       {
//          CxTestSet::getInstance().addTest(this);
//       }
//
//       void test()
//       {
//			// testSomething();
//       }
// 
//       void testSomething()
//       {
//          // Add your test assertions here!!!
//          // TestAssert(<test case>);
//       }
//    };
//    static YourClassName>Test s_<YourClassName>Test;
//    #endif
// 
// Please try to give your test (here testSomething()) proper names.
// Tests can document the use of your class interface if you name them right. 
// Do not use test1(), test2(), ... 

class CxTest
{
public:
    using FailedAssertions = std::vector<std::string>;
	
    CxTest();
    CxTest(const CxTest& source);
    virtual ~CxTest();

	// implement in your derived classes. 
	virtual void test() = 0;

    const CxTest& operator=(const CxTest& source);

	void testAssert(bool assertion, bool abort = false, std::string expression = "", long lineNumber = 0, std::string fileName = "");
    void clearFailures();
    const FailedAssertions& getFailedAssertions();
    static std::string getTestFilePath() { return s_testFilePath; }

    // The path in which test files can be found that may be used to run tests.
    static void setTestFilePath(const std::string& testFilePath)
    { 
		s_testFilePath = testFilePath; 
	}

private:
    FailedAssertions m_failedAssertions;
    static std::string s_testFilePath;
};

#endif // #ifndef Test_h
