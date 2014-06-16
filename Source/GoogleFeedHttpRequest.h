#ifndef GOOGLEFEEDHTTPREQUEST_H
#define GOOGLEFEEDHTTPREQUEST_H

#include <QObject>
#include <QtNetwork>

struct GoogleApiVersion;
class GoogleFeedHttpRequest : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
    Q_PROPERTY(QUrl url READ url NOTIFY urlChanged)

public:
    explicit GoogleFeedHttpRequest(QUrl url, QObject* parent = 0);
    ~GoogleFeedHttpRequest();

    QUrl url();

    bool isLoading();
    void cancel();

signals:
    void loadingChanged(bool loading);
    void urlChanged();

    void responseReady(const QJsonObject& response);
    void requestError(QNetworkReply::NetworkError error, QString description);

public slots:
    void send();

private slots:
    void get(QUrl url);
    void setPendingReply(QNetworkReply* reply);
    void redirectRequest();
    void onRequestError(QNetworkReply::NetworkError error);
    void onRequestFinished(QNetworkReply* reply);

private:
    QUrl m_url;
    QNetworkAccessManager m_requestManager;
    QNetworkReply* m_pendingReply;
};

#endif // GOOGLEFEEDHTTPREQUEST_H
