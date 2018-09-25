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
    QString url = request.url().toString();
    if(!this->interceptor.isEmpty() && url.contains(this->interceptor))
    {
        QNetworkRequest req;
        req.setUrl(QUrl(""));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
    QString path = request.url().path();
    if(op != GetOperation && outgoingData != NULL) {
        QByteArray data = readOutgoingData(outgoingData);
        path.append("?").append(data);
    }
    QNetworkReply * reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    bool flag = false;
    for(int i = 0; i < this->extractors.size(); i ++) {
        QRegExp extractor = extractors.at(i);
        if(extractor.exactMatch(path)) {
            flag = true;
            break;
        }
    }
    if(flag) {
        qDebug() << path << " will be save ";
        if(reply->bytesAvailable() > 0) {
            extractMap.insertMulti(path, reply->peek(reply->bytesAvailable()).toBase64());
        } else {
            QByteArray * data = new QByteArray;
            connect(reply, &QNetworkReply::readyRead, [reply, data, path](){
                qDebug() << path << " readyRead ";
                qint64 size = reply->bytesAvailable();
                QByteArray array = reply->peek(size);
                data->append(array);
            });
            connect(reply, &QNetworkReply::finished, [=](){
                qDebug() << path << " finished ";
                extractMap.insertMulti(path, (*data).toBase64());
                delete data;
            });
        }
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
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
