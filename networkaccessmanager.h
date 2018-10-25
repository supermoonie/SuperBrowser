#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QRegExp>
#include <QList>
#include <QMap>
#include <memorycookiejar.h>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkAccessManager(MemoryCookieJar* cookieJar, QObject *parent = 0);
    ~NetworkAccessManager();

public:
    bool setInterceptor(const QRegExp &interceptor);
    void setInterceptors(const QList<QString> &interceptors);
    bool addExtractor(const QRegExp &extractor);
    QByteArray extract(const QRegExp &extractor);

private:
    MemoryCookieJar* cookieJar;
    QNetworkDiskCache* diskCache;
    QRegExp interceptor;
    QList<QString> interceptors;
    QList<QRegExp> extractors;
    QMap<QString, QByteArray> extractMap;

private:
    QByteArray readOutgoingData(QIODevice* outgoingData);
    bool match(const QUrl url);

signals:

public slots:

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H
