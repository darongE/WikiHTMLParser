#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractItemModel>
#include <QMimeData>
#include <QWidget>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void slotSearchCompleted();
    void slotTriggerSearch();
    void slotCurrentlySelectedResultChanged(const QModelIndex& current
                                            ,const QModelIndex& previous );
    void slotUpdateAction();
    void slotRemoveSelectedFromResultList();
    void slotClearSearchResults();

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWindow *ui;

    class Private;
    Private* const d;
};

#endif // MAINWINDOW_H
