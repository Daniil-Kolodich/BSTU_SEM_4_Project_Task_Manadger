#include "clickhousehandler.h"

ClickhouseHandler::ClickhouseHandler()
{
    manager = new QNetworkAccessManager();
    CreateTable ();
}


ClickhouseHandler::~ClickhouseHandler(){
    manager->deleteLater ();
}

void ClickhouseHandler::InsertData (QString data){
    QByteArray post_data;
    post_data.append (data);
    QUrl tmp_url("http://localhost:8123/?query=insert into kursach_test2 (* EXCEPT(date)) format TabSeparated");
    QNetworkRequest request(tmp_url);
    request.setHeader (QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post (request,post_data);
    connect(reply,SIGNAL(finished()),this,SLOT(_ReplyFinished()));
}

void ClickhouseHandler::CreateTable (){
    QByteArray post_data;
    QUrl tmp_url("http://localhost:8123/?query=create table if not exists kursach_test2 (date DateTime64(3,'Europe/Moscow') DEFAULT now64(),name String,value Float32) Engine = Memory");
    QNetworkRequest request(tmp_url);
    request.setHeader (QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post (request,post_data);
    connect(reply,SIGNAL(finished()),this,SLOT(_ReplyFinished()));
}

void ClickhouseHandler::_ReplyFinished(){
    QNetworkReply *reply=qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
    {
        exit(1);
    }
    reply->deleteLater();
}
