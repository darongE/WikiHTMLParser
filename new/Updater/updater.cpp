#include "updater.h"
#include "ui_updater.h"

Updater::Updater(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Updater)
{
    ui->setupUi(this);
}

Updater::~Updater()
{
    delete ui;
}
