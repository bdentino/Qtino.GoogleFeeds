#-------------------------------------------------
#
# Project created by QtCreator 2014-06-14T10:28:55
#
#-------------------------------------------------

QT       += network qml quick
QT       -= gui

CONFIG += qt plugin
TARGET = $$qtLibraryTarget(QtGoogleFeeds)
TEMPLATE = lib

uri = Qtino.GoogleFeeds

DEFINES += QTGOOGLEFEEDS_LIBRARY

SOURCES += \
    GoogleFeedQuery.cpp \
    QtGoogleFeedApi.cpp \
    GoogleFeedHttpRequest.cpp \
    GoogleFeedQueryResult.cpp \
    GoogleFeedChannel.cpp \
    GoogleFeedItem.cpp \
    GoogleFeedChannelLoader.cpp \
    QtGoogleFeedsPlugin.cpp

PUBLIC_HEADERS += QtGoogleFeeds_Global.h \
                  QtGoogleFeedApi.h \
                  GoogleFeedQuery.h \
                  GoogleFeedChannel.h \
                  GoogleFeedItem.h \
                  GoogleFeedQueryResult.h

HEADERS += \
    $$PUBLIC_HEADERS \
    GoogleFeedHttpRequest.h \
    GoogleFeedChannelLoader.h \
    QtGoogleFeedsPlugin.h

OTHER_FILES += \
               qmldir

QMAKE_MOC_OPTIONS += -Muri=$$uri

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
includePath = $$[QT_INSTALL_HEADERS]/$$replace(uri, \\., /)

unix: { libprefix = lib }
win32: { libprefix = }

CONFIG(static, static|shared) {
    macx|ios|unix: { libsuffix = a }
    win32: { libsuffix = lib }
}
else {
    macx: { libsuffix = dylib }
    unix:!macx: { libsuffix = so }
    win32: { libsuffix = lib }
}

cleanTarget.files +=
cleanTarget.path += $$installPath
macx|ios|unix: cleanTarget.extra = rm -rf $$installPath/*.qml $$installPath/qmldir $$installPath/plugins.qmltypes $$installPath/$$libprefix$$TARGET$${qtPlatformTargetSuffix}.$$libsuffix

headers.files = $$PUBLIC_HEADERS
headers.path = $$includePath

universalInclude.files = QtGoogleFeeds
universalInclude.path = $$includePath/..

qmldir.files = qmldir
qmldir.path = $$installPath
target.path = $$installPath

INSTALLS += cleanTarget target qmldir headers universalInclude

QMAKE_POST_LINK += make install
