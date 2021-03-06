#include "networkaccessmanager.h"

NetworkAccessManager::NetworkAccessManager(MemoryCookieJar *cookieJar, QObject *parent)
    : QNetworkAccessManager(parent)
{
    this->cookieJar = cookieJar;
    this->setCookieJar(cookieJar);
    diskCache = new QNetworkDiskCache(this);
    connect(this, &NetworkAccessManager::sslErrors, [](QNetworkReply * reply, const QList<QSslError> & errors){
        Q_UNUSED(errors);
        reply->ignoreSslErrors();
    });
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
    QNetworkReply * reply = NULL;
    if(op == PostOperation && !request.header(QNetworkRequest::ContentTypeHeader).isValid()) {
        QNetworkRequest req(request);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(tr("application/x-www-form-urlencoded")));
        reply = QNetworkAccessManager::createRequest(op, req, outgoingData);
    } else {
        reply = QNetworkAccessManager::createRequest(op, request, outgoingData);
    }
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
    bool exist = this->extractMap.contains(extractor)
            && !this->extractMap.value(extractor).isNull();
    if(exist) {
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
    for(QString extractor : this->extractors) {
        if(extractor.startsWith("http") && urlText == extractor) {
            return extractor;
        }
        if(extractor.startsWith("/") && path == extractor) {
            return extractor;
        }
        QRegExp regExp(extractor);
        if(regExp.isValid() && regExp.exactMatch(urlText)) {
            return extractor;
        }
    }
    return NULL;
}

void NetworkAccessManager::setExtractors(QStringList &extractors) {
    this->extractors = extractors;
}
