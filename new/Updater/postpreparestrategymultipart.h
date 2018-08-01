#ifndef POSTPREPARESTRATEGYMULTIPART_H
#define POSTPREPARESTRATEGYMULTIPART_H
#include "ipostpreparestrategy.h"

class PostPrepareStrategyMultipart : public IPostPrepareStrategy
{
    Q_OBJECT
    QString boundary;

public:
    explicit PostPrepareStrategyMultipart(QObject *parent = nullptr);

    virtual QByteArray GenerateData(const QHash<QString, ContentData> & params);
    virtual QByteArray GetContentType();


};


#endif // POSTPREPARESTRATEGYMULTIPART_H
