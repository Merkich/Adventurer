#include <QCoreApplication>
#include <QApplication>
#include <QObject>
#include <QString>


#include "mainwindow.h"


int main(int argc, char *argv[])
{
   // QCoreApplication a(argc, argv);
    QApplication a(argc, argv);

    QString stringJson;
    QFile file;
    QDir dir;
    file.setFileName(dir.currentPath() + "/config/" + "config.json");
    QString name;
    if(file.exists())
    {
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            stringJson = file.readAll();
            file.close();

            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(stringJson.toUtf8(), &error);
            if(error.error == QJsonParseError::NoError)
            {
                if(doc.isObject())
                {
                    QJsonObject json = doc.object();

                    QJsonValue configValue = json.value("config");
                    if(!configValue.isUndefined())
                    {
                        name = configValue["name"].toString();
                    }
                }
            }
        }
    }
    if(name.isEmpty())
        name = "SearchEngineWindow";

    MainWindow w;
    w.setWindowTitle(name);
    w.show();

    return a.exec();
}
