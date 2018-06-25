#ifndef CONTENTSDIALOG_H
#define CONTENTSDIALOG_H

#include <QDialog>

namespace Ui {
class ContentsDialog;
}

class ContentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ContentsDialog(QWidget *parent = 0);
    ~ContentsDialog();

private:
    Ui::ContentsDialog *ui;
};

#endif // CONTENTSDIALOG_H
