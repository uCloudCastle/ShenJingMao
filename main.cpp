#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "catalgorithm.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<CatAlgorithm>("Algorithm", 1, 0, "CatAlgorithm");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    app.exec();
    return 0;
}
