#include "rest.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QTextStream>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QFile>
#include <QCoreApplication>
#include <QFileInfo>
#include <QStandardPaths>

REST::REST(QObject *parent) : QObject(parent)
{
    data_path = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if(!data_path.exists())
    {
        data_path.mkpath(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    }
    if(authtoken == nullptr){
        QFile f_token(data_path.absolutePath()+"/token.dat");
        f_token.open(QFile::ReadOnly);
        if(f_token.isOpen()){
            QTextStream f_token_d(&f_token);
            authtoken = f_token_d.readAll().trimmed();
        }
        f_token.close();
    }
    this->m_manager = new QNetworkAccessManager();
    //QObject::connect(this->m_manager, SIGNAL(networkAccessibleChanged()), this, SLOT(accessible()));
}

QDir REST::data_path;

QString REST::API_URL = QStringLiteral("http://192.168.1.5/api/");
//QString REST::API_URL = QStringLiteral("http://[fe80::7c0e:ee52:5eb3:9b56]/api/");


QString REST::authtoken = nullptr;

QNetworkRequest* REST::request(char verb)
{
    emit aboutSend();
    QString url = API_URL+this->endpoint;
    foreach (QString param, this->params) {
        url += "/"+param;
    }

    QNetworkRequest *request = new QNetworkRequest(url);

    request->setRawHeader("Connection","close");
    request->setRawHeader("Accept","application/json");
    request->setRawHeader("Content-type","application/json; charset=UTF-8");
    if(this->authenticate && authtoken != nullptr)
        request->setRawHeader("Authorization", QByteArray::fromStdString(("Bearer " + authtoken).toStdString()));
    if(deviceHash != nullptr)
        request->setRawHeader("device-hash", QByteArray::fromStdString(deviceHash.toStdString()));

    switch (verb) {
    case 'p':{
            QJsonDocument json = QJsonDocument::fromVariant(data.value<QJSValue>().toVariant());
            this->m_reply = this->m_manager->post(*request, json.toJson());
        }break;
    case 'u':{
            QJsonDocument json = QJsonDocument::fromVariant(data.value<QJSValue>().toVariant());
            this->m_reply = this->m_manager->put(*request, json.toJson());
        }break;
    case 'g':
        this->m_reply = this->m_manager->get(*request);
        break;
    case 'd':
        this->m_reply = this->m_manager->deleteResource(*request);
        break;
    }

    QObject::connect(this->m_reply, &QNetworkReply::finished, this, &REST::finishGetUrl);

    QObject::connect(this->m_reply, &QNetworkReply::errorOccurred, this, &REST::errorOnRequest);

    emit sended();
    return request;
}
Q_INVOKABLE void REST::setAuthToken(QString token)
{
    QFile ft(data_path.absolutePath()+"/token.dat");
    ft.open(QIODevice::WriteOnly);
    QTextStream st(&ft);
    st << token;
    ft.close();
    authtoken = token;
}
Q_INVOKABLE void REST::setDeviceHash(QString token)
{
    QFile ft(data_path.absolutePath()+"/device.dat");
    ft.open(QIODevice::WriteOnly);
    QTextStream st(&ft);
    st << token;
    ft.close();
    deviceHash = token;
}
Q_INVOKABLE bool REST::hasAuthToken() {
    return authtoken != nullptr;
}

Q_INVOKABLE void REST::post()
{
    this->request('p');
}

Q_INVOKABLE void REST::put()
{
    this->request('u');
}

Q_INVOKABLE void REST::del()
{
    this->request('d');
}

Q_INVOKABLE void REST::get()
{
    this->request('g');
}

Q_INVOKABLE void REST::appendParam(QString param){
    this->params.append(param);
}
Q_INVOKABLE void REST::clearParams(){
    this->params.clear();
}

void REST::finishGetUrl()
{
    this->m_reply->deleteLater();
    QByteArray answer = this->m_reply->readAll();
    if(answer.length() > 1)
    {
        QJsonDocument document = { QJsonDocument::fromJson(answer) };
        QJsonObject jsonObject = document.object();
        emit this->ready(jsonObject);
    }
}
void REST::errorOnRequest()
{
    this->m_reply->deleteLater();
    QByteArray answer = this->m_reply->readAll();
    QJsonDocument document = { QJsonDocument::fromJson(answer) };
    QJsonObject jsonObject = document.object();
    QVariant status_code = this->m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(status_code.isNull())
        status_code = QVariant(0);
    emit  this->error(status_code.toInt(),jsonObject);
}
