#ifndef NETWORKACCESSMANAGER_H
#define NETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkDiskCache>
#include <QRegExp>
#include <memorycookiejar.h>

class NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkAccessManager(MemoryCookieJar* cookieJar, QObject *parent = 0);
    ~NetworkAccessManager();

public:
    bool setInterceptor(const QRegExp &interceptor);

private:
    MemoryCookieJar* cookieJar;
    QNetworkDiskCache* diskCache;
    QRegExp interceptor;

private:
    QByteArray readOutgoingData(QIODevice* outgoingData);

signals:

public slots:

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H
