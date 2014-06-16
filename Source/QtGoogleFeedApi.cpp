#include "QtGoogleFeedApi.h"
#include "GoogleFeedHttpRequest.h"
#include "GoogleFeedQuery.h"
#include "GoogleFeedQueryResult.h"
#include "GoogleFeedChannel.h"
#include "GoogleFeedChannelLoader.h"
#include "GoogleFeedItem.h"

#include <QtQml>

const QString QtGoogleFeedApi::BaseUrl = "https://ajax.googleapis.com/ajax/services/feed";

QtGoogleFeedApi::QtGoogleFeedApi(GoogleApiVersion version, QObject* parent)
    : QObject(parent),
      m_version(version)
{
    qRegisterMetaType<GoogleFeedQuery*>();
    qRegisterMetaType<GoogleFeedQueryResult*>();
    qRegisterMetaType<GoogleFeedChannel*>();

    qmlRegisterType<GoogleFeedQuery>();
    qmlRegisterType<GoogleFeedQueryResult>();
    qmlRegisterType<GoogleFeedChannel>();
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

// @info
//   Putting this logic behind a function in the Api class so that versioning differences
//   can be handled without having to change Query/Load objects

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

    GoogleFeedChannel* channel = new GoogleFeedChannel(this);
    QJsonObject feed = getResponseData(response, parseError)["feed"].toObject();

    channel->m_author = feed["author"].toString();
    channel->m_description = feed["description"].toString();
    channel->m_feedUrl = QUrl(feed["feedUrl"].toString());
    channel->m_siteUrl = QUrl(feed["link"].toString());
    channel->m_title = feed["title"].toString();
    channel->m_description = feed["description"].toString();

    channel->m_items = parseFeedItems(feed["entries"].toArray(), parseError);
    return channel;
}

QList<GoogleFeedItem*> QtGoogleFeedApi::parseFeedItems(QJsonArray items, QString* parseError)
{
    if (parseError == 0) { QString tempError; parseError = &tempError; }
    qDebug() << items;
    return QList<GoogleFeedItem*>();
}

GoogleFeedChannel* QtGoogleFeedApi::loadFeed(QUrl url)
{
    GoogleFeedChannel* channel = new GoogleFeedChannel(this);
    channel->m_feedUrl = url;
    GoogleFeedChannelLoader* loader = new GoogleFeedChannelLoader(this, channel);
    connect(loader, SIGNAL(finishedLoading()), loader, SLOT(deleteLater()));
    loader->start();
    return channel;
}

GoogleFeedHttpRequest* QtGoogleFeedApi::getLoadRequest(QUrl feedUrl)
{
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("v", versionString());
    urlQuery.addQueryItem("q", feedUrl.toString());

    QUrl url(QtGoogleFeedApi::BaseUrl + "/load");
    url.setQuery(urlQuery);
    return new GoogleFeedHttpRequest(url);
}

QString QtGoogleFeedApi::versionString()
{
    return QString("%1.%2").arg(m_version.majorVersion).arg(m_version.minorVersion);
}

GoogleApiVersion QtGoogleFeedApi::version()
{
    return m_version;
}
