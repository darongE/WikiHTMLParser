#ifndef UPDATER_H
#define UPDATER_H

#include <QWidget>

namespace Ui {
class Updater;
}

class Updater : public QWidget
{
    Q_OBJECT

public:
    explicit Updater(QWidget *parent = 0);
    ~Updater();

private:
    Ui::Updater *ui;
};

#endif // UPDATER_H
