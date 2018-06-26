#include "contentsdialog.h"
#include "ui_contentsdialog.h"
#include "searchbackend.h"
#include <QDebug>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>



class ContentsDialog::Private
{
public:
    explicit Private()
    {
        searchBackend  = 0;

    }

    SearchBackend*     searchBackend;
    QByteArray          searchData;


};

ContentsDialog::ContentsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContentsDialog),
    model(Q_NULLPTR)
{
    ui->setupUi(this);

    connect(ui->clearButton, &QPushButton::clicked, this, &ContentsDialog::clear);
    QTimer::singleShot(0, this,&ContentsDialog::init);
}

ContentsDialog::~ContentsDialog()
{
    delete ui;
}

void ContentsDialog::init()
{

    QStandardItemModel *mod = new QStandardItemModel;
    QStandardItem *it = new QStandardItem("Contents");
    mod->setHorizontalHeaderItem(0,it);


    ui->tableView->setModel(mod);
    ui->tableView->show();
    ui->tableView->setWordWrap(false);

    ui->tableView->horizontalHeader()->setSectionsClickable(false);
    ui->tableView->horizontalHeader()->setSectionsMovable(false);

    // one column
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    ui->tableView->setTextElideMode(Qt::ElideLeft);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->setSortingEnabled(false);

    if (ui->tableView->model()->rowCount() > 0) {
        ui->tableView->setEnabled(true);
        ui->clearButton->setEnabled(true);
    }

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ContentsDialog::selectionChanged);

    setUpdatesEnabled(true);

}

void ContentsDialog::clear()
{
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Clear", "Don't Clear",
        QMessageBox::Yes | QMessageBox::No );

    if (reply == QMessageBox::No)
        return;


    ui->tableView->setEnabled(false);
    ui->clearButton->setEnabled(false);
}


void ContentsDialog::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    if(selected.indexes().count() == 0)
        return;

    QModelIndex index = selected.indexes().at(0);

    QString text;
    // check column

    if(index.column() == 0){

       text = index.data().toString();
       qDebug() <<  index.row() << text;
    }

}



// print out contents






