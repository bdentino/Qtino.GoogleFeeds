#ifndef GOOGLEFEEDQUERY_H
#define GOOGLEFEEDQUERY_H

#include "QtGoogleFeeds_Global.h"

#include <QObject>
#include <QJsonObject>
#include <QtQml>

#include "GoogleFeedQueryResult.h"
struct GoogleApiVersion;
class QtGoogleFeedApi;
class GoogleFeedHttpRequest;
class QTGOOGLEFEEDSSHARED_EXPORT GoogleFeedQuery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QtGoogleFeedApi* api READ api WRITE setApi)
    Q_PROPERTY(QString query READ query WRITE setQuery NOTIFY queryChanged)
    Q_PROPERTY(QQmlListProperty<GoogleFeedQueryResult> results READ qmlResults NOTIFY resultsChanged)

public:
    GoogleFeedQuery(QtGoogleFeedApi* api = 0);
    GoogleFeedQuery(GoogleApiVersion version, QObject* parent = 0);

    QtGoogleFeedApi* api();
    void setApi(QtGoogleFeedApi* api);

    QString query();
    void setQuery(QString query);

    QQmlListProperty<GoogleFeedQueryResult> qmlResults();
    const QList<GoogleFeedQueryResult*> results();

signals:
    void queryChanged();
    void resultsChanged();

public slots:
    void refresh();

private slots:
    void onResponseReceived(QJsonObject response);

private:
    QtGoogleFeedApi* m_api;
    GoogleFeedHttpRequest* m_request;

    QString m_query;
    QList<GoogleFeedQueryResult*> m_results;
};

#endif // GOOGLEFEEDQUERY_H
