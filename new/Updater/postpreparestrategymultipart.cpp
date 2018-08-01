#include "postpreparestrategymultipart.h"

PostPrepareStrategyMultipart::PostPrepareStrategyMultipart(QObject *parent) :
    IPostPrepareStrategy(parent)
{

}

QByteArray PostPrepareStrategyMultipart::GenerateData(const QHash<QString, ContentData> & params)
{
    QByteArray data;
    QList<QString> keys = params.keys();


}
