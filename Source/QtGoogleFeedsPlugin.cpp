#include "QtGoogleFeedsPlugin.h"

#include "GoogleFeedChannel.h"
#include "GoogleFeedItem.h"
#include "GoogleFeedQuery.h"
#include "GoogleFeedQueryResult.h"
#include "QtGoogleFeedApi.h"

#include <QtQml>

void QtGoogleFeedsPlugin::registerTypes(const char *uri)
{
    // @uri Qtino.GoogleFeeds
    qmlRegisterType<QtGoogleFeedApi>(uri, 1, 0, "GoogleFeedApi");
    qmlRegisterType<GoogleFeedQuery>(uri, 1, 0, "GoogleFeedQuery");
    qmlRegisterType<GoogleFeedChannel>(uri, 1, 0, "GoogleFeedChannel");
    qmlRegisterUncreatableType<GoogleFeedItem>(uri, 1, 0, "GoogleFeedItem", "GoogleFeedItems can only be created by the GoogleFeedChannel that owns them");
    qmlRegisterUncreatableType<GoogleFeedQueryResult>(uri, 1, 0, "GoogleFeedQueryResult", "GoogleFeedQueryResults are created through a GoogleFeedQuery object");
}
