#include "details.h"
#include "ui_details.h"
#include <QMessageBox>
Details::Details(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Details)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(5);
    //��ʼ������
        QStringList  HStrList;
        HStrList.push_back(QString("����"));
        HStrList.push_back(QString("����"));
        HStrList.push_back(QString("�ͺ�"));
        HStrList.push_back(QString("����"));
        HStrList.push_back(QString("С��"));

         //�����б�ǩ
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
            //��ȡ��������Ŀ��ֵ
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

          //��ӡhtml
            html=QString("<html><center><big>9V�̳�СƱ</big></center>�������:")+orderNo+QString("<br>�����绰:")+mobile+QString("<br>�ͻ���ַ:")+orderAddress+QString(" <hr>");



          ui->tableWidget->setRowCount(items.count());
     html+=QString("<table text-algin='center'><tr><td>���</td>")+QString("<td>����</td>")+QString("<td>����</td> ")+QString("<td>����</td>")+QString("<td>С��</td></tr>");
        for(int j=0;j<=items.count();j++){

            if(items[j].isObject()){
                QJsonObject res=items[j].toObject();                
                //��ȡ��������Ŀ��ֵ

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


                    //��ӡhtml
                    html+=QString("<tr><td>")+QString::number(j+1,'f',0)+QString("</td><td>")+name+QString("</td><td>")+real_price+QString("</td><td>")+goods_nums+QString("</td><td>")+xiaoji+QString("</td></tr>");

            }
        }
        //��ӡ��β
        html+=QString("</table><hr>�ϼ�:       ")+real_amount+QString("Ԫ<br>����ʱ��:")+create_time+QString("<br>")+QString("���ʽ��")+pay_name+QString("<br>")+QString("����״̬��")+pay_status+QString("</html>");
}


}


void Details::printDetail(){
qDebug()<<"׼����ӡ"<<html;
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
       dialog.setWindowTitle(tr("��ӡСƱ"));
       if (dialog.exec() != QDialog::Accepted) {
           return;
       }
           qDebug()<<"��ʼ��ӡ";
           int pageCount=2;
           for (int i = 1; i <= pageCount; ++i) {
                    // ���ֻ��ӡһ�ݣ���ֻ��ӡ����

               qDebug()<<"��ӡ";
                        if (i % 2 != 0) {
                            doc.setHtml(html.arg(tr("�ͻ���")));
                        } else {
                            doc.setHtml(html.arg(tr("����")));
                        }

                    doc.print(&printer);
                    if (i <pageCount) {
                        printer.newPage();
                    }
                }
//����״̬��Ϣ


}
