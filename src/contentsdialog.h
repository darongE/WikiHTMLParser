#ifndef CONTENTSDIALOG_H
#define CONTENTSDIALOG_H

#include <QDialog>
#include <QItemSelection>
#include <QStringListModel>
namespace Ui {
class ContentsDialog;
}

class ContentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContentsDialog(QWidget *parent = 0);
    ~ContentsDialog();

private slots:

    void init();
    void clear();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:

    Ui::ContentsDialog *ui;
    QModelIndex mContextIndex;
    QStringListModel *model;

    class Private;
    Private* const *d;
};

#endif // CONTENTSDIALOG_H
