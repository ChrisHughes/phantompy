#include "page.hpp"
#include "context.hpp"
#include "cookiejar.hpp"

namespace ph {

Page::Page(QObject *parent):QWebPage(parent), m_networkManager(this) {
    setNetworkAccessManager(&m_networkManager);
    m_nmProxy.setNetworkAccessManager(&m_networkManager);

    applySettings();

    connect(this, &QWebPage::loadFinished, this, &Page::loadFinished);

    connect(&m_networkManager, &NetworkManager::replyReceived, this, &Page::replyReceived);
    
    m_loaded = false;
    m_error = false;
    
}

Page::~Page() {}

void Page::applySettings() {
    Context *ctx = Context::instance();

    settings()->setAttribute(QWebSettings::AutoLoadImages, ctx->settingsLoadImagesEnabled());
    settings()->setAttribute(QWebSettings::DnsPrefetchEnabled, ctx->settingsDnsPrefetchEnabled());
    settings()->setAttribute(QWebSettings::JavascriptEnabled, ctx->settingsJavascriptEnabled());
    settings()->setAttribute(QWebSettings::PluginsEnabled, ctx->settingsPluginsEnabled());
    settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, ctx->settingsPrivateBrowsingEnabled());
    settings()->setAttribute(QWebSettings::FrameFlatteningEnabled, ctx->settingsFrameFlatteningEnabled());
    settings()->setAttribute(QWebSettings::LocalStorageEnabled, ctx->settingsLocalStorageEnabled());
    settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, ctx->settingsOfflineApplicationCacheEnabled());
    settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, ctx->settingsOfflineStorageDatabaseEnabled());

    // TODO: expose these settings to ctypes api
    settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings()->setAttribute(QWebSettings::JavascriptCanCloseWindows, false);
    settings()->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
    settings()->setAttribute(QWebSettings::SiteSpecificQuirksEnabled, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
}

void Page::load(const QString &_url) {
    m_mainUrl = QUrl::fromUserInput(_url);
	QNetworkRequest request = QNetworkRequest(m_mainUrl);
	Context::instance()->applyHTTPHeaders(request);
	m_loaded = false;
    mainFrame()->load(request);
    mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    setViewportSize(m_viewSize);
    m_eventLoop.exec();
}

QString Page::userAgentForUrl(const QUrl &_url) const {
	QString userAgent = Context::instance()->getUserAgent();
	if (userAgent.isNull()){
		return QWebPage::userAgentForUrl(_url);
	}
	return userAgent;
}

void Page::setViewSize(int x, int y) {
    this->m_viewSize = QSize(x, y);
}

void Page::loadFinished(bool ok) {
    qDebug() << "Page::loadFinished(ok)";
    m_eventLoop.quit();
    m_error = ok;
	m_loaded = true;
}

bool Page::isLoaded() {
    return m_loaded;
}

bool Page::hasLoadErrors() {
    return m_error;
}

QVariantList Page::getCookies() {
    return CookieJar::instance()->getCookies(mainFrame()->url().toString());
}

QSet<QString> Page::requestedUrls() {
    return m_requestedUrls;
}

QVariantMap Page::getResponseByUrl(const QString &url) {
    return m_responsesCache[url];
}

void Page::replyReceived(const QVariantMap &reply) {
    qDebug() << "background-request:" << reply["url"].toString();
    m_requestedUrls.insert(reply["url"].toString());
    m_responsesCache.insert(reply["url"].toString(), reply);
}

void Page::setInitialCookies(const QVariantList &cookies) {
    m_initialCookies = cookies;
}

}

