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
  Q_INVOKABLE  QByteArray readData(QString url);
  Q_INVOKABLE  void put(QString url, QVariantMap map);
  Q_INVOKABLE  void post(QString url, QVariantMap map);
  Q_INVOKABLE  void setDataRootUrl(QString  url);
  Q_INVOKABLE  void deleteData(QString child);
  Q_INVOKABLE  void patch(QString child,QString key, QString value);
  Q_INVOKABLE  QByteArray data();
               void handleError(QNetworkReply::NetworkError error);



    ~SKQtRealTimeDatabase();




public slots:
   QByteArray getData();


signals :
  Q_INVOKABLE void dataAvailable();
  //void imageReceived(const QImage &image);


private:
    QByteArray _arr;
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _dataUrl;
    QJsonObject _obj;
    QEventLoop  _eventLoop;


};

#endif // SKQTREALTIMEDATABASE_H
