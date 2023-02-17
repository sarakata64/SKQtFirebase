#include "skqtrealtimedatabase.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QImageWriter>
#include "SKQtFirebase_global.h"
#include <QImageReader>


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
    delete buff;
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

void SKQtRealTimeDatabase::sendToStorage(QString imageName, QString folder, QString imagePath)
{


    // Create a new network access manager
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // Create a new request to upload the image
    QNetworkRequest request;
    request.setUrl(QUrl("https://firebasestorage.googleapis.com/v0/b/"+_projectId+".appspot.com/o/"+folder+"%2F"+imageName));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setRawHeader("Authorization", "Bearer " + _idToken.toUtf8());

    // Open the image file
    QFile* file = new QFile(imagePath); //"C:\\Users\\sarak\\OneDrive\\Desktop\\TUTO\\essai.mp4"
    file->open(QIODevice::ReadOnly);

    // Send the request to upload the image
    QNetworkReply* reply = manager->post(request, file);
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error uploading image:" << reply->errorString();
            return;
        }

        // Parse the JSON response from the server
        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = json.object();

        // Get the URL of the uploaded image
        QString url = obj["downloadUrl"].toString();
        qDebug() << "Image uploaded successfully! URL:" << url;
    });
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


QString SKQtRealTimeDatabase::downloadImageFromFirebaseStorage(const QString &fileName) {


    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request((QUrl("https://firebasestorage.googleapis.com/v0/b/"+_projectId+".appspot.com/o?uploadType=media&name="+fileName)));
    request.setRawHeader("Authorization", QString("Bearer %1").arg(_idToken).toUtf8());
    QNetworkReply *reply = manager->get(request);
    QString imagePath;
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, [&, reply]() {


        QByteArray imdata = reply->readAll();


        if (reply->error()) {
            qDebug() << "Download error: " << reply->errorString();
            reply->deleteLater();
            return;
        }

        QBuffer buffer(&imdata);
        buffer.open(QIODevice::WriteOnly);
        QString image("data:image/png;base64,");
        image.append(QString::fromLatin1(imdata.toBase64().data()));
        imagePath = image;


        loop.quit();
        reply->deleteLater();
    });
    loop.exec();
    return imagePath;
}

void SKQtRealTimeDatabase::setProjectID(const QString projectId)
{
    _projectId = projectId;
}

bool SKQtRealTimeDatabase::removeFirestorageItem(const QString &fileName)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString("https://firebasestorage.googleapis.com/v0/b/"+_projectId+".appspot.com/o?alt=media&name="+fileName)));
    request.setRawHeader("Authorization", QString("Bearer %1").arg(_idToken).toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("User-Agent", "App");
    request.setAttribute(QNetworkRequest::CustomVerbAttribute, "DELETE");

    QNetworkReply *reply = _manager->deleteResource(request);
    connect(reply, &QNetworkReply::finished, [reply] {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "removed successful";
            return true;
        } else {
            qDebug() << "removing FireStorage failed, error : " << reply->errorString();
            return false;
        }
        reply->deleteLater();
    });
    return false;
}




