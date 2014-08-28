#include "GoogleFeedItem.h"
#include "GoogleFeedChannel.h"

#include <QJsonArray>
#include <QDebug>

GoogleFeedItem::GoogleFeedItem(GoogleFeedChannel* channel, QJsonObject item)
    : QObject(channel),
      m_channel(channel)
{
    m_link = retrieveLink(item);
    m_title = retrieveTitle(item);
    m_author = retrieveAuthor(item);
    m_htmlContent = retrieveContent(item);
    m_contentSnippet = retrieveSnippet(item);
    m_publishedDate = retrievePublishedDate(item);
    m_mediaGroups = retrieveMediaGroups(item);
}

void GoogleFeedItem::setChannel(GoogleFeedChannel* channel)
{
    m_channel = channel;
    setParent(channel);
}

QUrl GoogleFeedItem::retrieveLink(QJsonObject& item)
{
    if (item["link"].isString())
    {
        return QUrl(item["link"].toString());
    }
    else
    {
        qWarning() << "GoogleFeedItem Warning: Item from feed '"
                   << m_channel->feedUrl() << "' has no link";
        return QUrl();
    }
}

QString GoogleFeedItem::retrieveTitle(QJsonObject& item)
{
    if (item["title"].isString())
        return item["title"].toString();
    else
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse title '"
                   << item["title"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
        return QString();
    }
}

QString GoogleFeedItem::retrieveAuthor(QJsonObject& item)
{
    if (item["author"].isString())
        return item["author"].toString();
    else
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse author '"
                   << item["author"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
        return QString();
    }
}

QString GoogleFeedItem::retrieveContent(QJsonObject& item)
{
    if (item["content"].isString())
        return item["content"].toString();
    else
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse content '"
                   << item["content"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
        return QString();
    }
}

QString GoogleFeedItem::retrieveSnippet(QJsonObject& item)
{
    if (item["contentSnippet"].isString())
        return item["contentSnippet"].toString();
    else
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse contentSnippet '"
                   << item["contentSnippet"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
        return QString();
    }
}

QDateTime GoogleFeedItem::retrievePublishedDate(QJsonObject& item)
{
    if (item["publishedDate"].isString())
    {
        //Format: 13 Apr 2007 12:40:07 -0700
        QString dateString = item["publishedDate"].toString();
        return QDateTime::fromString(dateString, Qt::RFC2822Date);
    }
    else
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse publishedDate '"
                   << item["publishedDate"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
        return QDateTime();
    }
}

QStringList GoogleFeedItem::retrieveCategories(QJsonObject& item)
{
    if (item["categories"].isArray())
    {
        QStringList categories;
        QJsonArray array = item["categories"].toArray();
        foreach (QJsonValue value, array)
        {
            if (value.isString()) categories << value.toString();
            else break;
        }
        return categories;
    }
    else if (item["categories"].isString())
    {
        return QStringList(item["categories"].toString());
    }

    qWarning() << "GoogleFeedItem Warning: Could not parse categories '"
               << item["categories"] << "' from link '" << item["link"] << "'"
               << "(feed: " << m_channel->feedUrl() << ")";
    return QStringList();
}

QVariantList GoogleFeedItem::retrieveMediaGroups(QJsonObject& item)
{
    if (item["mediaGroups"].isArray())
    {
        return item["mediaGroups"].toArray().toVariantList();
    }
    else if (item["mediaGroups"].isObject())
    {
        QVariantList list;
        list << item["mediaGroups"].toObject().toVariantMap();
        return list;
    }
    else if (!item["mediaGroups"].isNull())
    {
        qWarning() << "GoogleFeedItem Warning: Could not parse mediaGroups '"
                   << item["mediaGroups"] << "' from link '" << item["link"] << "'"
                   << "(feed: " << m_channel->feedUrl() << ")";
    }
    return QVariantList();
}
