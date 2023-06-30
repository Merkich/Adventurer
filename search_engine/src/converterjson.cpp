#include "converterjson.h"

QVector<QString> ConverterJSON::GetTextDocuments()
{
    QString stringJson;
    QFile file;
    QDir dir(".");
    file.setFileName(confPath + "/config.json");

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
                        configInfo.name = configValue["name"].toString();
                        configInfo.version = configValue["version"].toString();

                        if(!checkVersion(configInfo.version))
                            emit showError("В файле config.json версия приложения не совпадает с текущей версией проект");

                        configInfo.max_responses = configValue["max_responses"].toInt();
                        if(configInfo.max_responses <= 0)
                            configInfo.max_responses = 5;
                        config.conf = configInfo;
                        QJsonArray valPath = json["files"].toArray();
                        QVector<QString> pathStrings;
                        foreach(const QJsonValue & value, valPath)
                        {
                            QFile file(confPath.left(confPath.lastIndexOf('/')) + "/" + value.toString());
                            if(!file.exists())
                            {
                                qDebug() << "File" << value.toString() << "does not exist";
                                pathStrings.push_back("");
                                emit showError("Файл " + value.toString() + " не найден");
                            }
                            else
                            {
                                if(file.open(QIODevice::ReadOnly | QIODevice::Text))
                                {
                                    pathStrings.push_back(QString(file.readAll()));
                                    file.close();
                                }

                            }
                        }
                        config.files = pathStrings;
                    }
                    else
                    {
                        qDebug() << "There is no field \"config\"  in the file config.json";
                        throw NoConfigField();
                    }
                }
            }
            else
            {
                qDebug() << "Wrong struct in file - config.json";
                throw WrongStructConfig();
            }
        }
        else
        {
            qDebug() << "File config.json isn`t open!";
            throw ConfigFileNotOpen();
        }
    }
    else
    {
        qDebug() << "config.json file not found!";
        throw NoConfigFile();
    }

    return config.files;
}

bool ConverterJSON::fileExists(QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}

bool ConverterJSON::checkVersion(QString versionConf)
{
    QString stringJson;
    QFile file;
    QVector<QString> requestsVec;
    QDir dir;
    file.setFileName(confPath + "/version.json");

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
                QString versionProj = json["version"].toString();

                if(versionProj == versionConf)
                    return true;
                else
                    return false;
            }
        }
        }
    }
}

int ConverterJSON::GetResponsesLimit()
{
    return config.conf.max_responses;
}

QVector<QString> ConverterJSON::GetRequests()
{
    QString stringJson;
    QFile file;
    QVector<QString> requestsVec;
    QDir dir;
    file.setFileName(confPath + "/requests.json");

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
                QJsonArray valPath = json["requests"].toArray();

                if(!valPath.empty())
                {
                    foreach(const QJsonValue & value, valPath)
                    {
                        requestsVec.push_back(value.toString());
                    }
                }
                else
                {
                    qDebug() << "There is no field \"request\"  in the file requests.json";
                    throw NoRequestField();
                }
            }
        }
        else
        {
            qDebug() << "Wrong struct in file - requests.json";
            throw WrongStructRequest();
        }
        }
        else
        {
            qDebug() << "File requests.json isn`t open!";
            throw RequestsFileNotOpen();
        }
    }
    else
    {
        qDebug() << "requests.json file not found!";
        throw NoRequestsFile();
    }

    return requestsVec;
}

void ConverterJSON::putAnswers(QVector<QVector<QPair<int, float> > > answers)
{
    int i = 0;

    QJsonArray arrayAnswer;
    QJsonObject textAnswer;

    for(auto &vecPair : answers)
    {
        QJsonObject textResult;
        QJsonObject textDocResult;
        QJsonArray arrayDocResult;

        QJsonObject textRequest;
        QJsonObject textRelevance;
        QJsonArray arrayRequest;

        if(!vecPair.isEmpty())
        {
            textResult.insert("result", true);
            arrayRequest.push_back(textResult);
            if(vecPair.size() == 1)
            {
                textDocResult.insert("docid", vecPair.at(0).first);
                textDocResult.insert("rank", vecPair.at(0).second);
                arrayRequest.push_back(textDocResult);
            }
            else
            {
                for(auto &p : vecPair)
                {
                    QJsonObject textDoc;
                    textDoc.insert("docid", p.first);
                    textDoc.insert("rank", p.second);
                    arrayDocResult.push_back(textDoc);
                }
                textRelevance.insert("relevance", arrayDocResult);
                arrayRequest.push_back(textRelevance);
            }
        }
        else
        {
            textResult.insert("result", false);
            arrayRequest.push_back(textResult);
        }
        textRequest.insert(("request" + QString::number(i)), arrayRequest);
        arrayAnswer.push_back(textRequest);
        i++;
    }
    textAnswer.insert("answers", arrayAnswer);

    QJsonDocument doc(textAnswer);
    QString jsonString = doc.toJson(QJsonDocument::Indented);
    QFile file;
    QDir dir;
    file.setFileName(confPath + "/answers.json");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << jsonString;
    file.close();
}

void ConverterJSON::setConfigPath(QString path)
{
    confPath = path.left(path.lastIndexOf('/'));
}

QString ConverterJSON::getConfigPath()
{
    return confPath;
}

void ConverterJSON::makeFirstConfigFile()
{
//    json firstConf;
    configInfo.name = "SkillBoxSearchEngine";
    configInfo.version = "1.0";
    configInfo.max_responses = 5;

    config.conf = configInfo;
    QVector<QString> paths;
    paths.push_back("../resources/file001.txt");
    paths.push_back("../resources/file002.txt");
    paths.push_back("../resources/file003.txt");
    config.files = paths;


    // Create a text object
       QJsonObject textConfObject;
       textConfObject.insert("name", QJsonValue::fromVariant(configInfo.name));
       textConfObject.insert("version", QJsonValue::fromVariant(configInfo.version));
       textConfObject.insert("max_responses", QJsonValue::fromVariant(configInfo.max_responses));

        QJsonArray arrayPath;
        for(auto& i : paths)
        {
            arrayPath.push_back(i);
        }

        QJsonObject resultFile;
        resultFile.insert("config", textConfObject);
        resultFile.insert("files", arrayPath);

        QJsonDocument doc(resultFile);
        QString jsonString = doc.toJson(QJsonDocument::Indented);
        QFile file;
        file.setFileName("config.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&file);
        stream << jsonString;
        file.close();
}







