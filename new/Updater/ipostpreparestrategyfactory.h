#ifndef IPOSTPREPARESTRATEGYFACTORY_H
#define IPOSTPREPARESTRATEGYFACTORY_H

#include <QObject>

class IPostPrepareStrategy;

class IPostPrepareStrategyFactory : public QObject
{
    Q_OBJECT
public:
    explicit IPostPrepareStrategyFactory(QObject *parent = nullptr);
    virtual IPostPrepareStrategy* Create(const QString& type) = 0;

signals:

public slots:
};

#endif // IPOSTPREPARESTRATEGYFACTORY_H
