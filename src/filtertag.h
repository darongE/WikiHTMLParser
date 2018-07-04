#ifndef FILTERTAG_H
#define FILTERTAG_H

#include <QObject>


class FilterTag : public QObject
{
    Q_OBJECT

public:
    explicit FilterTag(QObject *parent = nullptr);
    ~FilterTag();

signals:

public slots:


public:

    void compareTag();

    void trimmedH2Tag(const QRegExp &divExp,const QRegExp &trimmedH3,
                  const QRegExp &tablerole, const QRegExp &tableclass);

    void trimmedH3Tag();
    void trimmedDIVTag();
    void trimmedTableTag();


    QStringList tagH2End;
    QStringList tagDIVStart;
    QStringList tagDIVEnd;
    QStringList tagH3Start;
    QStringList tagH3End;
    QStringList tagTableEndList;

    QStringList titleList;
    QStringList subTitleList;



};

#endif // FILTERTAG_H
