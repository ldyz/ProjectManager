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
    //   Qtʹ��post����
    QNetworkAccessManager *network_manager = new QNetworkAccessManager();
    QNetworkRequest network_request;
    QByteArray postData;

    //���÷��͵�����
            postData.append("seller_name=");
            postData.append(ui->username->text());
            postData.append("&password=");
            postData.append(ui->password->text());



    //����ͷ��Ϣ
    network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    network_request.setHeader(QNetworkRequest::ContentLengthHeader, postData.length());

    QString uri=QString("http://www.weidik.com/block/seller_login");

    //����url
    network_request.setUrl(QUrl(uri));

    qDebug()<<uri;

    connect(network_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    //��������
    network_manager->post(network_request, postData);

}
void Login::replyFinished(QNetworkReply *reply){

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
 QJsonParseError json_error;
QByteArray bytes;

   //�޴��󷵻�
    if(status_code.toString()=="200")
    {
        bytes = reply->readAll();  //��ȡ�ֽ�

        QJsonDocument json_document = QJsonDocument::fromJson(bytes, &json_error);
    //��������

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
   
    this->ui->msg->setText("��������������������������磡");


    //�յ���Ӧ�������Ҫ����
    delete reply;


}
