#include "searchresultmodel.h"


#include <QContextMenuEvent>
#include <QPainter>
#include <QAction>
#include <QStandardPaths>


static bool RowRangeLessThan(const QPair<int, int>& a, const QPair<int, int>& b)
{
    return a.first < b.first;
}

class SearchResultModel::Private
{
public:
    explicit Private()
    {
        selectionModel = 0;
    }

    QList<SearchResultModel::SearchResultItem> searchResults;
    QItemSelectionModel*  selectionModel;
};



SearchResultModel::SearchResultModel(QObject * const parent)
    : QAbstractItemModel(parent),
      d(new Private())
{

}

SearchResultModel::~SearchResultModel()
{
    delete d;
}

int SearchResultModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;

}

bool SearchResultModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;

}


QVariant SearchResultModel::data(const QModelIndex &index, int role) const
{
    const int rowNumber = index.row();

    if((rowNumber < 0) || (rowNumber >= d->searchResults.count()))
    {
        return QVariant();
    }

    const int columnNumber = index.column();

    if(columnNumber == 0)
    {
        switch (role)
        {/*
        QString str("I am a long long long"
            "long long long"
            "long QString");*/
            case Qt::DisplayRole:


            return    QString("%1  " "%2" ).arg(d->searchResults.at(rowNumber).result.index)
                                        .arg(d->searchResults.at(rowNumber).result.text);


            default:
                return QVariant();

        }
    }

    return QVariant();
}


QModelIndex SearchResultModel::index(int row, int column, const QModelIndex &parent) const
{
    if(parent.isValid())
        return QModelIndex();

    if((column < 0) || (column >= 1) || (row < 0) || (row >= d->searchResults.count()) )
    {
        return QModelIndex();
    }

    return createIndex(row, column, (void*)0);
}

QModelIndex SearchResultModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);

    return QModelIndex();
}

int SearchResultModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return d->searchResults.count();
}

bool SearchResultModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(value)
    Q_UNUSED(role)

    return false;
}

QVariant SearchResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(role)

    if ((section >= 1) || (orientation != Qt::Horizontal))
    {
        return false;
    }

    return QVariant(QString::fromLatin1("Name"));
}

Qt::ItemFlags SearchResultModel::flags(const QModelIndex& index) const
{
    return QAbstractItemModel::flags(index);
}


void SearchResultModel::addResults(const SearchBackend::SearchResult::List& results)
{
    // first check which items are not duplicates
    QList<int> nonDuplicates;

    for (int i = 0; i < results.count(); ++i)
    {
        const SearchBackend::SearchResult& currentResult = results.at(i);
        bool isDuplicate                                 = false;

        for (int j = 0; j < d->searchResults.count(); ++j)
        {
            if (currentResult.index == d->searchResults.at(j).result.index)
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate)
        {
            nonDuplicates << i;
        }
    }

    if (nonDuplicates.isEmpty())
    {
        return;
    }

    beginInsertRows(QModelIndex(), d->searchResults.count(), d->searchResults.count()+nonDuplicates.count()-1);

    for (int i = 0; i < nonDuplicates.count(); ++i)
    {
        SearchResultItem item;
        item.result = results.at(nonDuplicates.at(i));
        d->searchResults << item;
    }

    endInsertRows();
}


SearchResultModel::SearchResultItem SearchResultModel::resultItem(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return SearchResultItem();
    }

    return d->searchResults.at(index.row());
}

void SearchResultModel::setSelectionModel(QItemSelectionModel* const selectionModel)
{
    d->selectionModel = selectionModel;
}

void SearchResultModel::clearResults()
{
    beginResetModel();
    d->searchResults.clear();
    endResetModel();
}

void SearchResultModel::removeRowsByIndexes(const QModelIndex& rowsList)
{
    // extract the row numbers first:
    QList<int> rowNumbers;

   while (rowsList.isValid())
   {

      rowNumbers << rowsList.row();

   }

    if (rowNumbers.isEmpty())
    {
        return;
    }

    std::sort(rowNumbers.begin(), rowNumbers.end());

    // now delete the rows, starting with the last row:
    for (int i = rowNumbers.count()-1; i >= 0; i--)
    {
        const int rowNumber = rowNumbers.at(i);

        /// @todo This is very slow for several indexes, because the views update after every removal
        beginRemoveRows(QModelIndex(), rowNumber, rowNumber);
        d->searchResults.removeAt(rowNumber);
        endRemoveRows();
    }
}

void SearchResultModel::removeRowsBySelection(const QItemSelection& selectionList)
{
    // extract the row numbers first:
    QList<QPair<int, int> > rowRanges;

    foreach(const QItemSelectionRange& range, selectionList)
    {
        rowRanges << QPair<int, int>(range.top(), range.bottom());
    }

    // we expect the ranges to be sorted here
    std::sort(rowRanges.begin(), rowRanges.end(), RowRangeLessThan);

    // now delete the rows, starting with the last row:
    for (int i = rowRanges.count()-1; i >= 0; i--)
    {
        const QPair<int, int> currentRange = rowRanges.at(i);

        /// @todo This is very slow for several indexes, because the views update after every removal
        beginRemoveRows(QModelIndex(), currentRange.first, currentRange.second);

        for (int j = currentRange.second; j >= currentRange.first; j--)
        {
            d->searchResults.removeAt(j);
        }

        endRemoveRows();
    }
}








