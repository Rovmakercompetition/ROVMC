#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QtGamepad/QGamepad>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void sendData();

private slots:

//    void readPendingDatagrams();

    void on_camButton_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket * udp;
    QGamepad *m_gamepad;
    QTimer *timer;
    QProcess process;

    unsigned char dir[3] = {0};//FB,LR,UD
    uint8_t DZ = 25; //deadzone
    uint8_t DA = 0; //disarm
    QByteArray msg;
    uint8_t format(double x);

};

#endif // MAINWINDOW_H
