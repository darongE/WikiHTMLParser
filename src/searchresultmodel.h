#ifndef SEARCHRESULTMODEL_H
#define SEARCHRESULTMODEL_H

// Qt includes
#include <QAbstractItemModel>
#include <QItemSelectionModel>

#include "searchbackend.h"

class QItemSelection;
class SearchResultItem;

class SearchResultModel : public QAbstractItemModel
{


public:

    class SearchResultItem
    {
    public:
        SearchBackend::SearchResult result;
    };


public:

    explicit SearchResultModel(QObject* const parent = 0);
    ~SearchResultModel();

    void addResults(const SearchBackend::SearchResult::List& results);
    SearchResultItem resultItem(const QModelIndex& index) const;
    void setSelectionModel(QItemSelectionModel* const selectionModel);
    void clearResults();
    void removeRowsByIndexes(const QModelIndex& rowsList);
    void removeRowsBySelection(const QItemSelection& selection);

    // QAbstractItemModel:
    virtual int columnCount(const QModelIndex& parent = QModelIndex() ) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent(const QModelIndex& index) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

private:
    class Private;
    Private* const d;

};

#endif // SEARCHRESULTMODEL_H
