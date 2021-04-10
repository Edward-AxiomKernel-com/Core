#ifndef WaitCursor_h
#define WaitCursor_h

#include <QApplication>
#include <QCursor>

class WaitCursor
{
public:

	WaitCursor(Qt::CursorShape cursor = Qt::CursorShape::WaitCursor)
	{
		QCursor* activeCursor = QApplication::overrideCursor();

		if (!activeCursor || activeCursor->shape() != Qt::CursorShape::WaitCursor)
		{
			QApplication::setOverrideCursor(QCursor(cursor));
			m_showing = true;
		}
	}

	virtual ~WaitCursor(void)
	{
		this->hide();
	}

	void hide()
	{
		if (m_showing)
			QApplication::restoreOverrideCursor();

		m_showing = false;
	}

private:
	bool m_showing;
};

class UnWaitCursor
{
public:

	UnWaitCursor(void)
	{
		m_waitCursor = QApplication::overrideCursor()->shape();

		QApplication::restoreOverrideCursor();
	}

	virtual ~UnWaitCursor(void)
	{
		QApplication::setOverrideCursor(QCursor(m_waitCursor));
	}

private:
	Qt::CursorShape m_waitCursor;
};

#endif
