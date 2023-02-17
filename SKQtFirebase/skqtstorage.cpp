#include "skqtstorage.h"

SKQtStorage::SKQtStorage(QObject *parent): QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
}

void SKQtStorage::setProjectID(const QString projectId)
{
    _projectId = projectId;
}

QString SKQtStorage::downloadImageFromFirebaseStorage(const QString &fileName)
{
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

bool SKQtStorage::removeFirestorageItem(const QString &fileName)
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

void SKQtStorage::sendToStorage(QString imageName, QString folder, QString imagePath)
{
    // Create a new network access manager
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    // Create a new request to upload the image
    QNetworkRequest request;
    request.setUrl(QUrl("https://firebasestorage.googleapis.com/v0/b/"+_projectId+".appspot.com/o/"+folder+"%2F"+imageName));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    request.setRawHeader("Authorization", "Bearer " + _idToken.toUtf8());

    // Open the image file
    QFile* file = new QFile(imagePath);
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

SKQtStorage::~SKQtStorage()
{
    _manager->deleteLater();
    _reply->deleteLater();
}
