#include "config.h"

static Config* config = NULL;

Config::Config(QObject *parent) : QObject(parent)
{
    debug = true;
}

Config::~Config()
{

}

Config* Config::instance(QObject *parent) {
    if(NULL == config) {
        config = new Config(parent);
    }
    return config;
}

void Config::setDebug() {
    debug = true;
}

bool Config::isDebug() {
    return debug;
}

void Config::setRelease() {
    debug = false;
}

bool Config::isRelease() {
    return !debug;
}
