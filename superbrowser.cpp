#include "superbrowser.h"

static SuperBrowser* superBrowser = NULL;

SuperBrowser::SuperBrowser()
{

}

SuperBrowser::~SuperBrowser()
{

}

void SuperBrowser::init() {
    cookieJar = new MemoryCookieJar();

}
