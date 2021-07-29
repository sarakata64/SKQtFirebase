#ifndef SKQTAUTH_H
#define SKQTAUTH_H

#include <QObject>
#include "SKQtFirebase_global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtQml/QtQml>

class SKQTFIREBASE_EXPORT SKQtAuth : public QObject
{
    Q_OBJECT
   // Q_PROPERTY(QString getResponseValue READ getResponseValue NOTIFY userSignedIn)
    Q_PROPERTY(QString getResponseValue  READ getResponseValue NOTIFY errorAuth)
    Q_PROPERTY(QString getResponseValue  READ getResponseValue NOTIFY successAuth)

public:
   explicit SKQtAuth(QObject *parent = nullptr);
   Q_INVOKABLE void setApiKey( const QString & apikey);
   Q_INVOKABLE void signUpUser(const QString &email,const QString & password);
   Q_INVOKABLE void signInUser(const QString &email,const QString & password);
   Q_INVOKABLE void performPost(const QString & url, const QJsonDocument &payload);
     Q_INVOKABLE void performPostLogin(const QString & url, const QJsonDocument &payload);
   Q_INVOKABLE void resetPassword(const QString & email);
   Q_INVOKABLE void changeEmail(const QString & new_email);
   Q_INVOKABLE void changePassword(const QString & new_password);
   Q_INVOKABLE void updateProfil(const QString &name, const QString & UrlPicture);
   Q_INVOKABLE void userDetails();
   Q_INVOKABLE void deleteAccount();
   Q_INVOKABLE void anonymousLoginIn();
   Q_INVOKABLE bool getResponseToken(const QByteArray & response);
   Q_INVOKABLE QString getResponseValue() ;


   ~SKQtAuth();

public slots:
    QString readResponse();
    QString readResponseLogin();

signals :
    Q_INVOKABLE void errorAuth();
    Q_INVOKABLE void successAuth();

private:

    QString _apikey;
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _ResponseValue;
    QString _Error;
    QString _Success;


};

#endif // SKQTAUTH_H
