#ifndef SKQTSTORAGE_H
#define SKQTSTORAGE_H

#include <QObject>
#include "SKQtFirebase_global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "skqtauth.h"
#include <QtQml/QtQml>
#include <QMap>
#include <QJsonDocument>

class SKQTFIREBASE_EXPORT  SKQtStorage :public QObject
{
    Q_OBJECT
public:
    SKQtStorage(QObject *parent = nullptr);
    Q_INVOKABLE void setProjectID(const QString projectId);
    Q_INVOKABLE QString downloadImageFromFirebaseStorage(const QString &fileName);
    Q_INVOKABLE bool removeFirestorageItem(const QString &fileName);
    Q_INVOKABLE void sendToStorage(QString imageName, QString folder, QString imagePath);
    ~SKQtStorage();

private :
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _projectId;
};

#endif // SKQTSTORAGE_H
