TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../../Source

LIBS += -L../../Source -lQtGoogleFeeds

macx {
    PRE_TARGETDEPS += ../../Source/libQtGoogleFeeds.dylib

    sharedLibs.path = Contents/MacOS
    sharedLibs.files += $$OUT_PWD/../../Source/libQtGoogleFeeds.1.dylib

    for(lib, sharedLibs.files) {
        QMAKE_POST_LINK += "cp $${lib} $${TARGET}.app/$${sharedLibs.path}/;"
    }
}
