#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "ipnetlib/src/ipnetlib.hpp"
#include <array>
#include <cmath>
#include <QTimer>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icons/out.ico"));

    QMenu *menu = new QMenu;
    QAction *action = new QAction("PackMan");
    menu->addAction(action);

    QAction *quit = new QAction("Quit");
    menu->addAction(quit);

    QAction *show = new QAction("Show");
    menu->addAction(show);

    connect(show, &QAction::triggered, this, &QMainWindow::show);
    connect(quit, &QAction::triggered, this, &QApplication::quit);

    trayIcon->setContextMenu(menu);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::updateText));
    timer->start(500);
}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::to_gb(int bytes) {
    return bytes/pow(10, 9);

}

float MainWindow::to_mb(int bytes) {
    return bytes/pow(10,6);

}

QString MainWindow::round_bytes(float num) {
    return QString::number(num, 'f', 2);
}


void MainWindow::updateText() {
    std::array<unsigned int, 2> byte_pair = bytes_downloaded();
    unsigned int tx = byte_pair[0];
    unsigned int rx = byte_pair[1];

    QString up_gb = round_bytes(to_gb(tx));
    QString up_mb = round_bytes(to_mb(tx));
    QString down_gb = round_bytes(to_gb(rx));
    QString down_mb = round_bytes(to_mb(rx));

    ui->val_gb->setText(down_gb.append("GB"));
    ui->val_gb_up->setText(up_gb.append(("GB")));
    ui->val_mb->setText(down_mb.append("MB"));
    ui->val_mb_up->setText(up_mb.append("MB"));

    this->trayIcon->setToolTip(QString("Download: %1 %2\nUpload: %3 %4").arg(down_gb, down_mb, up_gb, up_mb));

}

void MainWindow::on_hide_action_triggered()
{
    trayIcon->show();
    this->hide();

}





