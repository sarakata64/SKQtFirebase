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
  Q_INVOKABLE  void POST(QString url, QVariantMap map);
  Q_INVOKABLE  void setDataRootUrl(QString  url);
  Q_INVOKABLE  void deleteData(QString child);
  Q_INVOKABLE  void upDateData(QString child,QString key, QString value);
  Q_INVOKABLE  QByteArray data();
  Q_INVOKABLE QString downloadImageFromFirebaseStorage(const QString &fileName);
  Q_INVOKABLE void setProjectID(const QString projectId);
  Q_INVOKABLE bool removeFirestorageItem(const QString &fileName);
    ~SKQtRealTimeDatabase();

  Q_INVOKABLE void sendToStorage(QString imageName, QString folder, QString imagePath);


public slots:
   QByteArray getData();


signals :
  Q_INVOKABLE void dataAvailable();
  void imageReceived(const QImage &image);


private:
    QByteArray _arr;
    QNetworkAccessManager * _manager;
    QNetworkReply * _reply;
    QString _dataUrl;
    QString _projectId;
    QJsonObject _obj;


};

#endif // SKQTREALTIMEDATABASE_H
