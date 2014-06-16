#ifndef GOOGLEFEEDCHANNELLOADER_H
#define GOOGLEFEEDCHANNELLOADER_H

#include <QObject>
#include <QNetworkReply>
#include <QJsonObject>

class QtGoogleFeedApi;
class GoogleFeedChannel;
class GoogleFeedHttpRequest;
class GoogleFeedChannelLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit GoogleFeedChannelLoader(QtGoogleFeedApi* api, GoogleFeedChannel* channel);

    void start();
    bool loading();

signals:
    void loadingChanged();
    void finishedLoading();

public slots:

protected slots:
    void onResponseReceived(QJsonObject response);
    void onError(QNetworkReply::NetworkError error, QString description);

private:
    QtGoogleFeedApi* m_api;
    GoogleFeedChannel* m_channel;
    GoogleFeedHttpRequest* m_request;
};

#endif // GOOGLEFEEDCHANNELLOADER_H
