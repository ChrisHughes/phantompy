#include "context.hpp"

namespace ph {

static Context *contextInstance = NULL;

Context::Context(QObject *parent): QObject(parent) {
    char *argv;
    int argc = 1;

    p_mutex = new QMutex();
    p_app = new QApplication(argc, &argv);
    
    // Sets app name and version to avoid segfault while getting default UserAgent when code is accessed outside of main library 
    p_app->setApplicationName(QString("PhantomPy"));
    p_app->setApplicationVersion(QString("0.0.1"));
    
    p_ep = new EventProcessor(200, this);

    setDefaultSettings();
}

Context::~Context() {
    delete p_app;
    delete p_ep;
    delete p_mutex;
}

Context* Context::instance() {
    if (contextInstance == NULL) {
        contextInstance = new Context();
    }

    return contextInstance;
}

void Context::clearInstance() {
    if (contextInstance != NULL) {
        delete contextInstance;
        contextInstance = NULL;
    }
}

QString Context::getUserAgent(){
	return m_userAgent;
}

void Context::setUserAgent(const QString &userAgent){
	m_userAgent = userAgent;
}

void Context::setHTTPHeaders(QHash<QString, QString> &headers){
	QHash<QString, QString>::iterator i;
	for (i = headers.begin(); i != headers.end(); ++i){
		m_headers[i.key()] = i.value();
	}
}

void Context::setHTTPHeaders(const QVariantMap &headers){
	QVariantMap::const_iterator i;
	QHash<QString, QString> hashHeaders;
    for(i=headers.begin(); i!=headers.end(); ++i) {
		hashHeaders[i.key()] = i.value().toString();
    }
    setHTTPHeaders(hashHeaders);
}

void Context::applyHTTPHeaders(QNetworkRequest &request){
	QHash<QString, QString> headers = Context::instance()->getHTTPHeaders();
    QHash<QString, QString>::iterator i;
    for (i = headers.begin(); i != headers.end(); ++i){
		QByteArray key = QByteArray(i.key().toStdString().c_str(), i.key().length());
		QByteArray value = QByteArray(i.value().toStdString().c_str(), i.value().length());
		request.setRawHeader(key, value);
	}
}

QHash<QString, QString> Context::getHTTPHeaders(){
	return m_headers;
}

void Context::setMaximumPagesInCache(int pages) {
   QWebSettings::setMaximumPagesInCache(pages);
}

void Context::setObjectCacheCapacities(int cacheMinDeadCapacity, int cacheMaxDead, int totalCapacity) {
   QWebSettings::setObjectCacheCapacities(cacheMinDeadCapacity, cacheMaxDead, totalCapacity);
}

void Context::clearMemoryCaches() {
    QWebSettings::clearMemoryCaches();
}

void Context::setConfig(const Settings &key, const QVariant &value) {
    m_settings[key] = value;
}

QVariant Context::getConfig(const Settings &key) {
    return m_settings[key];
}

void Context::setDefaultSettings() {
    m_settings[ph::LoadImages] = QVariant::fromValue(true);
    m_settings[ph::Javascript] = QVariant::fromValue(true);
    m_settings[ph::DnsPrefetching] = QVariant::fromValue(true);
    m_settings[ph::Plugins] = QVariant::fromValue(false);
    m_settings[ph::PrivateBrowsing] = QVariant::fromValue(false);
    m_settings[ph::OfflineStorageDB] = QVariant::fromValue(true);
    m_settings[ph::OfflineAppCache] = QVariant::fromValue(true);
    m_settings[ph::OfflineStorageQuota] = QVariant::fromValue(0);
    m_settings[ph::FrameFlattening] = QVariant::fromValue(true);
    m_settings[ph::LocalStorage] = QVariant::fromValue(true);
}

// Shortcuts

bool Context::settingsLoadImagesEnabled() {
    return m_settings[ph::LoadImages].toBool();
}

bool Context::settingsJavascriptEnabled() {
    return m_settings[ph::Javascript].toBool();
}

bool Context::settingsDnsPrefetchEnabled() {
    return m_settings[ph::DnsPrefetching].toBool();
}

bool Context::settingsPluginsEnabled() {
    return m_settings[ph::Plugins].toBool();
}

bool Context::settingsPrivateBrowsingEnabled() {
    return m_settings[ph::PrivateBrowsing].toBool();
}

bool Context::settingsFrameFlatteningEnabled() {
    return m_settings[ph::FrameFlattening].toBool();
}

bool Context::settingsLocalStorageEnabled() {
    return m_settings[ph::LocalStorage].toBool();
}

bool Context::settingsOfflineStorageDatabaseEnabled() {
    return m_settings[ph::OfflineStorageDB].toBool();
}

bool Context::settingsOfflineApplicationCacheEnabled() {
    return m_settings[ph::OfflineAppCache].toBool();
}

int Context::settingsofflineStorageDefaultQuota() {
    return m_settings[ph::OfflineStorageQuota].toInt();
}

QApplication* Context::app() {
    return p_app;
}

QMutex* Context::mutex() {
    return p_mutex;
}

}
