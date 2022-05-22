#include "rest.h"
#include <QJSEngine>
#include <QNetworkReply>
#include <QTextStream>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

REST::REST(QObject *parent) : QObject(parent)
{

    this->m_manager = new QNetworkAccessManager();
    //QObject::connect(this->m_manager, SIGNAL(networkAccessibleChanged()), this, SLOT(accessible()));
}

QNetworkRequest* REST::request(char verb)
{
    QUrl url(this->API_URL+this->endpoint);
    QNetworkRequest *request = new QNetworkRequest(url);

    request->setRawHeader("Connection","close");
    request->setRawHeader("Accept","application/json");
    request->setRawHeader("Content-type","application/json; charset=UTF-8");
    if(this->authenticate && this->authtoken != nullptr)
        request->setRawHeader("Authorization", QByteArray::fromStdString(("Bearer " + this->authtoken).toStdString()));

    switch (verb) {
    case 'p':
        this->m_reply = this->m_manager->post(*request, data.toByteArray());
        break;
    case 'u':
        this->m_reply = this->m_manager->put(*request, data.toByteArray());
        break;
    case 'g':
        this->m_reply = this->m_manager->get(*request);
        break;
    case 'd':
        this->m_reply = this->m_manager->deleteResource(*request);
        break;
    }

    QObject::connect(this->m_reply, &QNetworkReply::finished, this, &REST::finishGetUrl);

    QObject::connect(this->m_reply, &QNetworkReply::errorOccurred, this, &REST::errorOnRequest);

    return request;
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

void REST::finishGetUrl()
{
    this->m_reply->deleteLater();
    QVariant answer = QString::fromUtf8(this->m_reply->readAll());

    if(answer.toString().length() > 1)
    {
        emit this->ready(answer);
    }
}
void REST::errorOnRequest()
{
    this->m_reply->deleteLater();
    QVariant answer = QString::fromUtf8(this->m_reply->readAll());
    QVariant status_code = this->m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(answer.toString().length() > 1)
    {
        emit  this->error(status_code.toInt(),answer);
    }
}
