#ifndef REST_H
#define REST_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QDir>
#include <QJsonObject>
#include <QJSValue>

class REST : public QObject
{
    Q_OBJECT
public:
    explicit REST(QObject *parent = nullptr);

    Q_PROPERTY(QVariant data MEMBER data NOTIFY dataChanged);
    Q_PROPERTY(QString endpoint MEMBER endpoint NOTIFY endpointChanged);
    Q_PROPERTY(QStringList params MEMBER params NOTIFY paramsChanged);
    Q_PROPERTY(bool authenticate MEMBER authenticate NOTIFY authenticateChanged);

    Q_INVOKABLE void post();
    Q_INVOKABLE void put();
    Q_INVOKABLE void del();
    Q_INVOKABLE void get();
    Q_INVOKABLE void appendParam(QString);
    Q_INVOKABLE void clearParams();
    Q_INVOKABLE void setAuthToken(QString);
    Q_INVOKABLE static bool hasAuthToken();

private:
    QVariant data; //data to send
    QVariant validation; //validation rules for data
    QString endpoint = ""; //endpoint :)
    QStringList params; // url params
    QVariant headers; //request headers
    bool authenticate = true; // if true authenticate token will automaticly add to headers

    
    static QString API_URL; //base url, endpoint will append to this
    static QString authtoken; // authentication token
    static QDir data_path; //path to store authentication token

    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;

    QNetworkRequest* request(char verb);

Q_SIGNALS:
    void AccessibleChanged();

    void error(int code, QJsonObject message);
    void ready(QJsonObject resualt);

    void aboutSend();
    void sended();

    void dataChanged();
    void endpointChanged();
    void paramsChanged();
    void authenticateChanged();

public Q_SLOTS:

private Q_SLOTS:
    void finishGetUrl();
    void errorOnRequest();

};

#endif // REST_H
