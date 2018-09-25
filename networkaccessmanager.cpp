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
    if(op != GetOperation && outgoingData != NULL) {
        QByteArray data = readOutgoingData(outgoingData);
        qDebug() << QString(data);
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
            delete buffer;
            break;
        }
        len += step;
        delete buffer;
    }
    return data;
}
