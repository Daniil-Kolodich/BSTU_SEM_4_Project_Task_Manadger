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
    QUrl tmp_url("http://localhost:8123/?query=insert into TaskManagerDB (* EXCEPT(date)) format TabSeparated");
    QNetworkRequest request(tmp_url);
    request.setHeader (QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post (request,post_data);
    connect(reply,SIGNAL(finished()),this,SLOT(_ReplyFinished()));
}

void ClickhouseHandler::CreateTable (){
    QByteArray post_data;
     QUrl tmp_url("http://localhost:8123/?query=create table if not exists TaskManagerDB (date DateTime64(3) DEFAULT now64(),name String,value Float32) Engine MergeTree() PARTITION by date order by date");
    QNetworkRequest request(tmp_url);
    request.setHeader (QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post (request,post_data);
    connect(reply,SIGNAL(finished()),this,SLOT(_ReplyFinished()));
}

void ClickhouseHandler::_ReplyFinished(){
    QNetworkReply *reply=qobject_cast<QNetworkReply *>(sender());
    if (reply->error() != QNetworkReply::NoError)
        exit(1);

    reply->deleteLater();
}
