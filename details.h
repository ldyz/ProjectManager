#ifndef DETAILS_H
#define DETAILS_H
#include <QWidget>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include<QtNetwork>
#include<QTableWidgetItem>
#include<QTextDocument>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QNetworkReply>
namespace Ui {
class Details;
}

class Details : public QWidget
{
    Q_OBJECT

public:
    explicit Details(QWidget *parent = 0);
    ~Details();

    QJsonDocument json_document;
    QJsonArray items;
    QString html;
    QString orderNo;
    void init(int i,QByteArray bytes);

public slots:
    void printDetail();   


private slots:


private:
    Ui::Details *ui;
};

#endif // DETAILS_H
