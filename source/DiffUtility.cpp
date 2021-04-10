#include "DiffUtility.h"

#include <QTextEdit>

const QString kColorNew = "FF8566"; 
const QString kColorOld = "A3FFA3"; 

void DiffUtility::sideBySideDiff(QString& oldString, QString& newString, bool showDifferencesOnly /*= false*/, bool showWhitespacesChanges /*= false*/)
{
	DiffMatchPatch diffMatchPatchTool;

	QList<Diff> differences = diffMatchPatchTool.diff_main(oldString, newString);
	diffMatchPatchTool.diff_cleanupEfficiency(differences);

	if(showDifferencesOnly)
	{
		diffMatchPatchTool.diff_prettyHtmlSideBySide(differences, oldString, newString, kColorOld, kColorNew, false, false);

		DiffUtility::deleteEqual(oldString, true);
		DiffUtility::deleteEqual(newString, true);

		differences = diffMatchPatchTool.diff_main(oldString, newString);
	}
	
	diffMatchPatchTool.diff_prettyHtmlSideBySide(differences, oldString, newString, kColorOld, kColorNew, showWhitespacesChanges);
}

void DiffUtility::deleteEqual(QString& htmlDiff, bool keepLineBreaks /*= false*/)
{
	QStringList htmlLines = htmlDiff.split("<br>", QString::KeepEmptyParts);
	htmlDiff = "";
	bool delTagClosed = true;
	bool insTagClosed = true;
	bool lineAppended = false;

	for (auto line : htmlLines)
	{
		lineAppended = false;

		if(line.contains(kColorNew) || !delTagClosed )
		{
			htmlDiff.append(line);
			lineAppended = true;

			keepLineBreaks ? htmlDiff.append("<br>") : htmlDiff.append("; ");

			if(line.contains("</del>"))
				delTagClosed = true;
			else
				delTagClosed = false;
		}

		if(line.contains(kColorOld) || !insTagClosed)
		{
			if(!lineAppended)
			{
				htmlDiff.append(line);
				keepLineBreaks ? htmlDiff.append("<br>") : htmlDiff.append("; ");
			}

			if(line.contains("</ins>"))
				insTagClosed = true;
			else
				insTagClosed = false;
		}
	}
	if(keepLineBreaks)
		convertHtmlToPlainText(htmlDiff);
}

QString DiffUtility::createDiffSummary(QString& oldString, QString& newString)
{	
	DiffMatchPatch diffMatchPatchTool;

	QList<Diff> differences = diffMatchPatchTool.diff_main(oldString, newString);

	diffMatchPatchTool.diff_prettyHtmlSideBySide(differences, oldString, newString, kColorOld, kColorNew, false, false);

	DiffUtility::optimizeSummary(oldString);
	DiffUtility::optimizeSummary(newString);

	QString summary = oldString.append(newString);

	QStringList changes = summary.split(";",QString::SkipEmptyParts);
	changes.removeDuplicates();
	changes.sort();

	summary = "";

	for (auto change : changes)
		summary.append(change).append("; ");

	return summary.simplified();
}

void DiffUtility::optimizeSummary(QString& summary)
{
	DiffUtility::deleteEqual(summary);

	DiffUtility::convertHtmlToPlainText(summary);

	QStringList summaryEntries = summary.split(";", QString::SkipEmptyParts);
	summary = "";

	for (auto entry : summaryEntries)
	{
		if(entry.contains("="))
			summary.append(entry.left(entry.indexOf("="))).append(";");
		else if(entry.contains(":"))
			summary.append(entry).append(";");
	}

	DiffUtility::convertHtmlToPlainText(summary);

	summary = summary.simplified();
	summary.replace(" ;", ";");
}

void DiffUtility::convertHtmlToPlainText(QString& htmlDiff)
{
	QTextEdit htmlConverter;
	htmlConverter.setHtml(htmlDiff);
	htmlDiff = htmlConverter.toPlainText();
}
