TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

ios {
    LIBS += -L/Users/bdentino/Documents/GitHub/OpenSSL-for-iPhone/lib -lssl -lcrypto
}
