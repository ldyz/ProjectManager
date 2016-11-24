#ifndef LOGIN_H
#define LOGIN_H
#include <QDialog>
#include<QtNetwork>
#include <qcryptographichash.h>
#include <QtScript/QScriptEngine>
#include <data.h>
namespace Ui {
class Login;
} 
class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_LoginButton_clicked();

     void replyFinished(QNetworkReply*);
private:
    Ui::Login *ui;
};

#endif // LOGIN_H
