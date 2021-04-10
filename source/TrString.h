#ifndef TrString_h__
#define TrString_h__

#include <QString>
#include <QRegularExpression>
#include <cassert>

class QObject;

namespace
{
	static QRegularExpression s_findClassNameRegExp("(?<=\\bclass\\s)(\\w+)");
	static QRegularExpression s_findQuotesRegExp("\"([^\"\\\\]|\\\\.)*\"");
}

#define TRStringVoid(QT_TRANSLATE_NOOP_STRING) TrString<void>(#QT_TRANSLATE_NOOP_STRING)
#define TRString(QT_TR_NOOP_STRING) TrString<decltype(this)>(this, #QT_TR_NOOP_STRING)

// Use      //: MyComment
// one line before the call to add a comment for the translator

namespace
{
	const QString kQT_TR_NOOP_Macro = "QT_TR_NOOP(";
	const QString kQT_TRANSLATE_NOOP_Macro = "QT_TRANSLATE_NOOP(";
	const QString kQT_TRANSLATE_NOOP_3_Macro = "QT_TRANSLATE_NOOP3(";
}

// Use TRString or TRStringVoid macro to get an instance of this class
template <class T> class TrString
{
	enum class MacroType { Unknown, QT_TR_NOOP, QT_TRANSLATE_NOOP, QT_TRANSLATE_NOOP3 };

public:
	// used for global functions where T is void
	template <typename SfinaeT = void, typename SfinaeT2 = std::enable_if_t<std::is_void<T>::value, T>>
	TrString(const char* lineString)
		: m_native(lineString)
	{
		m_macroType = getLineStringType(lineString);

		assert(("You must use TRStringVoid(QT_TRANSLATE_NOOP('Context','StringToTranslate')) here because you are not in a class context.",
			(m_macroType == MacroType::QT_TRANSLATE_NOOP || m_macroType == MacroType::QT_TRANSLATE_NOOP3)));

		initStrings();
	}

	// used for calls from within objects
	template <typename SfinaeT = void, typename SfinaeT2 = std::enable_if_t<!std::is_void<T>::value, T>>
	TrString(SfinaeT2 caller, const char* lineString)
		: m_native(lineString)
	{
		m_macroType = getLineStringType(lineString);
		checkIfTemplateHasCorrectType(caller);
		initStrings();
	}

	TrString& operator=(const TrString& other)
	{
		m_scope = other.m_scope;
		m_native = other.m_native;
		m_translated = other.m_translated;
	}

	TrString& arg(const TrString& arg1)
	{
		this->m_native = this->m_native.arg(arg1.native());
		this->m_translated = this->m_translated.arg(qApp->translate(m_scope.toLocal8Bit().data(), arg1.translated().toLocal8Bit().data()));

		return *this;
	}

	TrString& arg(const QString arg1)
	{
		this->m_native = this->m_native.arg(arg1);
		this->m_translated = this->m_translated.arg(arg1);
		return *this;
	}

	TrString& arg(const double arg1)
	{
		this->m_native = this->m_native.arg(arg1);
		this->m_translated = this->m_translated.arg(arg1);
		return *this;
	}

	inline const QString& native() const
	{
		return this->m_native;
	}

	inline const QString& translated() const
	{
		return this->m_translated;
	}

private:
	template<class SfinaeT = T>
	typename std::enable_if<!std::is_polymorphic< std::remove_pointer_t<SfinaeT>>::value, void>::type
                checkIfTemplateHasCorrectType(SfinaeT /*caller*/)
	{
		assert(("You must use TRString(QT_TRANSLATE_NOOP('Context', 'StringToTranslate')) here because your class does not derive from QObject.",
			(m_macroType == MacroType::QT_TRANSLATE_NOOP || m_macroType == MacroType::QT_TRANSLATE_NOOP3)));
	}

	template<class SfinaeT = T>
	typename std::enable_if<std::is_polymorphic< std::remove_pointer_t<SfinaeT>>::value, void>::type
		checkIfTemplateHasCorrectType(SfinaeT caller)
	{
		if (!dynamic_cast<QObject*>(caller))
			assert(("You must use TRString(QT_TRANSLATE_NOOP('Context', 'StringToTranslate')) here because your class does not derive from QObject.",
			(m_macroType == MacroType::QT_TRANSLATE_NOOP || m_macroType == MacroType::QT_TRANSLATE_NOOP3)));
		else
			m_isDerivedFromQObject = true;
	}

        MacroType getLineStringType(QString /*lineString*/) const
	{
		if (m_native.contains(kQT_TR_NOOP_Macro))
			return MacroType::QT_TR_NOOP;
		else if (m_native.contains(kQT_TRANSLATE_NOOP_3_Macro))
			return MacroType::QT_TRANSLATE_NOOP3;
		else if (m_native.contains(kQT_TRANSLATE_NOOP_Macro))
			return MacroType::QT_TRANSLATE_NOOP;
		else
		{
                        assert(false && "You must you TRString(QT_TR_NOOP('StringToTranslate') or TRString(QT_TRANSLATE_NOOP('Context', 'StringToTranslate')) if you are not in a QObject derived class for this to work.");
			return MacroType::Unknown;
		}
	}

	inline void initStrings()
	{
		QRegularExpressionMatchIterator regExpIter = s_findQuotesRegExp.globalMatch(m_native);
		QStringList macroArguments;

		while (regExpIter.hasNext())
		{
			QRegularExpressionMatch match = regExpIter.next();
			QString argumentString = match.captured(0);
			argumentString.remove(0, 1);
			argumentString.remove(argumentString.length() - 1, 1);
			macroArguments.push_back(argumentString);
		}

		if (macroArguments.size() == 1 && m_macroType == MacroType::QT_TR_NOOP)
		{
			m_scope = QRegularExpressionMatch(s_findClassNameRegExp.match(typeid(T).name())).captured(0);
			m_native = macroArguments[0];
		}
		else if ((macroArguments.size() == 2 && m_macroType == MacroType::QT_TRANSLATE_NOOP) || (macroArguments.size() == 3 && m_macroType == MacroType::QT_TRANSLATE_NOOP3))
		{
			m_scope = macroArguments[0];
			m_native = macroArguments[1];
		}
		else
		{
                        assert(false && "You must you TRString(QT_TR_NOOP('StringToTranslate') or TRString(QT_TRANSLATE_NOOP('Context', 'StringToTranslate')) if you are not in a QObject derived class for this to work.");
		}

		m_translated = qApp->translate(m_scope.toLocal8Bit().data(), m_native.toLocal8Bit().data());
	}

	MacroType m_macroType = MacroType::Unknown;
	bool m_isDerivedFromQObject = false;
	QString m_native;
	QString m_translated;
	QString m_scope;
};

#endif // TrString_h__
