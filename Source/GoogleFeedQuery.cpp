#include "GoogleFeedQuery.h"
#include "QtGoogleFeedApi.h"
#include "GoogleFeedHttpRequest.h"
#include "GoogleFeedQueryResult.h"

GoogleFeedQuery::GoogleFeedQuery(QtGoogleFeedApi* api) :
    QObject(api),
    m_api(api),
    m_request(NULL),
    m_query("")
{
    connect(this, SIGNAL(queryChanged()), this, SLOT(refresh()));
}

GoogleFeedQuery::GoogleFeedQuery(GoogleApiVersion version, QObject* parent)
    : QObject(parent),
      m_api(new QtGoogleFeedApi(version)),
      m_request(NULL),
      m_query("")
{
    connect(this, SIGNAL(queryChanged()), this, SLOT(refresh()));
}

QString GoogleFeedQuery::query()
{
    return m_query;
}

void GoogleFeedQuery::setQuery(QString query)
{
    if (m_query == query) return;
    m_query = query;
    emit queryChanged();
}

QQmlListProperty<GoogleFeedQueryResult> GoogleFeedQuery::qmlResults()
{
    return QQmlListProperty<GoogleFeedQueryResult>(this, m_results);
}

const QList<GoogleFeedQueryResult*> GoogleFeedQuery::results()
{
    return m_results;
}

void GoogleFeedQuery::refresh()
{
    if (m_request) {
        disconnect(m_request);
        m_request->disconnect(this);
        m_request->cancel();
        m_request->deleteLater();
    }
    m_request = m_api->getFindRequest(m_query);
    connect(m_request, SIGNAL(responseReady(QJsonObject)),
            this, SLOT(onResponseReceived(QJsonObject)));
    m_request->send();
}

void GoogleFeedQuery::onResponseReceived(QJsonObject response)
{
    m_results = m_api->parseFindResponse(response);
    qDebug() << m_results.count();
    emit resultsChanged();
}
