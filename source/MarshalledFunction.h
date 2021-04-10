#ifndef MarshalledFunction_h__
#define MarshalledFunction_h__

#include <functional>
#include <vector>

#include <QString>
#include <QIcon>
 
struct MarshalledFunction
{
	std::function<void(void)> function = nullptr;
	QString name = "";
	QIcon icon;
};

using MarshalledFunctionList = std::vector<MarshalledFunction>;

 
#endif // MarshalledFunction_h__