#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include <QHostAddress>
#include <QLabel>
#include <QNetworkInterface>
#include <QTimer>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udp = new QUdpSocket();

//    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
//        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
//             if (address.toString().startsWith("169.254.")){
//                 qDebug() << address.toString();
//                 //udp->bind(address,1488);
//             }
//        }
//    }
    udp->bind(QHostAddress("169.254.0.55"),8008);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendData()));
    timer->start(50);

    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.isEmpty()) {
        qDebug() << "No controller";
        return;
    }

    m_gamepad = new QGamepad(*gamepads.begin(), this);
    connect(m_gamepad, &QGamepad::axisRightYChanged, this, [&](double value){//turn
        dir[1] = format(value);
        ui->lr->setText(QString::number(dir[1]));
    });
    connect(m_gamepad, &QGamepad::axisLeftYChanged, this, [&](double value){//rsdv
        dir[2] = format(value);
        ui->ud->setText(QString::number(dir[2]));
    });
    connect(m_gamepad, &QGamepad::axisLeftXChanged, this, [&](double value){//fwdbwd
        dir[0] = format(value);
        ui->fb->setText(QString::number(dir[0]));
    });

    connect(m_gamepad, &QGamepad::buttonGuideChanged, this, [&](bool pressed){//disarm
        if (pressed){
            DA = !DA;
            if (DA){
                ui->active->setText("ja");
            } else {
                ui->active->setText("nej");
            }
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}



uint8_t MainWindow::format(double x){
    uint8_t res = (((x + 1)*128) > 255) ? 255 : ((x + 1)*128);
    if (res >= 128 - DZ && res <= 128 + DZ){
        res = 128;
    }
    return res;
}

void MainWindow::sendData(){
    QHostAddress receiver;
    /* Set IP address of ROV#3 - same for all */
    receiver.setAddress("169.254.112.186");
    msg[0]=dir[0];
    msg[1]=dir[1];
    msg[2]=dir[2];
    msg[3]=DA;
    msg[4]=msg[0]+msg[1]+msg[2]+msg[3];
    //qDebug() << msg;

    udp->writeDatagram(msg ,receiver,8008);
}

void MainWindow::on_camButton_clicked()
{
    process.start("gst-launch-1.0 -v udpsrc port=5000 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264\" ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink sync=f");
}
