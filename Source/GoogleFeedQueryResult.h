#ifndef GOOGLEFEEDQUERYRESULT_H
#define GOOGLEFEEDQUERYRESULT_H

#include "QtGoogleFeeds_Global.h"
#include <QObject>
#include <QUrl>

class GoogleFeedChannel;
class QtGoogleFeedApi;
class QTGOOGLEFEEDSSHARED_EXPORT GoogleFeedQueryResult : public QObject
{
    friend class GoogleFeedQueryResultBuilder;

    Q_OBJECT
    Q_PROPERTY(QUrl feedUrl READ url CONSTANT)
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString contentSnippet READ contentSnippet CONSTANT)
    Q_PROPERTY(QUrl link READ link CONSTANT)

public:
    QUrl url() const;
    QString title() const;
    QString contentSnippet() const;
    QUrl link() const;

    QtGoogleFeedApi* api();

protected:
    GoogleFeedQueryResult(QtGoogleFeedApi* api);

public slots:
    GoogleFeedChannel* feed();

private:
    QUrl m_url;
    QString m_title;
    QString m_contentSnippet;
    QUrl m_link;

    QtGoogleFeedApi* m_api;
};

class GoogleFeedQueryResultBuilder
{
public:

    GoogleFeedQueryResultBuilder(QtGoogleFeedApi* api)
    {
        m_api = api;
        m_result = new GoogleFeedQueryResult(api);
    }

    ~GoogleFeedQueryResultBuilder()
    {
        delete m_result;
    }

    void setUrl(QUrl url) const
    {
        m_result->m_url = url;
    }

    void setTitle(const QString title)
    {
        m_result->m_title = title;
    }

    void setContentSnippet(const QString contentSnippet)
    {
        m_result->m_contentSnippet = contentSnippet;
    }

    void setLink(const QUrl link)
    {
        m_result->m_link = link;
    }

    GoogleFeedQueryResult* createResultObject(QObject* parent = 0)
    {
        GoogleFeedQueryResult* returnObj = m_result;
        returnObj->setParent(parent);
        m_result = new GoogleFeedQueryResult(m_api);
        return returnObj;
    }

private:
    GoogleFeedQueryResult* m_result;
    QtGoogleFeedApi* m_api;
};

#endif // GOOGLEFEEDQUERYRESULT_H
