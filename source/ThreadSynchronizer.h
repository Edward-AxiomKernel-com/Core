#ifndef FutureSynchronizer_h__
#define FutureSynchronizer_h__

#include <vector>

#include <QFutureWatcher>

class ThreadSynchronizer : public QObject
{
	Q_OBJECT

public:
	ThreadSynchronizer() = default;
	~ThreadSynchronizer();

	// #TODO implement cancel, pause, resume, ... similar to QFutureWatcher here.

	void addFutureWatcher(QFutureWatcher<void>& future);
	void emitAllThreadsFinished(); // helper function to emulate the signal when we don't use threads
	void reset();

signals:
	void allThreadsFinished();

public slots:
	void futureFinished();

private:
	std::vector<QFutureWatcher<void>*> m_futureWatchers;
	size_t m_finishedFutureWatchers = 0;
};

#endif // FutureSynchronizer_h__
