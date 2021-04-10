#ifndef ScopeGuard_h__
#define ScopeGuard_h__

#include <functional>

// Usage:
// ScopeGuard guard = [&]() { m_isActive = false; };
class ScopeGuard
{
public:
	template<class CallableT>
	ScopeGuard(CallableT&& undoFunction)
		try 
			: m_undoFunction(std::forward<CallableT>(undoFunction)) { }
		catch (...) 
		{
			undoFunction();
			throw;
		}

	ScopeGuard(ScopeGuard&& other) 
		: m_undoFunction(std::move(other.m_undoFunction)) 
	{
		other.m_undoFunction = nullptr;
	}

	~ScopeGuard() 
	{
		if (m_undoFunction) 
			m_undoFunction(); // must not throw
	}

	void dismiss() noexcept 
	{
		m_undoFunction = nullptr;
	}

	ScopeGuard(const ScopeGuard&) = delete;
	void operator= (const ScopeGuard&) = delete;

private:
	std::function<void()> m_undoFunction;
};

#endif // ScopeGuard_h__