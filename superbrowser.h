#ifndef SUPERBROWSER_H
#define SUPERBROWSER_H

#include <QObject>
#include "webpage.h"
#include "memorycookiejar.h"

class SuperBrowser
{
public:
    SuperBrowser();
    ~SuperBrowser();

private:

    MemoryCookieJar* cookieJar;
};

#endif // SUPERBROWSER_H
