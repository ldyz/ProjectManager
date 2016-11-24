#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QDebug>
#include <QtScript/QScriptEngine>
#include<QtNetwork>
#include<QComboBox>
#include<QTableWidgetItem>
#include <QDesktopWidget>
#include <QSound>
#include <data.h>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     QJsonDocument json_document;
     QByteArray bytes;


public slots:
    void btnClick();
    void replyFinished(QNetworkReply*); 
    void getItem(QTableWidgetItem* item);

    void playSound();
private slots:
    void on_calendar_selectionChanged();

    void on_pushButton_clicked();

    void on_tableWidget_CustomContextMenu(QPoint pos);

    void send_action();

    void finish_action();
private:
    Ui::MainWindow *ui;
    QMenu *popMenu;
    QAction *sendaction;
    QAction *finishaction;
protected:
    bool play_sound;
    QTimer *timer1;
    QTimer *timer2;

};


#endif // MAINWINDOW_H
