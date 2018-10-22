#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
    ~Config();
    static Config* instance(QObject *parent = 0);
    void setDebug();
    bool isDebug();
    void setRelease();
    bool isRelease();

private:
    bool debug;

signals:

public slots:
};

#endif // CONFIG_H
