#ifndef hInstanceCounter
#define hInstanceCounter

// InstanceCounter provides the ability to count the number of instances 
// of a class. Usage:
// class SampleClass : public InstanceCounter<SampleClass>
// class SampleWidgetClass : public QWidget, public InstanceCounter<SampleClass>
// class SampleObjectClass : public QObject, public InstanceCounter<SampleClass>
//
// Alternative:
// class SampleObjectClass : public QObject
// #ifdef qUseInstanceCounting
//     , public InstanceCounter<SampleClass>
// #endif
// {
//
// static SampleClass s_sampleClass;
// SampleClass::decrementInstanceCounterToAccountForStaticVariables();
//
// void debugReport()
// {
// #ifdef qUseInstanceCounting
//     qDebug("Test Debug Report");
//     qDebug("  Instance Counts");
//     qDebug("    SampleClass: %d", SampleClass::getInstanceCount());
//     qDebug("    SampleWidgetClass: %d", SampleWidgetClass::getInstanceCount());
//     qDebug("    SampleObjectClass: %d", SampleObjectClass::getInstanceCount());
//
// Private inheritance would make the destructor not need to be virtual, but would make InstanceCounter harder to use.
// See http://www.drdobbs.com/cpp/counting-objects-in-c/184403484
//
// StaticInstanceCounter<T> is to be instantiated with every static instance of T. If we do this consistently, this lets us know
// at the end of main that everything is accounted for.
// Example:
//
// static SampleClass1 s_a;
// static StaticInstanceCounter<SampleClass1> s_aStaticCounter;

// #ifdef qUseInstanceCounting is not used here in case someone purposefully chooses to use InstanceCounter without qUseInstanceCounting

template <class T>
class StaticInstanceCounter;

template <class T>
class InstanceCounter
{
	static int sInstanceCount;
    static int sStaticInstanceCount;

	public:
	InstanceCounter()
	{
		++sInstanceCount;
	}

	virtual ~InstanceCounter()
	{
		--sInstanceCount;
	}

	InstanceCounter(InstanceCounter const&)
	{
		++sInstanceCount;
	}

	static int getInstanceCount()
	{
		return sInstanceCount - sStaticInstanceCount;
	}

    static int getStaticInstanceCount()
    {
        return sStaticInstanceCount;
    }

    friend class StaticInstanceCounter<T>;
};

template <class T>
class StaticInstanceCounter
{
public:
    StaticInstanceCounter()
    {
        ++InstanceCounter<T>::sStaticInstanceCount;
    }

    virtual ~StaticInstanceCounter()
    {
        --InstanceCounter<T>::sStaticInstanceCount;
    }

};

template <class T>
int InstanceCounter<T>::sInstanceCount = 0;
template <class T>
int InstanceCounter<T>::sStaticInstanceCount = 0;

#endif
