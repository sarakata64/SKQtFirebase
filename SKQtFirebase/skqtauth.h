#ifndef SKQTAUTH_H
#define SKQTAUTH_H

#include <QObject>
#include "SKQtFirebase_global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtQml/QtQml>


#ifdef Q_OS_ANDROID
#include <QtWebView>
#elif defined(Q_OS_IOS)
#include <QtWebView>
#else
#include <QWebEngineView>
#endif

class SKQTFIREBASE_EXPORT SKQtAuth : public QObject
{
    Q_OBJECT
    Q_PROPERTY( READ getResponseValue NOTIFY errorAuth)
    Q_PROPERTY(  READ getResponseToken NOTIFY successAuth)

public:
   explicit SKQtAuth(QObject *parent = nullptr);

   Q_INVOKABLE void setApiKey( const QString & apikey);
   Q_INVOKABLE void signUpUser(const QString &email,const QString & password);
   Q_INVOKABLE void signInUser(const QString &_email,const QString & _password);
   Q_INVOKABLE void performPost(const QString & url, const QJsonDocument &payload);
   Q_INVOKABLE void performPostLogin(const QString & url, const QJsonDocument &payload);
   Q_INVOKABLE void resetPassword(const QString & email);
   Q_INVOKABLE void changeEmail(const QString & new_email);
   Q_INVOKABLE void changePassword(const QString & new_password);
   Q_INVOKABLE void updateProfil(const QString &name, const QString & UrlPicture);
   Q_INVOKABLE void userDetails();
   Q_INVOKABLE QString getUserUid() const;
   Q_INVOKABLE void requestRefreshToken();
   Q_INVOKABLE void deleteAccount();
   Q_INVOKABLE void anonymousLoginIn();
   Q_INVOKABLE bool getResponseToken(const QByteArray & response);
   Q_INVOKABLE QString getResponseValue() ;
   Q_INVOKABLE QString getToken();
   Q_INVOKABLE void facebookOAuthWithFirebase(const QString appId,const QString appSecretId, const QString redirectionUrl, QString api_verion= "15.0");
   Q_INVOKABLE QByteArray loginData() const;
  // QAbstractOAuth::ModifyParametersFunction buildModifyParametersFunction();


   ~SKQtAuth();

public slots:
    QString readResponse();
    QString readResponseLogin();
    QString readExchangeToken();



signals :
    Q_INVOKABLE void errorAuth();
    Q_INVOKABLE void successAuth();

private:


    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _ResponseValue;
    QString _Error;
    QString _Success;
    QString _user_id;
    QString _refreshToken;

};

#endif // SKQTAUTH_H
