#include "skqtauth.h"
#include "SKQtFirebase_global.h"
#include <QJsonDocument>
#include <QVariantMap>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QtNetworkAuth>
#include <QtCore>
#include <QtNetwork>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>



QString _idToken;
QString _accessToken;
QByteArray _loginData ;
QString _apikey ;

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

void SKQtAuth::signInUser(const QString &_email, const QString& _password)
{
    QString signEndpoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + _apikey;
    QVariantMap payload;
    payload["email"]= _email;
    payload["password"]= _password;
    payload["returnSecureToken"]= true;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    performPostLogin(signEndpoint,doc);
}

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
        requestRefreshToken();

    }

    return response;
}



QString SKQtAuth::readExchangeToken()
{
    QByteArray response=  _reply->readAll();
    _reply->deleteLater();
    _ResponseValue = response;

    qDebug()<<"UID RESPNSE : "<<response;

    QJsonDocument mydoc = QJsonDocument::fromJson(response);
    if(mydoc.object().contains("error"))
    {
        qDebug()<< "error occured : "+response;

    }else{
        _user_id = mydoc.object().value("user_id").toString();
        _idToken = mydoc.object().value("id_token").toString();
        _refreshToken = mydoc.object().value("refresh_token").toString();

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

QString SKQtAuth::getUserUid() const
{
    return _user_id;
}

void SKQtAuth::requestRefreshToken()
{
    QString signEndpoint = "https://securetoken.googleapis.com/v1/token?key=" + _apikey;
    QVariantMap payload;
    payload["grant_type"]="refresh_token" ;
    payload["refresh_token"]=_refreshToken ;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);

    QNetworkRequest networkRequest((QUrl(signEndpoint)));
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    _reply = _manager->post(networkRequest, doc.toJson());
    connect(_reply,&QNetworkReply::readyRead, this,&SKQtAuth::readExchangeToken);
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
        _user_id = mydoc.object().value("localId").toString();
        _refreshToken = mydoc.object().value("refreshToken").toString();
        _idToken = idToken;
        qDebug()<< "refresh Token : "+_refreshToken;
        return true;
    }
    return false;
}

QString SKQtAuth::getResponseValue()
{
    return _ResponseValue;
}

QString SKQtAuth::getToken()
{
    return _idToken;
}



SKQtAuth::~SKQtAuth()
{
    _manager->deleteLater();
    _reply->deleteLater();
}





void SKQtAuth::facebookOAuthWithFirebase(const QString appId, const QString appSecretId, const QString redirectionUrl, QString api_verion)
{

    // get authentification code
    QString  code;


#ifdef Q_OS_ANDROID
    QWebView m_webView;
#elif defined(Q_OS_IOS)
    QWebView m_webView;
#else
    QWebEngineView m_webView;
#endif



    QUuid uuid = QUuid::createUuid();
    QString state = uuid.toString();
    QUrl url("https://www.facebook.com/v"+ api_verion +"/dialog/oauth");
    url.setQuery("client_id=" + appId + "&redirect_uri="+redirectionUrl+"&scope=public_profile,email&state="+state);
    m_webView.load(url);
    m_webView.setVisible(true);

    QEventLoop loop;
    m_webView.show();
    // Connect to redirection url
    QObject::connect(m_webView.page(), &QWebEnginePage::urlChanged, [&](const QUrl &url) {
        if (url.toString().startsWith(redirectionUrl)) {
            QUrlQuery query(url);
            code = query.queryItemValue("code");

            if (!query.hasQueryItem("state")) {
                // If state is missing, throw an error
                qDebug() <<"Missing state in auth response"<<query.queryItemValue("state");
            }

            loop.quit();
        }
    });

    loop.exec();

    // exchange code to access token
    QUrl url2("https://graph.facebook.com/v"+ api_verion +"/oauth/access_token");
    QUrlQuery query;
    query.addQueryItem("client_id", appId);
    query.addQueryItem("client_secret", appSecretId);
    query.addQueryItem("redirect_uri", redirectionUrl);
    query.addQueryItem("code", code);
    query.addQueryItem("state", state);
    url.setQuery(query);

    QUrl urlWithQuery = url2;
    urlWithQuery.setQuery(query);

    QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(urlWithQuery);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QNetworkReply *reply = networkManager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());

    QEventLoop loop3;

    connect(reply, &QNetworkReply::finished, [reply, &loop3](){
        if (reply->error()) {
            qDebug() << "Error: " << reply->errorString();
        } else {
            QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
            QJsonObject jsonObject = json.object();
            _accessToken = jsonObject["access_token"].toString();
            loop3.quit();

        }
        reply->deleteLater();
    });
    loop3.exec();


    // login with access token
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url3("https://identitytoolkit.googleapis.com/v1/accounts:signInWithIdp?key="+_apikey);
    QJsonObject json;
    json.insert("returnSecureToken", true);
    json["requestUri"] = QString("https://www.facebook.com/connect/login_success.html");
    json["returnIdpCredential"] = true;
    json["postBody"] = QString("access_token="+_accessToken+"&providerId=facebook.com");

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();
    QNetworkRequest request3(url3);
    request3.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply3 = manager->post(request3, jsonData);

    QEventLoop loup;
    QObject::connect(reply3, &QNetworkReply::finished, [&, reply3]() {
        if (reply3->error() != QNetworkReply::NoError) {
            qDebug() << "Error authenticating with Firebase:" << reply3->errorString();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply3->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "reponse :" <<jsonObject;
        } else {
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply3->readAll());
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "reponse :" <<jsonObject;
            if (jsonObject.contains("idToken")) {
                QString idToken = jsonObject["idToken"].toString();
                _idToken = idToken;
                _user_id = jsonObject["localId"].toString();
                _refreshToken = jsonObject["refreshToken"].toString();
                _loginData = jsonResponse.toJson(QJsonDocument::Compact);
                successAuth();

            } else {
                qDebug() << "Error authenticating with Firebase:" << jsonObject["error"].toObject()["message"].toString();
            }
        }

        loup.quit();
        reply3->deleteLater();
    });
    loup.exec();
    manager->deleteLater();


}



QByteArray SKQtAuth::loginData() const
{

    return _loginData;
}

