#ifndef SignalBlocker_h
#define SignalBlocker_h

#include <QObject>

namespace Core
{
	class SignalBlocker
	{
	public:

		enum BlockFlag
		{
			kNoBlock,
			kBlock
		};

		SignalBlocker(QObject* object)
		:   m_object(object)
		{
			if (NULL == m_object)
				return;

			m_blocked = m_object->blockSignals(true);
		}

		~SignalBlocker()
		{
			if (NULL == m_object)
				return;

			m_object->blockSignals(m_blocked);
		}

	protected:
		QObject* m_object;
		bool m_blocked;
	};
}

#endif // SignalBlocker_h
