#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "arpsniffer.h"
#include "tcpdumpsniffer.h"
#include "limits.h"
#include <QDebug>
#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QThread>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QElapsedTimer>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    bool hasNetworkConnection();
    void finalizeReport();
    void drawClientsGraph();
    void drawClientsTrendGraph();
    void drawPacketsGraph();
    void drawPacketsTrendGraph();

    QElapsedTimer duration;
    QTimer durationUpdateTimer;

    QList<QLabel *> clientGraphValues;
    QThread arpSnifferThread;
    ArpSniffer *arpSniffer;

    QList<QLabel *> trafficGraphValues;
    QThread tcpDumpSnifferThread;
    TcpDumpSniffer *tcpDumpSniffer;

private slots:
    void onRecord();
    void onReport();
    void onServicesStopped();
    void onArpSnifferUpdated();
    void onArpSnifferErrored(QString);
    void onTcpDumpSnifferUpdated();
    void onTcpDumpSnifferErrored(QString);
    void updateDuration();
    void navigateToReport();
};

#endif // MAINWINDOW_H
