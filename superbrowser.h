#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include "memorycookiejar.h"

class SuperBrowser
{
public:
    SuperBrowser();
    ~SuperBrowser();
    void init();

private:
    MemoryCookieJar* cookieJar;
};

#endif // SUPERBROWSER_H
