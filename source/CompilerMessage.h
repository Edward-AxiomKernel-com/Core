/*
#include "CompilerMessage.h"
*/

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)

#define WARNING(text) message(__FILE__ "(" STRINGIZE(__LINE__) ") : Warning: " #text)
#define CompilerMessage(text) message(__FILE__ "(" STRINGIZE(__LINE__) ") : CompilerMessage: " #text)

/* usage:
#pragma WARNING(FIXME: Code removed because...)

#pragma CompilerMessage(text) 
*/
