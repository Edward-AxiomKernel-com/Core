#ifndef QtVersionDefines_h__
#define QtVersionDefines_h__

#if QT_VERSION >= 0x040000

#define leftJustify leftJustified
#define rightJustify rightJustified 
#define qEscapeContext Qt

#else

#define qEscapeContext QStyleSheet
#define toLower lower
#define leftJustified leftJustify
#define rightJustified rightJustify
#define qPrintable(string) (string).operator const char*()

#endif

#endif // QtVersionDefines_h__
