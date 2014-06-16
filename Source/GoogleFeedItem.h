#ifndef GOOGLEFEEDITEM_H
#define GOOGLEFEEDITEM_H

#include <QObject>

#include "QtGoogleFeeds_Global.h"

class QTGOOGLEFEEDSSHARED_EXPORT GoogleFeedItem : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title)

public:
    explicit GoogleFeedItem(QObject *parent = 0);

signals:

public slots:

};

#endif // GOOGLEFEEDITEM_H
