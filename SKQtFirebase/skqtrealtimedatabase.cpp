#include "skqtrealtimedatabase.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QImageWriter>
#include "SKQtFirebase_global.h"
#include <QImageReader>


SKQtRealTimeDatabase::SKQtRealTimeDatabase(QObject *parent): QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
}

QByteArray SKQtRealTimeDatabase::readData(QString url)
{
    _reply = _manager->get(QNetworkRequest(QUrl(_dataUrl +"/" + url+".json?auth="+_idToken)));
    connect(_reply, &QNetworkReply::readyRead, this, &SKQtRealTimeDatabase::getData);
    connect(_reply, &QNetworkReply::errorOccurred, this, &SKQtRealTimeDatabase::handleError);
    _eventLoop.exec();
    return _arr;
}

void SKQtRealTimeDatabase::put(QString url, QVariantMap map)
{
    QJsonDocument myjson = QJsonDocument::fromVariant(map);
    QString temp;
    temp = _dataUrl + "/" + url + ".json?auth=" + _idToken;
    qDebug() << "temp : " << temp;

    QNetworkRequest myrequest((QUrl(temp)));
    myrequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    QNetworkReply* reply = _manager->put(myrequest, myjson.toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // the request succeeded
            qDebug() << "PUT request succeeded!";
        } else {
            // the request failed
            qDebug() << "PUT request failed: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void SKQtRealTimeDatabase::post(QString url, QVariantMap map)
{
    QJsonDocument myjson = QJsonDocument::fromVariant(map);
    QString temp;
    temp = _dataUrl + "/" + url + ".json?auth=" + _idToken;
    qDebug() << "temp : " << temp;

    QNetworkRequest myrequest((QUrl(temp)));
    myrequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    QNetworkReply* reply = _manager->post(myrequest, myjson.toJson());
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // the request succeeded
            qDebug() << "POST request succeeded!";
        } else {
            // the request failed
            qDebug() << "POST request failed: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

void SKQtRealTimeDatabase::setDataRootUrl(QString url)
{
    _dataUrl = url;
}

void SKQtRealTimeDatabase::deleteData(QString child)
{
    _reply = _manager->deleteResource(QNetworkRequest(QUrl(_dataUrl +"/" + child+".json?auth="+_idToken)));

}

void SKQtRealTimeDatabase::patch(QString child,QString key, QString value)
{
    QBuffer *buff = new QBuffer;
    buff->open(QIODevice::ReadWrite);
    buff->write(QString{ "{ \""+key+"\": \""+value+"\" }" }.toUtf8());
    buff->seek(0);

    QString temp;
    temp = _dataUrl + "/" + child + ".json?auth=" + _idToken;
    QNetworkRequest myrequest((QUrl(temp)));
    myrequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    QNetworkReply* reply = _manager->sendCustomRequest(myrequest, "PATCH", buff);
    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            // la requête a réussi
            qDebug() << "updated successful !";
        } else {
            // la requête a échoué
            qDebug() << "error when trying to update : " << reply->errorString();
        }
        reply->deleteLater();
    });
}

QByteArray SKQtRealTimeDatabase::data()
{

    return _arr;
}

SKQtRealTimeDatabase::~SKQtRealTimeDatabase()
{
    _manager->deleteLater();
    _reply->deleteLater();

}

QByteArray SKQtRealTimeDatabase::getData()
{
    QString json = QString::fromUtf8(_reply->readAll());
    QJsonDocument temp = QJsonDocument::fromJson(json.toUtf8());
    QByteArray arr = temp.toJson(QJsonDocument::Compact);
    _arr = arr;
    _eventLoop.quit();
    dataAvailable();
    return _arr;
}

void SKQtRealTimeDatabase::handleError(QNetworkReply::NetworkError error)
{
    qDebug() << "connexion error :" << error << "-" << _reply->errorString();
    _eventLoop.quit();
}






