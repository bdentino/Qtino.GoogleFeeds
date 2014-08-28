#include "GoogleFeedQueryResult.h"
#include "QtGoogleFeedApi.h"
#include "GoogleFeedChannel.h"

GoogleFeedQueryResult::GoogleFeedQueryResult(QtGoogleFeedApi* api)
    : QObject(api),
      m_api(api)
{
}

QUrl GoogleFeedQueryResult::url() const
{
    return m_url;
}

QString GoogleFeedQueryResult::title() const
{
    return m_title;
}

QString GoogleFeedQueryResult::contentSnippet() const
{
    return m_contentSnippet;
}

QUrl GoogleFeedQueryResult::link() const
{
    return m_link;
}

QtGoogleFeedApi* GoogleFeedQueryResult::api()
{
    return m_api;
}

GoogleFeedChannel* GoogleFeedQueryResult::feed()
{
    return m_api->loadFeed(m_url);
}
