#ifndef ACI_COLOR_SCHEME
#define ACI_COLOR_SCHEME

#include <QColor>

class AciColorScheme
{
	public:
		static QColor getColor(int colorIndex);
		static int    getColorIndex(const QColor &color);
};

#endif
