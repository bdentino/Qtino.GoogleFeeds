#include "GoogleFeedChannel.h"

#include "QtGoogleFeedApi.h"
#include "GoogleFeedHttpRequest.h"

GoogleFeedChannel::GoogleFeedChannel(QtGoogleFeedApi* api)
    : QObject(api),
      m_loaded(false),
      m_api(api),
      m_request(NULL)
{

}

GoogleFeedChannel& GoogleFeedChannel::operator =(GoogleFeedChannel& copy)
{
    m_feedUrl = copy.m_feedUrl;
    m_title = copy.m_title;
    m_author = copy.m_author;
    m_description = copy.m_description;
    m_type = copy.m_type;

    m_items.clear();
    m_items.append(copy.m_items);
    emit loadingChanged();
    emit itemsChanged();
    return *this;
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

void GoogleFeedChannel::refresh()
{
    if (m_request) {
        disconnect(m_request);
        m_request->disconnect(this);
        m_request->cancel();
        m_request->deleteLater();
    }
    m_request = m_api->getLoadRequest(m_feedUrl);
    connect(m_request, SIGNAL(responseReady(QJsonObject)),
            this, SLOT(onResponseReceived(QJsonObject)));
    m_request->send();
}

void GoogleFeedChannel::onResponseReceived(QJsonObject response)
{
    m_items.clear();
    GoogleFeedChannel* channel = m_api->parseFeedChannel(response);
    m_items.append(channel->items());
    channel->deleteLater();
    qDebug() << m_items.count();
    emit itemsChanged();
}

void GoogleFeedChannel::onError(QNetworkReply::NetworkError error, QString description)
{
    Q_UNUSED(error);
    qWarning("GoogleFeedChannel Network Error: %s", qPrintable(description));
}
