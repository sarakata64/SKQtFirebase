#ifndef SKQTREALTIMEDATABASE_H
#define SKQTREALTIMEDATABASE_H

#include <QObject>
#include "SKQtFirebase_global.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "skqtauth.h"
#include <QtQml/QtQml>
#include <QMap>
#include <QJsonDocument>

class SKQTFIREBASE_EXPORT SKQtRealTimeDatabase: public QObject
{
        Q_OBJECT
        Q_PROPERTY(QJsonDocument getData READ getData NOTIFY dataAvaible)
public:
   explicit SKQtRealTimeDatabase(QObject *parent = nullptr);
  Q_INVOKABLE  void readData(QString url);
  Q_INVOKABLE  void sendData(QString url, QVariantMap map);
  Q_INVOKABLE  void replaceData(QString url, QVariantMap map);
  Q_INVOKABLE  void setDataRootUrl(QString  url);
  Q_INVOKABLE  void deleteData(QString child);
  Q_INVOKABLE  void upDateData(QString child,QString key, QString value);
    ~SKQtRealTimeDatabase();
public slots:
    QJsonDocument getData();

signals :
  Q_INVOKABLE void dataAvaible();


private:
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _dataUrl;
};

#endif // SKQTREALTIMEDATABASE_H
