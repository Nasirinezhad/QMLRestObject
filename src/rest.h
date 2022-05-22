#ifndef REST_H
#define REST_H

#include <QNetworkAccessManager>
#include <QJSValue>
#include <QObject>

class REST : public QObject
{
    Q_OBJECT
public:
    explicit REST(QObject *parent = nullptr);

    Q_PROPERTY(QVariant data MEMBER data NOTIFY dataChanged);
    Q_PROPERTY(QString endpoint MEMBER endpoint NOTIFY endpointChanged);
    Q_PROPERTY(bool authenticate MEMBER authenticate NOTIFY authenticateChanged);

    Q_INVOKABLE void post();
    Q_INVOKABLE void put();
    Q_INVOKABLE void del();
    Q_INVOKABLE void get();

private:
    QVariant data = {}; //data to send
    QString endpoint = ""; //endpoint :)
    bool authenticate = true; // if true authenticate token will automaticly add to headers

    QString API_URL = QStringLiteral("http://127.0.0.1:8000/api/"); //base url, endpoint will append to this
    QString authtoken = nullptr;

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;

    QNetworkRequest* request(char verb);

Q_SIGNALS:
    void AccessibleChanged();

    void error(int code, QVariant message);
    void ready(QVariant resualt);

    void progressReady(double ans);

    void dataChanged();
    void endpointChanged();
    void authenticateChanged();
    void SSLChanged();
    void cacheChanged();

public Q_SLOTS:

private Q_SLOTS:
    void finishGetUrl();
    void errorOnRequest();

};

#endif // REST_H
