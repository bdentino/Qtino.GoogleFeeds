#ifndef GOOGLEFEEDCHANNEL_H
#define GOOGLEFEEDCHANNEL_H

#include <QObject>
#include <QQmlListProperty>
#include <QJsonObject>
#include <QUrl>
#include <QNetworkReply>

#include "QtGoogleFeeds_Global.h"
#include "GoogleFeedItem.h"

struct GoogleApiVersion;
class GoogleFeedHttpRequest;
class QtGoogleFeedApi;
class QTGOOGLEFEEDSSHARED_EXPORT GoogleFeedChannel : public QObject
{
    friend class QtGoogleFeedApi;
    friend class GoogleFeedChannelLoader;

    Q_OBJECT
    Q_PROPERTY(QtGoogleFeedApi* api READ api WRITE setApi)

    Q_PROPERTY(QUrl feedUrl READ feedUrl CONSTANT)
    Q_PROPERTY(QString title READ title NOTIFY loadingChanged)
    Q_PROPERTY(QUrl siteUrl READ siteUrl NOTIFY loadingChanged)
    Q_PROPERTY(QString author READ author NOTIFY loadingChanged)
    Q_PROPERTY(QString description READ description NOTIFY loadingChanged)
    Q_PROPERTY(QString type READ type NOTIFY loadingChanged)

    Q_PROPERTY(int maxItems READ maxItems WRITE setMaxItems NOTIFY maxItemsChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY loadingChanged)

    Q_PROPERTY(QQmlListProperty<GoogleFeedItem> items READ itemsProperty NOTIFY itemsChanged)

protected:
    explicit GoogleFeedChannel(QtGoogleFeedApi* api = 0);
    void copyAndDelete(GoogleFeedChannel* copy);

public:
    QtGoogleFeedApi* api();
    void setApi(QtGoogleFeedApi* api);

    QUrl feedUrl();
    QString title();
    QUrl siteUrl();
    QString author();
    QString description();
    QString type();

    bool isLoading();

    QQmlListProperty<GoogleFeedItem> itemsProperty();

    QList<GoogleFeedItem*> items();

    int maxItems();
    void setMaxItems(int maxItems);

signals:
    void loadingChanged();
    void itemsChanged();
    void maxItemsChanged();

public slots:
    void refresh();

private slots:
    void onResponseReceived(QJsonObject response);
    void onError(QNetworkReply::NetworkError error, QString description);

private:
    QUrl m_feedUrl;
    QString m_title;
    QUrl m_siteUrl;
    QString m_author;
    QString m_description;
    QString m_type;
    bool m_loaded;

    int m_maxItems;

    QList<GoogleFeedItem*> m_items;

    QtGoogleFeedApi* m_api;
    GoogleFeedHttpRequest* m_request;
};

#endif // GOOGLEFEEDCHANNEL_H
