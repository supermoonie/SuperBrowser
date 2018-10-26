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
    QString path = request.url().path();
    if(op != GetOperation && outgoingData != NULL) {
        QByteArray data = readOutgoingData(outgoingData);
        if(request.url().hasQuery()) {
            path.append("&");
        } else {
            path.append("?");
        }
        path.append(data);
    }
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    if(matchWillBeIntercept(request.url())) {
        if(reply->bytesAvailable() > 0) {
            extractMap.insert(path, reply->peek(reply->bytesAvailable()).toBase64());
        } else {
            QByteArray * data = new QByteArray;
            connect(reply, &QNetworkReply::readyRead, [reply, data, path](){
                qint64 size = reply->bytesAvailable();
                QByteArray array = reply->peek(size);
                data->append(array);
            });
            connect(reply, &QNetworkReply::finished, [=](){
                extractMap.insert(path, (*data).toBase64());
                delete data;
            });
        }
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
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

bool NetworkAccessManager::setInterceptor(const QRegExp &interceptor) {
    if(interceptor.isEmpty() || !interceptor.isValid()) {
        return false;
    }
    this->interceptor = interceptor;
    return true;
}

bool NetworkAccessManager::addExtractor(const QRegExp &extractor) {
    if(extractor.isEmpty() || !extractor.isValid()) {
        return false;
    }
    if(this->extractors.contains(extractor)) {
        return false;
    }
    this->extractors.append(extractor);
    return true;
}

QByteArray NetworkAccessManager::extract(const QRegExp &extractor) {
    if(extractor.isEmpty() || !extractor.isValid()) {
        return NULL;
    }
    QString key;
    QList<QString> keys = this->extractMap.keys();
    for(int i = 0; i < keys.size(); i ++) {
        key = keys.at(i);
        if(extractor.exactMatch(key)) {
            break;
        }
    }
    if(key.isEmpty()) {
        return NULL;
    }
    const QString path = key;
    QByteArray data = this->extractMap.value(path);
    extractMap.remove(path);
    return data;
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

bool NetworkAccessManager::matchWillBeExtract(const QUrl url) {
    if(url.isEmpty()) {
        return false;
    }
    QString urlText = url.toString();
    for(int i = 0; i < this->extractors.size(); i ++) {
        QRegExp extractor = extractors.at(i);
        if(extractor.isEmpty() || !extractor.isValid()) {
            continue;
        }
        if(extractor.exactMatch(urlText)) {
            return true;
            break;
        }
    }
    return false;
}
