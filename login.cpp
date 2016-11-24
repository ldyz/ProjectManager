#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_LoginButton_clicked()
{
    //   Qt使用post请求：
    QNetworkAccessManager *network_manager = new QNetworkAccessManager();
    QNetworkRequest network_request;
    QByteArray postData;

    //设置发送的数据
            postData.append("seller_name=");
            postData.append(ui->username->text());
            postData.append("&password=");
            postData.append(ui->password->text());



    //设置头信息
    network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    network_request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());

    QString uri=QString("http://www.weidik.com/block/seller_login");

    //设置url
    network_request.setUrl(QUrl(uri));

    qDebug()<<uri;

    connect(network_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    //发送请求
    network_manager->post(network_request, postData);

}
void Login::replyFinished(QNetworkReply *reply){

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
 QJsonParseError json_error;
QByteArray bytes;

   //无错误返回
    if(status_code.toString()=="200")
    {
        bytes = reply->readAll();  //获取字节

        QJsonDocument json_document = QJsonDocument::fromJson(bytes, &json_error);
    //处理ｊｓｏｎ

                QJsonObject obj= json_document.object();
  if(obj.take("seller_name")==this->ui->username->text())


      qDebug()<<obj.value("id").toString();
      Data::seller_id = obj.value("id").toString();

      accept();
}else{

      this->ui->username->clear();
      this->ui->password->clear();
      this->ui->username->setFocus();

}
   
    this->ui->msg->setText("到服务器请求出错，请检查您的网络！");


    //收到响应，因此需要处理
    delete reply;


}
