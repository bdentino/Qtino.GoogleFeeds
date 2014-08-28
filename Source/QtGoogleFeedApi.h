#ifndef QTGOOGLEFEEDS_H
#define QTGOOGLEFEEDS_H

#include "QtGoogleFeeds_Global.h"
#include <QObject>
#include <QStringList>
#include <QUrl>

struct QTGOOGLEFEEDSSHARED_EXPORT GoogleApiVersion {
    unsigned short majorVersion;
    unsigned short minorVersion;

    GoogleApiVersion(QString version)
    {
        QStringList parts = version.split(".");
        if (parts.length() < 1) return;
        majorVersion = parts.at(0).toShort();
        if (parts.length() < 2) return;
        minorVersion = parts.at(1).toShort();
    }

    GoogleApiVersion()
    {
        majorVersion = 0;
        minorVersion = 0;
    }

    QString toString() const
    {
        return QString("v%1.%2").arg(majorVersion).arg(minorVersion);
    }

    bool operator==(const GoogleApiVersion& rhs)
    {
        return majorVersion == rhs.majorVersion && minorVersion == rhs.minorVersion;
    }

    bool operator!=(const GoogleApiVersion& rhs)
    {
        return !(*this == rhs);
    }
};

class GoogleFeedQuery;
class GoogleFeedHttpRequest;
class GoogleFeedQueryResult;
class GoogleFeedChannel;
class GoogleFeedChannelLoader;
class GoogleFeedItem;
class QTGOOGLEFEEDSSHARED_EXPORT QtGoogleFeedApi : public QObject
{
    Q_OBJECT

    friend class GoogleFeedQuery;
    friend class GoogleFeedChannel;
    friend class GoogleFeedChannelLoader;

    Q_PROPERTY(QString version READ versionString WRITE setVersion)

public:
    QtGoogleFeedApi(QObject* parent = 0);
    QtGoogleFeedApi(GoogleApiVersion version, QObject* parent = 0);

    GoogleFeedQuery* getQueryObject();
    GoogleFeedChannel* loadFeed(QUrl feedUrl, int maxItems = 10);

    void setVersion(QString version);
    QString versionString();
    GoogleApiVersion version();

protected:
    GoogleFeedHttpRequest* getFindRequest(QString query);
    GoogleFeedHttpRequest* getLoadRequest(QUrl feedUrl, int maxItems);

    QList<GoogleFeedQueryResult*> parseFindResponse(QJsonObject response, QString* parseError = 0);
    GoogleFeedChannel* parseFeedChannel(QJsonObject response, QString* parseError = 0);
    QList<GoogleFeedItem*> parseFeedItems(GoogleFeedChannel* channel, QJsonArray items);

    QJsonObject getResponseData(QJsonObject response, QString* parseError = 0);

private:
    GoogleApiVersion m_version;
    static const QString BaseUrl;
};

#endif // QTGOOGLEFEEDS_H
