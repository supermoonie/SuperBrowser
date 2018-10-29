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
    void setInterceptors(const QList<QString> &interceptors);
    QList<QString> getInterceptors();
    void setExtractors(QStringList &extractors);
    QString extract(const QString &extractor);

private:
    MemoryCookieJar* cookieJar;
    QNetworkDiskCache* diskCache;
    QList<QString> interceptors;
    QStringList extractors;
    QMap<QString, QString> extractMap;

private:
    QByteArray readOutgoingData(QIODevice* outgoingData);
    bool matchWillBeIntercept(const QUrl url);
    QString matchWillBeExtract(const QUrl url);

signals:
    void dataExtracted(const QString &extractor, const QString &base64Data);

public slots:

protected:
    QNetworkReply * createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // NETWORKACCESSMANAGER_H
