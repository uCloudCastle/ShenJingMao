#include <QGuiApplication>
#include <QProcess>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "catalgorithm.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<CatAlgorithm>("Algorithm", 1, 0, "CatAlgorithm");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    int ret = app.exec();
    if (ret == 773) {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    return ret;
}
