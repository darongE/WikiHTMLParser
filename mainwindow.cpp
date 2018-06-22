
#include <qglobal.h>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QtGlobal>
#include <QToolButton>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "searchresultmodel.h"


static int QItemSelectionModel_selectedRowsCount(const QItemSelectionModel* const selectionModel)
{
    if (!selectionModel->hasSelection())
    {
        return 0;
    }

    return selectionModel->selectedRows().count();
}

class MainWindow::Private
{
public:

    explicit Private()
    {
       searchBackend               = 0;
       searchInProgress            = 0;
       searchResultsModel          = 0;
       searchResultsSelectionModel = 0;
    }

    //Search: backend

    SearchBackend*  searchBackend;
    SearchResultModel*       searchResultsModel;
    QItemSelectionModel* searchResultsSelectionModel;

    //Search: UI
    bool            searchInProgress;
    QAction*        actionClearResultsList;
    QAction*        actionRemovedSelectedSearchResultsFromList;


};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    d(new Private())
{
    ui->setupUi(this);

    d->searchBackend = new SearchBackend(this);
    d->searchResultsModel = new SearchResultModel(this);

    d->searchResultsSelectionModel = new QItemSelectionModel(d->searchResultsModel);
    d->searchResultsModel->setSelectionModel(d->searchResultsSelectionModel);

    d->actionClearResultsList = new QAction(QString("Clear the search results."), this);

    d->actionRemovedSelectedSearchResultsFromList = new QAction(QString("Remove from results list"), this);
    d->actionRemovedSelectedSearchResultsFromList->setIcon(QIcon::fromTheme(QString::fromLatin1("list-remove")));

    ui->treeView->setRootIsDecorated(false);
    ui->treeView->setModel(d->searchResultsModel);
    ui->treeView->setSelectionModel(d->searchResultsSelectionModel);
    ui->treeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //edit 키워드를 입력하면 검색 시작!!
    connect(ui->btnSearch, SIGNAL(pressed()),this, SLOT(slotTriggerSearch()));
  //  connect(ui->KeywordEdit, SIGNAL(textChanged(QString)),this, SLOT(slotUpdateAction()));
    connect(d->searchBackend, SIGNAL(signalSearchCompleted()), this, SLOT(slotSearchCompleted()));

    connect(d->actionClearResultsList, SIGNAL(triggered(bool)),
            this, SLOT(slotClearSearchResults()));

    connect(d->actionRemovedSelectedSearchResultsFromList, SIGNAL(triggered(bool)),
            this, SLOT(slotRemoveSelectedFromResultList()));


    ui->treeView->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotSearchCompleted()
{
    d->searchInProgress = false;
    const QString errorString = d->searchBackend->getErrorMessage();

    if(!errorString.isEmpty())
    {
        QMessageBox::critical(this,QString("Search failed"),QString("Your search failed:\n%1").arg(errorString));
        return;
    }

    const SearchBackend::SearchResult::List searchResults = d->searchBackend->getResults();
    d->searchResultsModel->addResults(searchResults);


    slotUpdateAction();


}

void MainWindow::slotTriggerSearch()
{
    // check Line edit text
    if(ui->LE_Keyword->text().isEmpty() || d->searchInProgress)
        return;

    d->searchInProgress = true;
    d->searchBackend->search("wikipedia",ui->LE_Keyword->text());

     slotUpdateAction();
}


void MainWindow::slotCurrentlySelectedResultChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

}




void MainWindow::slotClearSearchResults()
{
    d->searchResultsModel->clearResults();
    slotUpdateAction();
}

void MainWindow::slotUpdateAction()
{
    const int nSelectedResults       = QItemSelectionModel_selectedRowsCount(d->searchResultsSelectionModel);
    const bool haveOneSelectedResult = nSelectedResults == 1;

    d->actionRemovedSelectedSearchResultsFromList->setEnabled(nSelectedResults>=1);

    const bool haveSearchText        = !ui->LE_Keyword->text().isEmpty();

    //ui->btnSearch->setEnabled(haveSearchText && !d->searchInProgress);
    d->actionClearResultsList->setEnabled(d->searchResultsModel->rowCount()>0);

}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // make context-menu events
    if( watched == ui->treeView)
    {
        if(event->type() == QEvent::ContextMenu)
        {
            if(d->searchResultsSelectionModel->hasSelection())
            {
                const QModelIndex currentIndex = d->searchResultsSelectionModel->currentIndex();
                const SearchResultModel::SearchResultItem searchResult = d->searchResultsModel->resultItem(currentIndex);

            }


            slotUpdateAction();

            // construct the context-menu:
            QMenu* const menu = new QMenu(ui->treeView);
            menu->addAction(d->actionRemovedSelectedSearchResultsFromList);

            QContextMenuEvent* const e = static_cast<QContextMenuEvent*>(event);
             menu->exec(e->globalPos());
            delete menu;
        }
    }

     return QObject::eventFilter(watched, event);
}

void MainWindow::slotRemoveSelectedFromResultList()
{
    const QItemSelection selectedRows = d->searchResultsSelectionModel->selection();

    if (selectedRows.isEmpty())
    {
        return;
    }

    d->searchResultsModel->removeRowsBySelection(selectedRows);

    slotUpdateAction();
}











