#ifndef DiffUtility_h__
#define DiffUtility_h__

#include <QList>
#include <QString>

#include "DiffMatchPatch.h"

class DiffUtility
{
public:
	static void sideBySideDiff(QString& oldString, QString& newString, bool showDifferencesOnly = false, bool showWhitespacesChanges = false);
	static QString createDiffSummary(QString& oldString, QString& newString);

private:
	static void optimizeSummary(QString& summary);
	static void deleteEqual(QString& htmlDiff, bool keepLineBreaks = false);
	static void convertHtmlToPlainText(QString& htmlDiff);
};

#endif // DiffUtility_h__
