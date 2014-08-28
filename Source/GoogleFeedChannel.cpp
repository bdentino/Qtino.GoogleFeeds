#include "GoogleFeedChannel.h"

#include "QtGoogleFeedApi.h"
#include "GoogleFeedHttpRequest.h"

GoogleFeedChannel::GoogleFeedChannel(QtGoogleFeedApi* api)
    : QObject(api),
      m_loaded(false),
      m_maxItems(10),
      m_api(api),
      m_request(NULL)
{

}

QtGoogleFeedApi* GoogleFeedChannel::api() { return m_api; }

void GoogleFeedChannel::setApi(QtGoogleFeedApi* api)
{
    if (m_api) {
        qWarning("Cannot change GoogleFeed API after initialization");
        return;
    }
    m_api = api;
    refresh();
}

void GoogleFeedChannel::copyAndDelete(GoogleFeedChannel* copy)
{
    m_feedUrl = copy->m_feedUrl;
    m_title = copy->m_title;
    m_author = copy->m_author;
    m_description = copy->m_description;
    m_type = copy->m_type;

    foreach (GoogleFeedItem* item, m_items)
        item->deleteLater();
    m_items.clear();

    m_items.append(copy->m_items);
    foreach (GoogleFeedItem* item, m_items)
        item->setChannel(this);

    copy->deleteLater();
    emit itemsChanged();
}

QUrl GoogleFeedChannel::feedUrl()
{
    return m_feedUrl;
}

QString GoogleFeedChannel::title()
{
    return m_title;
}

QUrl GoogleFeedChannel::siteUrl()
{
    return m_siteUrl;
}

QString GoogleFeedChannel::author()
{
    return m_author;
}

QString GoogleFeedChannel::description()
{
    return m_description;
}

QString GoogleFeedChannel::type()
{
    return m_type;
}

bool GoogleFeedChannel::isLoading()
{
    if (!m_loaded) return false;
    if (!m_request) return false;
    else return m_request->isLoading();
}

QQmlListProperty<GoogleFeedItem> GoogleFeedChannel::itemsProperty()
{
    return QQmlListProperty<GoogleFeedItem>(this, m_items);
}

QList<GoogleFeedItem*> GoogleFeedChannel::items()
{
    return m_items;
}

int GoogleFeedChannel::maxItems()
{
    return m_maxItems;
}

void GoogleFeedChannel::setMaxItems(int maxItems)
{
    if (maxItems == m_maxItems) return;
    m_maxItems = maxItems;
    refresh();
    emit maxItemsChanged();
}

void GoogleFeedChannel::refresh()
{
    if (!m_api) { return; }
    if (!m_feedUrl.isValid()) { return; }
    if (m_request) {
        disconnect(m_request);
        m_request->disconnect(this);
        m_request->cancel();
        m_request->deleteLater();
    }
    m_request = m_api->getLoadRequest(m_feedUrl, m_maxItems);
    connect(m_request, SIGNAL(responseReady(QJsonObject)),
            this, SLOT(onResponseReceived(QJsonObject)));
    m_request->send();
    emit loadingChanged();
}

void GoogleFeedChannel::onResponseReceived(QJsonObject response)
{
    foreach (GoogleFeedItem* item, m_items)
        item->deleteLater();
    m_items.clear();

    GoogleFeedChannel* channel = m_api->parseFeedChannel(response);

    m_items.append(channel->items());
    foreach (GoogleFeedItem* item, channel->items())
        item->setChannel(this);
    channel->deleteLater();

    emit itemsChanged();
    emit loadingChanged();
}

void GoogleFeedChannel::onError(QNetworkReply::NetworkError error, QString description)
{
    Q_UNUSED(error);
    qWarning("GoogleFeedChannel Network Error: %s", qPrintable(description));
}
