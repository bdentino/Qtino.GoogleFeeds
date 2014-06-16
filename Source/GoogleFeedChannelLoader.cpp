#include "GoogleFeedChannelLoader.h"

#include "QtGoogleFeedApi.h"
#include "GoogleFeedChannel.h"
#include "GoogleFeedHttpRequest.h"

GoogleFeedChannelLoader::GoogleFeedChannelLoader(QtGoogleFeedApi* api, GoogleFeedChannel* channel) :
    QObject(api),
    m_api(api),
    m_channel(channel)
{
    m_request = m_api->getLoadRequest(m_channel->feedUrl());
    connect(m_request, SIGNAL(responseReady(QJsonObject)),
            this, SLOT(onResponseReceived(QJsonObject)));
    connect(m_request, SIGNAL(requestError(QNetworkReply::NetworkError, QString)),
            this, SLOT(onError(QNetworkReply::NetworkError, QString)));
}

void GoogleFeedChannelLoader::start()
{
    m_request->send();
}

bool GoogleFeedChannelLoader::loading()
{
    return m_request->isLoading();
}

void GoogleFeedChannelLoader::onResponseReceived(QJsonObject response)
{
    QString parseError;
    GoogleFeedChannel* tempChannel = m_api->parseFeedChannel(response, &parseError);
    if (parseError == "")
    {
        *m_channel = *tempChannel;
    }
    tempChannel->deleteLater();
    m_channel->m_loaded = true;
    m_channel->loadingChanged();
    emit loadingChanged();
    emit finishedLoading();
}

void GoogleFeedChannelLoader::onError(QNetworkReply::NetworkError error, QString description)
{
    Q_UNUSED(error);
    qWarning("GoogleFeedChannelLoader Network Error: %s", qPrintable(description));
}

