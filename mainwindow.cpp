#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "details.h"
#include <QMessageBox>
#include <QMenu>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //��ʼ������
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    popMenu=new QMenu(ui->tableWidget);

    sendaction = new QAction("��ʼ����",this);
    finishaction = new QAction("�������",this);

    connect(sendaction, SIGNAL(triggered()), this, SLOT(send_action()));
     connect(finishaction, SIGNAL(triggered()), this, SLOT(finish_action()));

      connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));

      connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_tableWidget_CustomContextMenu(QPoint)));
//����һ����ʱ��,��ʱ��������
      timer1= new QTimer(this);
      connect(timer1,SIGNAL(timeout()),this,SLOT(btnClick()));
      timer1->start(60*1000);
      timer2= new QTimer(this);
      connect(timer2,SIGNAL(timeout()),this,SLOT(playSound()));
      timer2->start(5000);

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::send_action(){
//ȷ�϶����������

                qDebug()<<"���¶�������״̬";
                int i =ui->tableWidget->currentRow();


                QString orderNo=ui->tableWidget->item(i,0)->text();
                QString reqUri=QString("http://www.weidik.com/shop/send_delivery/order_no/")+orderNo+QString("/distribution_status/1");

                qDebug()<<reqUri;
               QNetworkAccessManager *network_manager = new QNetworkAccessManager();
               QNetworkRequest network_request;

               //����ͷ��Ϣ
               network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

               //����url
               network_request.setUrl(QUrl(reqUri));

               //��������
               network_manager->get(network_request);
               QMessageBox::information(this,"��ʾ","�������Ϳ�ʼ��");
               btnClick();
}

void MainWindow::finish_action(){
//ȷ�϶����������
    qDebug()<<"���¶���״̬";
    int i =ui->tableWidget->currentRow();


    QString orderNo=ui->tableWidget->item(i,0)->text();
    QString reqUri=QString("http://www.weidik.com/shop/send_delivery/order_no/")+orderNo+QString("/status/5");

    qDebug()<<reqUri;
   QNetworkAccessManager *network_manager = new QNetworkAccessManager();
   QNetworkRequest network_request;

   //����ͷ��Ϣ
   network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

   //����url
   network_request.setUrl(QUrl(reqUri));

   //��������
   network_manager->get(network_request);
   QMessageBox::information(this,"��ʾ","����״̬������ɣ�");
   btnClick();

}
void MainWindow::on_tableWidget_CustomContextMenu(QPoint pos){

    popMenu->addAction(sendaction);
    popMenu->addAction(finishaction);
    popMenu->exec(QCursor::pos());
    ui->tableWidget->addAction(sendaction);
    ui->tableWidget->addAction(finishaction);

}

void MainWindow::btnClick(){
  this->ui->statusBar->show();
  this->ui->statusBar->showMessage(tr("���ڶ�ȡ������..."),0);
 play_sound=false;
 //   Qtʹ��post����
 QNetworkAccessManager *network_manager = new QNetworkAccessManager();
 QNetworkRequest network_request;
 QByteArray post_data;
 //���÷��͵�����
 post_data.append("user=wangliang");
 //����ͷ��Ϣ
 network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
 network_request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
 QString uri=QString("http://www.weidik.com/block/seller_orders/seller_id/")+Data::seller_id+QString("/date/")+QString(ui->calendar->selectedDate().toString("yyyyMMdd"));
 //����url
 network_request.setUrl(QUrl(uri));
 qDebug()<<uri;
 connect(network_manager, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(replyFinished(QNetworkReply*)));
 //��������
 network_manager->post(network_request, post_data);
}

void MainWindow::playSound(){

    if(play_sound){
        QSound::play("voice.wav");
    }
}

void MainWindow::replyFinished(QNetworkReply* reply){
    this->ui->statusBar->showMessage(tr("���ݶ�ȡ��ϣ�"),0);
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    double yye=0;
    //�޴��󷵻�
    if(status_code.toString()=="200")
    {
        bytes = reply->readAll();  //��ȡ�ֽ�
        QJsonParseError json_error;

    json_document = QJsonDocument::fromJson(bytes, &json_error);
//��������
        if(json_document.isObject()){
            qDebug()<<"is object";
        }
        if(json_document.isArray()){
            qDebug()<<"it is Array";
            QJsonArray result = json_document.array();
            int count = result.count();
            ui->tableWidget->setRowCount(count);
            for(int i=0;i<count;i++){

                if(result[i].isObject()){


                  QJsonObject ret=result[i].toObject();
                    //��ȡ��������Ŀ��ֵ
                  QString order_no=ret.take("order_no").toString();
                  QString accept_name=ret.take("accept_name").toString();
                  QString mobile=ret.take("mobile").toString();
                  QString dis_status=ret.take("dis_status").toString();
                  QString pay_status=ret.take("pay_status").toString();
                  QString real_amount=ret.take("real_amount").toString();
                  QString create_time=ret.take("cteate_time").toString();
                  QString pay_name =ret.take("payment_name").toString();
                  QString status=ret.take("status").toString();
                  yye=yye+real_amount.toDouble();



                 QString filename= create_time.left(10)+QString(".ini");

                  //�ж��Ƿ����¶������ȶ�ȡdata.ini�ļ���Ȼ��Ա�date.ini�ļ����Ƿ��ж�����
                     QSettings *configIniRead = new QSettings(filename, QSettings::IniFormat);
                     //����ȡ����ini�ļ�������QString�У���ȡֵ��Ȼ��ͨ��toString()����ת����QString����
                     bool has_order_no = configIniRead->value(order_no).toBool();


                     if(!has_order_no){
                         //��������Ų����ڣ��򲥷�����,����������д��ini�����ļ�
                         QFile file(filename);
                         if(!file.exists()){
                             file.open(QIODevice::WriteOnly);
                             file.close();
                         }


                         QSettings *configIniWrite = new QSettings(filename, QSettings::IniFormat);
                           //��ini�ļ���д������,setValue���������������Ǽ�ֵ��
                           //��ini�ļ��ĵ�һ����д������,ip���µĵ�һ������
                           configIniWrite->setValue(order_no, true);

                           play_sound=true;


                           delete configIniWrite;
                     }
                    delete configIniRead;


                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(order_no));
                    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(accept_name));
                    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(mobile));
                    ui->tableWidget->setItem(i, 3, new QTableWidgetItem(pay_name));
                    ui->tableWidget->setItem(i, 5, new QTableWidgetItem(dis_status));
                    ui->tableWidget->setItem(i, 4, new QTableWidgetItem(pay_status));
                    ui->tableWidget->setItem(i, 6, new QTableWidgetItem(real_amount));
                    ui->tableWidget->setItem(i, 7, new QTableWidgetItem(create_time));
                     ui->tableWidget->setItem(i, 8, new QTableWidgetItem(status));
//                  ui->tableWidget->setCellWidget(i,6,printButton);
                }
            }
            //��ʾ��Ӫҵ��
            ui->label_yye->setText(QString::number(yye,'f',2));
        }
    }
    else
    {
		
      this->ui->statusBar->showMessage(reply->errorString(),0);
        //�������
    }
    //�յ���Ӧ�������Ҫ����
    delete reply;
}


void MainWindow::getItem(QTableWidgetItem* item){
int i=item->row();
//˫��֮�󵯳���������չʾ�������Դ�ӡ��
if(json_document.isArray()){
    qDebug()<<"������ʾ����ҳ";
        //����һ������
           Details *d=new Details;
            d->move((QApplication::desktop()->width() - d->width())/2,(QApplication::desktop()->height() - d->height())/2);
           d->setWindowModality(Qt::ApplicationModal);
            d->init(i,bytes);
            d->show();

        }
    }



void MainWindow::on_calendar_selectionChanged()
{
    this->btnClick();
}


void MainWindow::on_pushButton_clicked()
{
    play_sound=false;
     QMessageBox::information(this,"��ʾ","��������ֹͣ��");

}


