#include "skqtrealtimedatabase.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include "skqtauth.h"
#include "SKQtFirebase_global.h"


SKQtRealTimeDatabase::SKQtRealTimeDatabase(QObject *parent): QObject(parent)
{
  _manager = new QNetworkAccessManager(this);
}

void SKQtRealTimeDatabase::readData(QString url)
{
    _reply = _manager->get(QNetworkRequest(QUrl(_dataUrl +"/" + url+".json?auth="+_idToken)));
    connect(_reply, &QNetworkReply::readyRead, this, &SKQtRealTimeDatabase::getData);
}

void SKQtRealTimeDatabase::sendData(QString url, QVariantMap map)
{
  QJsonDocument myjson = QJsonDocument::fromVariant(map);
  QString temp;
  temp=_dataUrl +"/" + url+".json?auth="+_idToken;
  qDebug()<< "temp : "<<temp;

  QNetworkRequest myrequest((QUrl(temp)));
  myrequest.setHeader(QNetworkRequest::ContentTypeHeader,QString("applicaton/json"));
  _manager->put(myrequest,myjson.toJson());
}

void SKQtRealTimeDatabase::POST(QString url, QVariantMap map)
{
    QJsonDocument myjson = QJsonDocument::fromVariant(map);
    QString temp;
    temp=_dataUrl +"/" + url+".json?auth="+_idToken;
    qDebug()<< "temp : "<<temp;

    QNetworkRequest myrequest((QUrl(temp)));
    myrequest.setHeader(QNetworkRequest::ContentTypeHeader,QString("applicaton/json"));
    _manager->post(myrequest,myjson.toJson());
}

void SKQtRealTimeDatabase::setDataRootUrl(QString url)
{
    _dataUrl = url;
}

void SKQtRealTimeDatabase::deleteData(QString child)
{
     _reply = _manager->deleteResource(QNetworkRequest(QUrl(_dataUrl +"/" + child+".json?auth="+_idToken)));

}

void SKQtRealTimeDatabase::upDateData(QString child,QString key, QString value)
{
    QBuffer *buff = new QBuffer;
    buff->open(QIODevice::ReadWrite);
    buff->write(QString{ "{ \""+key+"\": \""+value+"\" }" }.toUtf8());
    buff->seek(0);

    QString temp;
    temp=_dataUrl +"/" + child+".json?auth="+_idToken;
    QNetworkRequest myrequest((QUrl(temp)));
   myrequest.setHeader(QNetworkRequest::ContentTypeHeader,QString("applicaton/json;charset=UTF-8"));
   _manager->sendCustomRequest(myrequest,"PATCH",buff);
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
     dataAvailable();
    return _arr;
}
