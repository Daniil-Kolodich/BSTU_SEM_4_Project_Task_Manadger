#ifndef CLICKHOUSEHANDLER_H
#define CLICKHOUSEHANDLER_H

#include <QtNetwork/QNetworkReply>

class ClickhouseHandler : public QObject
{
    Q_OBJECT
public:
    ClickhouseHandler();
    virtual ~ClickhouseHandler();
    void InsertData(QString data);
    void CreateTable();
private slots:
    void _ReplyFinished();
private:
    QNetworkAccessManager *manager;
};

#endif // CLICKHOUSEHANDLER_H
