#include "QtGoogleFeedApi.h"
#include "GoogleFeedHttpRequest.h"
#include "GoogleFeedQuery.h"
#include "GoogleFeedQueryResult.h"
#include "GoogleFeedChannel.h"
#include "GoogleFeedChannelLoader.h"
#include "GoogleFeedItem.h"

#include <QtQml>

const QString QtGoogleFeedApi::BaseUrl = "https://ajax.googleapis.com/ajax/services/feed";

QtGoogleFeedApi::QtGoogleFeedApi(QObject* parent)
    : QObject(parent)
{
    qRegisterMetaType<GoogleFeedQuery*>();
    qRegisterMetaType<GoogleFeedQueryResult*>();
    qRegisterMetaType<GoogleFeedChannel*>();
    qRegisterMetaType<GoogleFeedItem*>();

    qmlRegisterType<GoogleFeedQuery>();
    qmlRegisterType<GoogleFeedQueryResult>();
    qmlRegisterType<GoogleFeedChannel>();
    qmlRegisterType<GoogleFeedItem>();
}

QtGoogleFeedApi::QtGoogleFeedApi(GoogleApiVersion version, QObject* parent)
    : QObject(parent),
      m_version(version)
{
    qRegisterMetaType<GoogleFeedQuery*>();
    qRegisterMetaType<GoogleFeedQueryResult*>();
    qRegisterMetaType<GoogleFeedChannel*>();
    qRegisterMetaType<GoogleFeedItem*>();

    qmlRegisterType<GoogleFeedQuery>();
    qmlRegisterType<GoogleFeedQueryResult>();
    qmlRegisterType<GoogleFeedChannel>();
    qmlRegisterType<GoogleFeedItem>();
}

GoogleFeedQuery* QtGoogleFeedApi::getQueryObject()
{
    return new GoogleFeedQuery(this);
}

GoogleFeedHttpRequest* QtGoogleFeedApi::getFindRequest(QString query)
{
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("v", versionString());
    urlQuery.addQueryItem("q", query);

    QUrl url(QtGoogleFeedApi::BaseUrl + "/find");
    url.setQuery(urlQuery);
    return new GoogleFeedHttpRequest(url);
}

QJsonObject QtGoogleFeedApi::getResponseData(QJsonObject response, QString* parseError)
{
    if (parseError == 0) { QString tempError; parseError = &tempError; }

    int responseCode = response["responseStatus"].toInt();
    if (responseCode != 200)
    {
        *parseError = response["responseDetails"].toString();
        if (*parseError == "")
            *parseError = QString("Invalid Response Status: %1").arg(responseCode);
        return QJsonObject();
    }
    return response["responseData"].toObject();
}

//TODO: Separate class for Parsed result (allows managing error info)
QList<GoogleFeedQueryResult*> QtGoogleFeedApi::parseFindResponse(QJsonObject response, QString* parseError)
{
    if (parseError == 0) { QString tempError; parseError = &tempError; }

    QList<GoogleFeedQueryResult*> results;
    QJsonObject responseData = getResponseData(response, parseError);
    if (responseData.isEmpty()) {
        return results;
    }

    QJsonArray entries = responseData["entries"].toArray();
    foreach (QJsonValue value, entries)
    {
        QJsonObject entry = value.toObject();

        // TODO: Instead of having a builder, I should just have a constructor
        // (or virtual function) that takes a QJsonObject... (like GoogleFeedItem)
        GoogleFeedQueryResultBuilder builder(this);

        builder.setUrl(QUrl(entry["url"].toString()));
        builder.setTitle(entry["title"].toString());
        builder.setLink(QUrl(entry["link"].toString()));
        builder.setContentSnippet(entry["contentSnippet"].toString());

        results.append(builder.createResultObject());
    }
    return results;
}

GoogleFeedChannel* QtGoogleFeedApi::parseFeedChannel(QJsonObject response, QString *parseError)
{
    if (parseError == 0) { QString tempError; parseError = &tempError; }

    // TODO: Same thing here...should rework this so GoogleFeedChannel takes a JSON object
    // and builds itself...only problem is that the api interface returns a feed before it's fully
    // loaded so I will still need a way of constructing it without knowing its properties yet
    GoogleFeedChannel* channel = new GoogleFeedChannel(this);
    QJsonObject feed = getResponseData(response, parseError)["feed"].toObject();

    channel->m_author = feed["author"].toString();
    channel->m_description = feed["description"].toString();
    channel->m_feedUrl = QUrl(feed["feedUrl"].toString());
    channel->m_siteUrl = QUrl(feed["link"].toString());
    channel->m_title = feed["title"].toString();
    channel->m_description = feed["description"].toString();

    channel->m_items = parseFeedItems(channel, feed["entries"].toArray());
    return channel;
}

QList<GoogleFeedItem*> QtGoogleFeedApi::parseFeedItems(GoogleFeedChannel* channel, QJsonArray items)
{
    QList<GoogleFeedItem*> feedItems;
    foreach (QJsonValue item, items)
    {
        feedItems << new GoogleFeedItem(channel, item.toObject());
    }
    return feedItems;
}

GoogleFeedChannel* QtGoogleFeedApi::loadFeed(QUrl url, int maxItems)
{
    GoogleFeedChannel* channel = new GoogleFeedChannel(this);
    channel->setMaxItems(maxItems);
    channel->m_feedUrl = url;
    GoogleFeedChannelLoader* loader = new GoogleFeedChannelLoader(this, channel);
    connect(loader, SIGNAL(finishedLoading()), loader, SLOT(deleteLater()));
    loader->start();
    return channel;
}

GoogleFeedHttpRequest* QtGoogleFeedApi::getLoadRequest(QUrl feedUrl, int maxItems)
{
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("v", versionString());
    urlQuery.addQueryItem("q", QUrl::toPercentEncoding(feedUrl.toString()));
    urlQuery.addQueryItem("num", QString("%1").arg(maxItems));

    QUrl url(QtGoogleFeedApi::BaseUrl + "/load");
    url.setQuery(urlQuery);
    return new GoogleFeedHttpRequest(url);
}

void QtGoogleFeedApi::setVersion(QString version)
{
    GoogleApiVersion defaultVersion;
    if (m_version != defaultVersion) {
        qWarning("Cannot change GoogleFeed API version after initialization.");
        return;
    }
    m_version = GoogleApiVersion(version);
}

QString QtGoogleFeedApi::versionString()
{
    return QString("%1.%2").arg(m_version.majorVersion).arg(m_version.minorVersion);
}

GoogleApiVersion QtGoogleFeedApi::version()
{
    return m_version;
}
