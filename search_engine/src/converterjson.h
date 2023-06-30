#ifndef CONVERTERJSON_H
#define CONVERTERJSON_H

#include <QVector>
#include <QString>
#include <QPair>
#include <QTextStream>
#include <QFile>
#include <iostream>
#include <fstream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFileInfo>
#include <QDir>

#include "exceptions.h"

using namespace std;

struct ConfigInfo
{
    QString name;
    QString version;
    int max_responses;
};

struct ConfigFileStruct
{
    ConfigInfo conf;
    QVector<QString> files;
};

class ConverterJSON: public QObject
{
    Q_OBJECT
public:
    ConverterJSON() = default;
    /**
    * File content retrieval method
    * @return Returns a list with the contents of the files listed in the config.json
    */
    QVector<QString> GetTextDocuments();
    /**
    * The method reads the max_responses field to determine the limit
    * number of responses per request
    * @return
    */
    int GetResponsesLimit();
    /**
    * Method for receiving requests from the requests.json file
    * @return returns a list of requests from the requests.json file
    */
    QVector<QString> GetRequests();
    /**
    * Put search results in answers.json file
    */
    void putAnswers(QVector<QVector<QPair<int, float>>>
    answers);

    /**
    * Remember the address of config files
    */
    void setConfigPath(QString);
    /**
    * Get address of config files
    */
    QString getConfigPath();

    /**
    * Function for testing, creates a config file
    */
    void makeFirstConfigFile();
signals:
    /**
    * Sending an error to an application form
    */
    void showError(QString error);
private:
    /**
    * config file structures
    */
    ConfigInfo configInfo;
    ConfigFileStruct config;
    /**
    * Checking if a file exists at an incoming address
    */
    bool fileExists(QString path);
    /**
    * Address of config files
    */
    QString confPath;

    /**
     * @brief checkVersion
     */
    bool checkVersion(QString);

};

#endif // CONVERTERJSON_H
