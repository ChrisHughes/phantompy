#include "networkmanagerproxy.hpp"
#include "context.hpp"

namespace ph {

SyncNetworkManagerProxy::SyncNetworkManagerProxy(QObject *parent): QObject(parent) {}
SyncNetworkManagerProxy::~SyncNetworkManagerProxy() {}

void SyncNetworkManagerProxy::setNetworkAccessManager(QNetworkAccessManager *manager) {
    p_manager = manager;
}

QNetworkReply* SyncNetworkManagerProxy::get(const QUrl &url) {
    QNetworkRequest request = QNetworkRequest(url);
    return get(request);
}

QNetworkReply* SyncNetworkManagerProxy::get(QNetworkRequest &request) {
	Context::instance()->applyHTTPHeaders(request);
	
    QNetworkReply *reply = p_manager->get(request);

    QMetaObject::Connection connection = connect(reply, &QNetworkReply::finished,
                                                 this, &SyncNetworkManagerProxy::finished);
    m_eventLoop.exec();
    QObject::disconnect(connection);

    return reply;
}

void SyncNetworkManagerProxy::finished() {
    m_eventLoop.quit();
}

}
