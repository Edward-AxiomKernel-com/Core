#ifndef UnicodeSymbols_h__
#define UnicodeSymbols_h__

// Symbols must be in byte inverted UTF16 encoding. 
// e.g. Diameter Symbol:
// 0x2300 becomes QChar(0x00, 0x23)

namespace Symbol 
{
	const QString CircledX = QChar(0xE7, 0x24);
	const QString CircledY = QChar(0xE8, 0x24);
	const QString CircledZ = QChar(0xE9, 0x24);
	const QString Depth = QChar(0x73, 0x2B);
	const QString Diameter = QChar(0x00, 0x23);
	const QString Angle = QChar(0x20, 0x22);
	const QString LeftRightArrow = QChar(0x94, 0x21);
	const QString UpDownArrow = QChar(0x95, 0x21);
}
 
#endif // UnicodeSymbols_h__