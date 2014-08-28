#ifndef GOOGLEFEEDITEM_H
#define GOOGLEFEEDITEM_H

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include <QStringList>
#include <QJsonObject>
#include <QVariantList>

#include "QtGoogleFeeds_Global.h"

class GoogleFeedChannel;
class QTGOOGLEFEEDSSHARED_EXPORT GoogleFeedItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QUrl link READ link CONSTANT)
    Q_PROPERTY(QString htmlContent READ htmlContent CONSTANT)
    Q_PROPERTY(QString contentSnippet READ contentSnippet CONSTANT)
    Q_PROPERTY(QDateTime publishedDate READ publishedDate CONSTANT)
    Q_PROPERTY(QStringList categories READ categories CONSTANT)

    // Getting lazy here, but we're not currently using any of the 'mediaGroup'
    // content. Plus the spec is pretty damn extensive and so loose around media/images that
    // I don't think it will be very useful unless we implement some sort of configurable
    // parser for feed-specific extraction of the elements that we want. (This
    // may very well be a feature worth implementing in the future, though)
    //
    // Anyways, if we actually end up wanting to use this tag, we should probably build
    // out some QObject* classes to structure the data
    Q_PROPERTY(QVariantList mediaGroups READ mediaGroups CONSTANT)

public:

    QString title() const { return m_title; }
    QUrl link() const { return m_link; }
    QString author() const { return m_author; }
    QString htmlContent() const { return m_htmlContent; }
    QString contentSnippet() const { return m_contentSnippet; }
    QDateTime publishedDate() const { return m_publishedDate; }
    QStringList categories() const { return m_categories; }

    QVariantList mediaGroups() const { return m_mediaGroups; }

protected:
    friend class QtGoogleFeedApi;
    friend class GoogleFeedChannel;
    GoogleFeedItem(GoogleFeedChannel* channel, QJsonObject item);
    void setChannel(GoogleFeedChannel* channel);

private:
    QString retrieveTitle(QJsonObject& item);
    QUrl retrieveLink(QJsonObject& item);
    QString retrieveAuthor(QJsonObject& item);
    QString retrieveContent(QJsonObject& item);
    QString retrieveSnippet(QJsonObject& item);
    QDateTime retrievePublishedDate(QJsonObject& item);
    QStringList retrieveCategories(QJsonObject& item);

    QVariantList retrieveMediaGroups(QJsonObject& item);

private:
    GoogleFeedChannel* m_channel;
    QString m_title;
    QUrl m_link;
    QString m_author;
    QString m_htmlContent;
    QString m_contentSnippet;
    QDateTime m_publishedDate;
    QStringList m_categories;

    QVariantList m_mediaGroups;
};

#endif // GOOGLEFEEDITEM_H
