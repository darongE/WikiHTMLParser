#include "contentsdialog.h"
#include "ui_contentsdialog.h"

ContentsDialog::ContentsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ContentsDialog)
{
    ui->setupUi(this);
}

ContentsDialog::~ContentsDialog()
{
    delete ui;
}
