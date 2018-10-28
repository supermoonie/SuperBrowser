#include "networkaccessmanager.h"

NetworkAccessManager::NetworkAccessManager(MemoryCookieJar *cookieJar, QObject *parent)
    : QNetworkAccessManager(parent)
{
    this->cookieJar = cookieJar;
    this->setCookieJar(cookieJar);
    diskCache = new QNetworkDiskCache(this);
}

NetworkAccessManager::~NetworkAccessManager()
{

}

QNetworkReply* NetworkAccessManager::createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData) {
    if(matchWillBeIntercept(request.url())) {
        QNetworkRequest req;
        req.setUrl(QUrl("about:blank"));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    QString url = request.url().toString();
    if(op != GetOperation && outgoingData != NULL) {
        QByteArray data = readOutgoingData(outgoingData);
        if(request.url().hasQuery()) {
            url.append("&");
        } else {
            url.append("?");
        }
        url.append(QString(data));
    }
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    QString extractor = matchWillBeExtract(QUrl(url));
    if(!extractor.isNull()) {
        if(reply->bytesAvailable() > 0) {
            QString base64Data = QString(reply->peek(reply->bytesAvailable()).toBase64());
            extractMap.insert(extractor, base64Data);
            emit dataExtracted(extractor, base64Data);
        } else {
            QByteArray * data = new QByteArray;
            connect(reply, &QNetworkReply::readyRead, [reply, data, extractor](){
                qint64 size = reply->bytesAvailable();
                QByteArray array = reply->peek(size);
                data->append(array);
            });
            connect(reply, &QNetworkReply::finished, [=](){
                QString base64Data = QString((*data).toBase64());
                extractMap.insert(extractor, base64Data);
                delete data;
                qDebug() << "url: " + url << "base64: " + base64Data;
                emit NetworkAccessManager::dataExtracted(extractor, base64Data);
            });
        }
    }
    return reply;
}

void NetworkAccessManager::setInterceptors(const QList<QString> &interceptors) {
    for(int i = 0; i < interceptors.size(); i ++) {
        QString interceptor = interceptors.at(i);
        if(!interceptor.isEmpty()) {
            this->interceptors << interceptor;
        }
    }
}

QList<QString> NetworkAccessManager::getInterceptors() {
    return this->interceptors;
}

QString NetworkAccessManager::extract(const QString &extractor) {
    if(extractor.isEmpty()) {
        return NULL;
    }
    if(this->extractMap.contains(extractor) && !this->extractMap.value(extractor).isEmpty()) {
        return this->extractMap.value(extractor);
    }
    return NULL;
}

QByteArray NetworkAccessManager::readOutgoingData(QIODevice *outgoingData) {
    if(outgoingData == NULL) {
        return NULL;
    }
    QByteArray data;
    const qint64 step = 256;
    qint64 len = step;
    while (true) {
        char* buffer = new char[len];
        qint64 count = outgoingData->peek(buffer, len);
        if(count < len) {
            data = QByteArray(buffer, count);
            delete[] buffer;
            break;
        }
        len += step;
        delete[] buffer;
    }
    return data;
}

bool NetworkAccessManager::matchWillBeIntercept(const QUrl url) {
    if(!url.isEmpty() && interceptors.contains(url.toString())) {
        return true;
    }
    return false;
}

QString NetworkAccessManager::matchWillBeExtract(const QUrl url) {
    if(url.isEmpty()) {
        return NULL;
    }
    QString urlText = url.toString();
    QString path = url.path();
    for(QMap<QString, QString>::iterator it = this->extractMap.begin(); it != this->extractMap.end(); it ++) {
        if(it.key().startsWith("http") && urlText == it.key()) {
            return it.key();
        }
        if(it.key().startsWith("/") && path == it.key()) {
            return it.key();
        }
        QRegExp regExp(it.key());
        if(regExp.isValid() && regExp.exactMatch(urlText)) {
            return it.key();
        }
    }
    return NULL;
}

void NetworkAccessManager::setExtractorMap(QMap<QString, QString> &extractMap) {
    this->extractMap = extractMap;
}
