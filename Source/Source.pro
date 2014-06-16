#-------------------------------------------------
#
# Project created by QtCreator 2014-06-14T10:28:55
#
#-------------------------------------------------

QT       += network qml

QT       -= gui

TARGET = QtGoogleFeeds
TEMPLATE = lib

DEFINES += QTGOOGLEFEEDS_LIBRARY

SOURCES += \
    GoogleFeedQuery.cpp \
    QtGoogleFeedApi.cpp \
    GoogleFeedHttpRequest.cpp \
    GoogleFeedQueryResult.cpp \
    GoogleFeedChannel.cpp \
    GoogleFeedItem.cpp \
    GoogleFeedChannelLoader.cpp

HEADERS += \
    GoogleFeedQuery.h \
    QtGoogleFeedApi.h \
    GoogleFeedHttpRequest.h \
    GoogleFeedQueryResult.h \
    QtGoogleFeeds_Global.h \
    GoogleFeedChannel.h \
    GoogleFeedItem.h \
    GoogleFeedChannelLoader.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
