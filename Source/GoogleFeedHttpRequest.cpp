#include "GoogleFeedHttpRequest.h"

#include <QJsonParseError>

GoogleFeedHttpRequest::GoogleFeedHttpRequest(QUrl url, QObject* parent)
    : QObject(parent),
      m_url(url),
      m_pendingReply(NULL)
{
    connect(&m_requestManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onRequestFinished(QNetworkReply*)));
}

GoogleFeedHttpRequest::~GoogleFeedHttpRequest()
{
    if (m_pendingReply) m_pendingReply->abort();
}

QUrl GoogleFeedHttpRequest::url()
{
    return m_url;
}

bool GoogleFeedHttpRequest::isLoading()
{
    if (m_pendingReply) return m_pendingReply->isRunning();
    else return false;
}

void GoogleFeedHttpRequest::cancel()
{
    if (!m_pendingReply) return;
    m_pendingReply->abort();
    emit loadingChanged(m_pendingReply->isRunning());
}

void GoogleFeedHttpRequest::send()
{
    get(m_url);
}

void GoogleFeedHttpRequest::get(QUrl url)
{
    setPendingReply(m_requestManager.get(QNetworkRequest(url)));
    connect(m_pendingReply, SIGNAL(metaDataChanged()), this, SLOT(redirectRequest()));
    connect(m_pendingReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onRequestError(QNetworkReply::NetworkError)));
}

void GoogleFeedHttpRequest::setPendingReply(QNetworkReply* reply)
{
    if (m_pendingReply == reply) return;
    if (m_pendingReply)
    {
        m_pendingReply->disconnect(this);
        m_pendingReply->deleteLater();
        m_pendingReply = NULL;
    }
    m_pendingReply = reply;
    emit loadingChanged(m_pendingReply->isRunning());
}

void GoogleFeedHttpRequest::redirectRequest()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply != m_pendingReply) return;

    QUrl redirectionTarget =
            reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if (redirectionTarget.isValid())
    {
        get(redirectionTarget);
    }
}

void GoogleFeedHttpRequest::onRequestError(QNetworkReply::NetworkError error)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply != m_pendingReply) return;

    if (error != QNetworkReply::OperationCanceledError) {
        qWarning("GoogleFeeds Request Error: '%s'\n  Url: '%s'",
                 qPrintable(reply->errorString()),
                 qPrintable(m_pendingReply->url().toString()));

        emit requestError(error, reply->errorString());
    }
    emit loadingChanged(m_pendingReply->isRunning());
}

void GoogleFeedHttpRequest::onRequestFinished(QNetworkReply* reply)
{
    if (reply != m_pendingReply) return;

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode == 0)
    {
        emit loadingChanged(m_pendingReply->isRunning());
        return;
    }
    if (statusCode >= 200 && statusCode < 300)
    {
        QByteArray bytes = reply->readAll();
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
        if (error.error != QJsonParseError::NoError)
        {
            qWarning("GoogleFeeds Request Error: Response could not be converted to JSON Object (%s)",
                     qPrintable(error.errorString()));
            emit requestError(QNetworkReply::ProtocolFailure, "Google Feeds Response Could Not Be Parsed.");
        }
        else
        {
            emit responseReady(doc.object());
        }
    }
    else
    {
        qWarning("GoogleFeeds Request Error: Unexpected Response Code %d returned from URL %s",
                    statusCode, qPrintable(m_url.toString()));
        emit requestError(QNetworkReply::UnknownNetworkError,
                          QString("Google Feeds Request Failed with Error Code %1").arg(statusCode));
    }
    emit loadingChanged(m_pendingReply->isRunning());
}

