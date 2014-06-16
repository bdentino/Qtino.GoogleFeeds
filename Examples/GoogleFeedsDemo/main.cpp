#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include <QtGoogleFeedApi.h>
#include <GoogleFeedQuery.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtGoogleFeedApi gfApi(GoogleApiVersion("1.0"));

    GoogleFeedQuery* query = gfApi.getQueryObject();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    engine.rootObjects().at(0)->setProperty("queryObject", QVariant::fromValue(query));

    return app.exec();
}
