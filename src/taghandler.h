#ifndef TAGHANDLER_H
#define TAGHANDLER_H

#include <QString>
#include "searchbackend.h"
/**
 * @brief The TagHandler class
 *
 * compare tag
*/

class TagHandler
{

    // access variable
public:

    QStringList tagH2End;
    QStringList tagDIVStart;
    QStringList tagDIVEnd;
    QStringList tagH3Start;
    QStringList tagH3End;

    QStringList titleList;
    QStringList subTitleList;


public:
    TagHandler();
    ~TagHandler();

    void compareTag();
    void eraseTag(const QRegExp &divExp,const QRegExp &trimmedH3,
                  const QRegExp &tablerole, const QRegExp &tableclass);
    void pushH2Tag();

};

#endif // TAGHANDLER_H
