#ifndef FRMSEARCHREPLACE_H
#define FRMSEARCHREPLACE_H

#include "include/topeditorcontainer.h"
#include "include/Search/filesearchresult.h"
#include "include/Search/searchinfilesworker.h"
#include "include/Search/replaceinfilesworker.h"
#include "include/Search/searchhelpers.h"
#include <QDialog>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QMessageBox>

namespace Ui {
class frmSearchReplace;
}

class frmSearchReplace : public QMainWindow
{
    Q_OBJECT

public:
    enum Tabs { TabSearch, TabReplace, TabSearchInFiles };
    explicit frmSearchReplace(TopEditorContainer *topEditorContainer, QWidget *parent = 0);
    ~frmSearchReplace();
    void show(Tabs defaultTab);

    /**
     * @brief Runs a "find next" or "find prev", taking the options from the UI
     * @param forward
     */
    void findFromUI(bool forward, bool searchFromStart = false);
    void replaceFromUI(bool forward, bool searchFromStart = false);

    static QString rawSearchString(QString search, SearchHelpers::SearchMode searchMode, SearchHelpers::SearchOptions searchOptions);
    static QString plainTextToRegex(QString text, bool matchWholeWord);

    void cleanFindInFilesPtrs();
protected:
    void keyPressEvent(QKeyEvent *evt);

private slots:
    void on_btnFindNext_clicked();
    void on_btnFindPrev_clicked();
    void on_btnReplaceNext_clicked();
    void on_btnReplacePrev_clicked();
    void on_btnReplaceAll_clicked();
    void on_btnSelectAll_clicked();
    void on_actionFind_toggled(bool on);
    void on_actionReplace_toggled(bool on);
    void on_actionFind_in_files_toggled(bool on);
    void on_chkShowAdvanced_toggled(bool checked);
    void on_radSearchWithRegex_toggled(bool checked);
    void on_radSearchPlainText_toggled(bool checked);
    void on_radSearchWithSpecialChars_toggled(bool checked);
    void on_searchStringEdited(const QString &text);
    void on_btnFindAll_clicked();
    void on_btnLookInBrowse_clicked();
    void on_btnReplaceAllInFiles_clicked();

private:
    Ui::frmSearchReplace*  ui;
    TopEditorContainer*    m_topEditorContainer;
    QString                m_lastSearch;

    class SearchInFilesSession : public QObject {
    public:
        SearchInFilesSession(QObject *parent) : QObject(parent) { }

        QThread*               threadSearch = nullptr;
        SearchInFilesWorker*   workerSearch = nullptr;
        QThread*               threadReplace = nullptr;
        ReplaceInFilesWorker*  workerReplace = nullptr;
        QMessageBox*           msgBox = nullptr;
    };

    QList<SearchInFilesSession*> m_findInFilesPtrs;

    Editor*                currentEditor();

    void search(QString string, SearchHelpers::SearchMode searchMode, bool forward, SearchHelpers::SearchOptions searchOptions);
    void replace(QString string, QString replacement, SearchHelpers::SearchMode searchMode, bool forward, SearchHelpers::SearchOptions searchOptions);
    int replaceAll(QString string, QString replacement, SearchHelpers::SearchMode searchMode, SearchHelpers::SearchOptions searchOptions);
    int selectAll(QString string, SearchHelpers::SearchMode searchMode, SearchHelpers::SearchOptions searchOptions);
    void searchInFiles(const QString &string, const QString &path, const QStringList &filters, const SearchHelpers::SearchMode &searchMode, const SearchHelpers::SearchOptions &searchOptions);
    void replaceInFiles(const QString &string, QString replacement, const QString &path, const QStringList &filters, const SearchHelpers::SearchMode &searchMode, const SearchHelpers::SearchOptions &searchOptions);
    void setCurrentTab(Tabs tab);
    void manualSizeAdjust();
    SearchHelpers::SearchOptions searchOptionsFromUI();
    SearchHelpers::SearchMode searchModeFromUI();
    QString regexModifiersFromSearchOptions(SearchHelpers::SearchOptions searchOptions);
    FileSearchResult::Result buildResult(const QRegularExpressionMatch &match, QString *content);
    QStringList fileFiltersFromUI();
signals:
    void fileSearchResultFinished(FileSearchResult::SearchResult result);
};

#endif // FRMSEARCHREPLACE_H
