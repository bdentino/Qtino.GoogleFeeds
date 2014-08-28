TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../../Source

installPath = $$[QT_INSTALL_QML]/Qtino/GoogleFeeds

iphonesimulator {
    LIBS += -L$$installPath -lQtGoogleFeeds_iphonesimulator
} else {
    LIBS += -L$$installPath -lQtGoogleFeeds
}

macx {
    PRE_TARGETDEPS += $$installPath/libQtGoogleFeeds.dylib

    sharedLibs.path = Contents/MacOS
    sharedLibs.files += $$installPath/libQtGoogleFeeds.dylib

    for(lib, sharedLibs.files) {
        QMAKE_POST_LINK += "cp $${lib} $${TARGET}.app/$${sharedLibs.path}/;"
    }
}
