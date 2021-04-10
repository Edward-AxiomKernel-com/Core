#include "ThreadSynchronizer.h"



ThreadSynchronizer::~ThreadSynchronizer()
{
	for (auto future : m_futureWatchers)
		future->waitForFinished();
}

void ThreadSynchronizer::addFutureWatcher(QFutureWatcher<void>& future)
{
	m_futureWatchers.push_back(&future);
	bool works = connect(m_futureWatchers.back(), &QFutureWatcher<void>::finished, this, &ThreadSynchronizer::futureFinished);
}

void ThreadSynchronizer::emitAllThreadsFinished()
{
	emit allThreadsFinished();
}

void ThreadSynchronizer::reset()
{
	m_futureWatchers.clear();
	m_finishedFutureWatchers = 0;
}

void ThreadSynchronizer::futureFinished()
{
	++m_finishedFutureWatchers;

	if (m_futureWatchers.size() == m_finishedFutureWatchers)
		emit allThreadsFinished();
}
