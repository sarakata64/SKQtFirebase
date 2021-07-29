#include "skqtauth.h"
#include "SKQtFirebase_global.h"
#include <QJsonDocument>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>

QString _idToken;

SKQtAuth::SKQtAuth(QObject *parent) : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
}

void SKQtAuth::setApiKey( const QString &apikey)
{
    _apikey = apikey;
}

void SKQtAuth::signUpUser(const QString &email, const QString &password)
{
    QString signEndpoint ="https://identitytoolkit.googleapis.com/v1/accounts:signUp?key="+ _apikey;
    QVariantMap payload;
    payload["email"]= email;
    payload["password"]= password;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPostLogin(signEndpoint,doc);

}

void SKQtAuth::signInUser(const QString &email, const QString &password)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + _apikey;
    QVariantMap payload;
    payload["email"]= email;
    payload["password"]= password;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPostLogin(signEndpoint,doc);
}

/*void SKQtAuth::readData()
{
    QByteArray resp = _reply->readAll();
    qDebug()<<resp;
    QJsonDocument mydoc = QJsonDocument::fromJson(resp);
    qDebug()<<mydoc.object().value("food").toInt();  // this is an example
}*/

QString SKQtAuth::readResponse()
{
    QByteArray response=  _reply->readAll();
     _reply->deleteLater();
     _ResponseValue = response;
     // get idToken
     getResponseToken(response);
     qDebug()<<response;

     return response;

}

QString SKQtAuth::readResponseLogin()
{
    QByteArray response=  _reply->readAll();
     _reply->deleteLater();
     _ResponseValue = response;
     // get idToken
     getResponseToken(response);
     qDebug()<<response;
     QJsonDocument mydoc = QJsonDocument::fromJson(response);
     if(mydoc.object().contains("error"))
     {
         errorAuth();


     }else if(mydoc.object().contains("kind"))
     {
         successAuth();

     }

     return response;
}

void SKQtAuth::performPost(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest networkRequest((QUrl(url)));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    _reply = _manager->post(networkRequest, payload.toJson());
    connect(_reply,&QNetworkReply::readyRead, this,&SKQtAuth::readResponse);
}

void SKQtAuth::performPostLogin(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest networkRequest((QUrl(url)));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    _reply = _manager->post(networkRequest, payload.toJson());
    connect(_reply,&QNetworkReply::readyRead, this,&SKQtAuth::readResponseLogin);
}

void SKQtAuth::resetPassword(const QString &email)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + _apikey;
    QVariantMap payload;
    payload["email"]= email;
    payload["requestType"]="PASSWORD_RESET" ;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::changeEmail(const QString &new_email)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + _apikey;
    QVariantMap payload;
    payload["email"]= new_email;
    payload["idToken"]=_idToken ;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::changePassword(const QString &new_password)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + _apikey;
    QVariantMap payload;
    payload["email"]= new_password;
    payload["idToken"]=_idToken ;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::updateProfil(const QString &name, const QString &UrlPicture)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + _apikey;
    QVariantMap payload;
    payload["displayName"]= name;
    payload["idToken"]=_idToken ;
    payload["returnSecureToken"]= true;
    payload["photoUrl"]= UrlPicture;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::userDetails()
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + _apikey;
    QVariantMap payload;
    payload["idToken"]=_idToken ;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::deleteAccount()
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + _apikey;
    QVariantMap payload;
    payload["idToken"]=_idToken ;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPost(signEndpoint,doc);
}

void SKQtAuth::anonymousLoginIn()
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + _apikey;
    QVariantMap payload;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPostLogin(signEndpoint,doc);
}


bool SKQtAuth::getResponseToken(const QByteArray &response)
{
    QJsonDocument mydoc = QJsonDocument::fromJson(response);
    if(mydoc.object().contains("error"))
    {
        qDebug()<< "error occured : "+response;
        return false;
    }else if(mydoc.object().contains("kind")){
        QString idToken = mydoc.object().value("idToken").toString();
        _idToken = idToken;
        return true;
    }
    return false;
}

QString SKQtAuth::getResponseValue()
{
    return _ResponseValue;
}



SKQtAuth::~SKQtAuth()
{
_manager->deleteLater();
}

