#ifndef Pimpl_h__
#define Pimpl_h__

#include <memory>

#define usePimpl(className)									\
	protected:												\
		struct PimplData;									\
		std::unique_ptr<PimplData> d;						\
		void copyDataFrom(const className& other);          \
	private:

#define usePimplNoCopy(className)							\
	protected:												\
		struct PimplData;									\
		std::unique_ptr<PimplData> d;						\
	private:


#endif // Pimpl_h__
