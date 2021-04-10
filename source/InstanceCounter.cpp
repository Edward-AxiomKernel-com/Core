#include "InstanceCounter.h"

#ifdef TEST

#include "TestSet.h"

class SampleClass1 : public InstanceCounter<SampleClass1>
{
};

class SampleClass2 : public InstanceCounter<SampleClass2>
{
};

class SampleClass3 : public InstanceCounter<SampleClass3>
{
};


static SampleClass1 s_a;
static StaticInstanceCounter<SampleClass1> s_aStaticAntiCounter;

class InstanceCounterTest : public CxTest
{
public:
    InstanceCounterTest()
    {
        CxTestSet::getInstance().addTest(this);
    }

    void test()
    {
        TestAssert(SampleClass1::getInstanceCount() == 0); // Check that the static instance, s_a was correctly accounted for.

        test1();
        TestAssert(SampleClass1::getInstanceCount() == 0);
        TestAssert(SampleClass2::getInstanceCount() == 0);
        TestAssert(SampleClass3::getInstanceCount() == 0);

        // Make sure that the static variable in test2() is initialized and anti-counted exactly once each, not per call.
        test2();
        test2();
        test2();

        TestAssert(SampleClass1::getInstanceCount() == 0); // Check that the static instance in test2() was correctly accounted for.
        TestAssert(SampleClass1::getStaticInstanceCount() == 2); // Check that the static instances, global and in test2() were correctly accounted for.
    }

    void test1()
    {
        SampleClass1 a1;
        SampleClass1 b1;

        SampleClass2 a2;

        TestAssert(SampleClass1::getInstanceCount() == 2);
        TestAssert(SampleClass2::getInstanceCount() == 1);
        TestAssert(SampleClass3::getInstanceCount() == 0);
    }

    void test2()
    {
        static SampleClass1 s_a;
        static StaticInstanceCounter<SampleClass1> s_aStaticAntiCounter;

        TestAssert(SampleClass1::getStaticInstanceCount() == 1);
    }
};
static InstanceCounterTest s_instanceCounterTest;
#endif // TEST
