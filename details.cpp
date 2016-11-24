#include "details.h"
#include "ui_details.h"
#include <QMessageBox>
Details::Details(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Details)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    //初始化界面
        QStringList  HStrList;
        HStrList.push_back(QString("名称"));
        HStrList.push_back(QString("数量"));
        HStrList.push_back(QString("型号"));
        HStrList.push_back(QString("单价"));
        HStrList.push_back(QString("小计"));

         //设置列标签
      ui->tableWidget->setHorizontalHeaderLabels(HStrList);
      connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(printDetail()));
}

Details::~Details()
{
    delete ui;
}

void Details::init(int i,QByteArray bytes){
   json_document = QJsonDocument::fromJson(bytes);
    QJsonArray result =  json_document.array();

    qDebug()<<result;
          if(result[i].isObject()){

          QJsonObject ret=result[i].toObject();
            //获取各数据项目的值
          orderNo=ret.take("order_no").toString();
          QString mobile=ret.take("mobile").toString();
          QString create_time=ret.take("cteate_time").toString();
          QString orderAddress=ret.take("address").toString();
          QString real_amount=ret.take("real_amount").toString();
          QString pay_name=ret.take("payment_name").toString();
          QString pay_status=ret.take("pay_status").toString();

          items=ret.take("items").toArray();
          qDebug()<<items.count();

          ui->labelNum->setText(orderNo);
          ui->labelSum->setText(real_amount);
          ui->lobelAddress->setText(orderAddress);
          ui->labelTel->setText(mobile);
          ui->labelTime->setText(create_time);

          //打印html
            html=QString("<html><center><big>9V商城小票</big></center>订单编号:")+orderNo+QString("<br>订货电话:")+mobile+QString("<br>送货地址:")+orderAddress+QString(" <hr>");



          ui->tableWidget->setRowCount(items.count());
     html+=QString("<table text-algin='center'><tr><td>序号</td>")+QString("<td>名称</td>")+QString("<td>单价</td> ")+QString("<td>数量</td>")+QString("<td>小计</td></tr>");
        for(int j=0;j<=items.count();j++){

            if(items[j].isObject()){
                QJsonObject res=items[j].toObject();                
                //获取各数据项目的值

              QString name=res.take("name").toString();
              QString goods_nums=res.take("goods_nums").toString();
              QString value=res.take("value").toString();
              QString real_price=res.take("real_price").toString();
              QString xiaoji=QString::number( real_price.toDouble()*goods_nums.toInt(),'f',2);


                 ui->tableWidget->setItem(j, 0, new QTableWidgetItem(name));
                  ui->tableWidget->setItem(j, 1, new QTableWidgetItem(goods_nums));
                   ui->tableWidget->setItem(j, 2, new QTableWidgetItem(value));
                    ui->tableWidget->setItem(j, 3, new QTableWidgetItem(real_price));
                    ui->tableWidget->setItem(j, 4, new QTableWidgetItem(xiaoji));


                    //打印html
                    html+=QString("<tr><td>")+QString::number(j+1,'f',0)+QString("</td><td>")+name+QString("</td><td>")+real_price+QString("</td><td>")+goods_nums+QString("</td><td>")+xiaoji+QString("</td></tr>");

            }
        }
        //打印结尾
        html+=QString("</table><hr>合计:       ")+real_amount+QString("元<br>订货时间:")+create_time+QString("<br>")+QString("付款方式：")+pay_name+QString("<br>")+QString("付款状态：")+pay_status+QString("</html>");
}


}


void Details::printDetail(){
qDebug()<<"准备打印"<<html;
QPrinter printer;
QTextDocument doc;

QFont font=doc.defaultFont();
font.setBold(false);
font.setPointSize(font.pointSize()+1);
doc.setDefaultFont(font);

QSizeF s= QSizeF(printer.logicalDpiX() * (58 / 25.4), printer.logicalDpiY() * 2);
doc.setPageSize(s);

printer.setOutputFormat(QPrinter::NativeFormat);



QPrintDialog dialog(&printer, this);
       dialog.setWindowTitle(tr("打印小票"));
       if (dialog.exec() != QDialog::Accepted) {
           return;
       }
           qDebug()<<"开始打印";
           int pageCount=2;
           for (int i = 1; i <= pageCount; ++i) {
                    // 如果只打印一份，即只打印店联

               qDebug()<<"打印";
                        if (i % 2 != 0) {
                            doc.setHtml(html.arg(tr("客户联")));
                        } else {
                            doc.setHtml(html.arg(tr("店联")));
                        }

                    doc.print(&printer);
                    if (i <pageCount) {
                        printer.newPage();
                    }
                }
//更新状态信息


}
