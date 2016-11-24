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

     //初始化界面
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    popMenu=new QMenu(ui->tableWidget);

    sendaction = new QAction("开始配送",this);
    finishaction = new QAction("订单完成",this);

    connect(sendaction, SIGNAL(triggered()), this, SLOT(send_action()));
     connect(finishaction, SIGNAL(triggered()), this, SLOT(finish_action()));

      connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(getItem(QTableWidgetItem*)));

      connect(ui->tableWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_tableWidget_CustomContextMenu(QPoint)));
//设置一个定时器,定时更新数据
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
//确认订单配送完成

                qDebug()<<"更新订单配送状态";
                int i =ui->tableWidget->currentRow();


                QString orderNo=ui->tableWidget->item(i,0)->text();
                QString reqUri=QString("http://www.weidik.com/shop/send_delivery/order_no/")+orderNo+QString("/distribution_status/1");

                qDebug()<<reqUri;
               QNetworkAccessManager *network_manager = new QNetworkAccessManager();
               QNetworkRequest network_request;

               //设置头信息
               network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

               //设置url
               network_request.setUrl(QUrl(reqUri));

               //发送请求
               network_manager->get(network_request);
               QMessageBox::information(this,"提示","订单配送开始！");
               btnClick();
}

void MainWindow::finish_action(){
//确认订单配送完成
    qDebug()<<"更新订单状态";
    int i =ui->tableWidget->currentRow();


    QString orderNo=ui->tableWidget->item(i,0)->text();
    QString reqUri=QString("http://www.weidik.com/shop/send_delivery/order_no/")+orderNo+QString("/status/5");

    qDebug()<<reqUri;
   QNetworkAccessManager *network_manager = new QNetworkAccessManager();
   QNetworkRequest network_request;

   //设置头信息
   network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

   //设置url
   network_request.setUrl(QUrl(reqUri));

   //发送请求
   network_manager->get(network_request);
   QMessageBox::information(this,"提示","订单状态更新完成！");
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
  this->ui->statusBar->showMessage(tr("正在读取数据中..."),0);
 play_sound=false;
 //   Qt使用post请求：
 QNetworkAccessManager *network_manager = new QNetworkAccessManager();
 QNetworkRequest network_request;
 QByteArray post_data;
 //设置发送的数据
 post_data.append("user=wangliang");
 //设置头信息
 network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
 network_request.setHeader(QNetworkRequest::ContentLengthHeader, post_data.length());
 QString uri=QString("http://www.weidik.com/block/seller_orders/seller_id/")+Data::seller_id+QString("/date/")+QString(ui->calendar->selectedDate().toString("yyyyMMdd"));
 //设置url
 network_request.setUrl(QUrl(uri));
 qDebug()<<uri;
 connect(network_manager, SIGNAL(finished(QNetworkReply*)),
         this, SLOT(replyFinished(QNetworkReply*)));
 //发送请求
 network_manager->post(network_request, post_data);
}

void MainWindow::playSound(){

    if(play_sound){
        QSound::play("voice.wav");
    }
}

void MainWindow::replyFinished(QNetworkReply* reply){
    this->ui->statusBar->showMessage(tr("数据读取完毕！"),0);
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    double yye=0;
    //无错误返回
    if(status_code.toString()=="200")
    {
        bytes = reply->readAll();  //获取字节
        QJsonParseError json_error;

    json_document = QJsonDocument::fromJson(bytes, &json_error);
//处理ｊｓｏｎ
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
                    //获取各数据项目的值
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

                  //判断是否有新订单，先读取data.ini文件，然后对比date.ini文件中是否有订单号
                     QSettings *configIniRead = new QSettings(filename, QSettings::IniFormat);
                     //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
                     bool has_order_no = configIniRead->value(order_no).toBool();


                     if(!has_order_no){
                         //如果订单号不存在，则播放声音,并将订单号写入ini配置文件
                         QFile file(filename);
                         if(!file.exists()){
                             file.open(QIODevice::WriteOnly);
                             file.close();
                         }


                         QSettings *configIniWrite = new QSettings(filename, QSettings::IniFormat);
                           //向ini文件中写入内容,setValue函数的两个参数是键值对
                           //向ini文件的第一个节写入内容,ip节下的第一个参数
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
            //显示总营业额
            ui->label_yye->setText(QString::number(yye,'f',2));
        }
    }
    else
    {
		
      this->ui->statusBar->showMessage(reply->errorString(),0);
        //处理错误
    }
    //收到响应，因此需要处理
    delete reply;
}


void MainWindow::getItem(QTableWidgetItem* item){
int i=item->row();
//双击之后弹出订单详情展示，并可以打印！
if(json_document.isArray()){
    qDebug()<<"正在显示详情页";
        //创建一个弹窗
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
     QMessageBox::information(this,"提示","语音播报停止！");

}


