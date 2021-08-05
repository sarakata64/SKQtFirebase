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
        Q_PROPERTY( QByteArray getData READ getData NOTIFY dataAvailable)
public:
   explicit SKQtRealTimeDatabase(QObject *parent = nullptr);
  Q_INVOKABLE  void readData(QString url);
  Q_INVOKABLE  void sendData(QString url, QVariantMap map);
  Q_INVOKABLE  void replaceData(QString url, QVariantMap map);
  Q_INVOKABLE  void setDataRootUrl(QString  url);
  Q_INVOKABLE  void deleteData(QString child);
  Q_INVOKABLE  void upDateData(QString child,QString key, QString value);

    ~SKQtRealTimeDatabase();
     Q_INVOKABLE QByteArray _arr;
public slots:
   QByteArray getData();

signals :
  Q_INVOKABLE void dataAvailable();


private:
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _dataUrl;
    QJsonObject _obj;

};

#endif // SKQTREALTIMEDATABASE_H
