#include "superbrowser.h"

static SuperBrowser* superBrowser = NULL;

SuperBrowser::SuperBrowser()
{
    cookieJar = MemoryCookieJar::instance();
}

SuperBrowser::~SuperBrowser()
{

}
