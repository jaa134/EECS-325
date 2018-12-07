#include "mainwindow.h"
#include "ui_mainwindow.h"

#define HOME 0
#define TRANSITION 1
#define REPORT 2
#define ERROR 3

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->recordButton, &QPushButton::released, this, &MainWindow::onRecord);
    connect(ui->reportButton, &QPushButton::released, this, &MainWindow::onReport);

    ui->recordButton->setText("Record");
    ui->reportButton->setText("Report");

    if (hasNetworkConnection()) {
        ui->stackedWidget->setCurrentIndex(HOME);

        ui->recordButton->setEnabled(true);
        ui->reportButton->setEnabled(false);
    }
    else {
        ui->stackedWidget->setCurrentIndex(ERROR);
        ui->textBrowser->setText("Netowrk connectivity error!\nMake sure a connection has been established...");

        ui->recordButton->setEnabled(false);
        ui->reportButton->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    arpSnifferThread.quit();
    arpSnifferThread.wait();
    tcpDumpSnifferThread.quit();
    tcpDumpSnifferThread.wait();
}

bool MainWindow::hasNetworkConnection() {
    QNetworkAccessManager networkManager;
    QNetworkRequest req(QUrl("http://www.google.com"));
    QNetworkReply *reply = networkManager.get(req);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    bool result = reply->bytesAvailable();
    delete reply;
    return result;
}

void MainWindow::onRecord() {
    ui->recordButton->setEnabled(false);
    ui->recordButton->setText("Recording...");
    ui->reportButton->setEnabled(true);
    ui->reportButton->setText("Report");

    arpSniffer = new ArpSniffer();
    connect(arpSniffer, &ArpSniffer::updated, this, &MainWindow::onArpSnifferUpdated);
    connect(arpSniffer, &ArpSniffer::errored, this, &MainWindow::onArpSnifferErrored);
    arpSniffer->moveToThread(&arpSnifferThread);
    arpSnifferThread.start();

    tcpDumpSniffer = new TcpDumpSniffer();
    connect(tcpDumpSniffer, &TcpDumpSniffer::updated, this, &MainWindow::onTcpDumpSnifferUpdated);
    connect(tcpDumpSniffer, &TcpDumpSniffer::errored, this, &MainWindow::onTcpDumpSnifferErrored);
    tcpDumpSniffer->moveToThread(&tcpDumpSnifferThread);
    tcpDumpSnifferThread.start();

    int durationInterval = 100;
    durationUpdateTimer.setInterval(durationInterval);
    durationUpdateTimer.setSingleShot(false);
    connect(&durationUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDuration);
    durationUpdateTimer.start();

    duration.restart();
}

void MainWindow::onReport() {
    QString messageTitle = "Warning!";
    QString messageText = "Finalizing the report will stop the module indefinitely. Are you sure you want to continue?";
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, messageTitle, messageText, QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        ui->stackedWidget->setCurrentIndex(TRANSITION);
        connect(arpSniffer, &ArpSniffer::stopped, this, &MainWindow::onServicesStopped);
        connect(tcpDumpSniffer, &TcpDumpSniffer::stopped, this, &MainWindow::onServicesStopped);
        ui->transitionLabel->setText("Stopping services...");
        ui->progressBar->setValue(0);
        arpSniffer->stop();
        tcpDumpSniffer->stop();
    }
}

void MainWindow::onServicesStopped() {
    if (!arpSniffer->isRunning && !tcpDumpSniffer->isRunning)
        finalizeReport();
}

void MainWindow::onArpSnifferUpdated() {
    drawClientsGraph();
}

void MainWindow::drawClientsGraph() {
    double maxClients = arpSniffer->summary.max;
    if (maxClients < 100)
        ui->clientsMaxValue->setText(QString::number(maxClients, 'f', 1));
    else if (maxClients < 1000)
        ui->clientsMaxValue->setText(QString::number(maxClients, 'f', 0));
    else if (maxClients < 10000)
        ui->clientsMaxValue->setText(QString::number(maxClients / 1000.0, 'f', 1) + "k");
    else if (maxClients < 100000)
        ui->clientsMaxValue->setText(QString::number(maxClients / 1000.0, 'f', 0) + "k");

    QLabel *newLabel = new QLabel(ui->clientsGraph);
    newLabel->setText(QString::number(arpSniffer->summary.current));
    newLabel->setStyleSheet("background-color: #777; color: rgba(0,0,0,0)");
    newLabel->setGeometry(0, 0, ui->clientsGraph->width(), ui->clientsGraph->height());
    clientGraphValues.append(newLabel);

    int numSlots = 25;
    if (clientGraphValues.length() > numSlots) {
        QLabel *removedLabel = clientGraphValues.takeFirst();
        removedLabel->hide();
        delete removedLabel;
    }

    for (int i = 0; i < clientGraphValues.length(); i++) {
        QLabel *label = clientGraphValues.at(i);
        int w = ui->clientsGraph->width() / numSlots;
        int h = label->text().toInt() / (double)arpSniffer->summary.max * ui->clientsGraph->height() * 0.9;
        int x = ui->clientsGraph->width() - (w * (clientGraphValues.length() - i));
        int y = ui->clientsGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}

void MainWindow::onArpSnifferErrored(QString error) {
    ui->stackedWidget->setCurrentIndex(ERROR);
    ui->textBrowser->setText("Arp Sniffer module error!\n" + error + "...");
}

void MainWindow::onTcpDumpSnifferUpdated() {
    drawPacketsGraph();
}

void MainWindow::drawPacketsGraph() {
    double maxPacketsPerSecond = tcpDumpSniffer->summary.maxPackets / (double)TcpDumpSniffer::timeout;
    if (maxPacketsPerSecond < 100)
        ui->trafficMaxValue->setText(QString::number(maxPacketsPerSecond, 'f', 1));
    else if (maxPacketsPerSecond < 1000)
        ui->trafficMaxValue->setText(QString::number(maxPacketsPerSecond, 'f', 0));
    else if (maxPacketsPerSecond < 10000)
        ui->trafficMaxValue->setText(QString::number(maxPacketsPerSecond / 1000.0, 'f', 1) + "k");
    else if (maxPacketsPerSecond < 100000)
        ui->trafficMaxValue->setText(QString::number(maxPacketsPerSecond / 1000.0, 'f', 0) + "k");

    QLabel *newLabel = new QLabel(ui->trafficGraph);
    newLabel->setText(QString::number(tcpDumpSniffer->summary.currentPackets));
    newLabel->setStyleSheet("background-color: #777; color: rgba(0,0,0,0)");
    newLabel->setGeometry(0, 0, ui->trafficGraph->width(), ui->trafficGraph->height());
    trafficGraphValues.append(newLabel);

    int numSlots = 25;
    if (trafficGraphValues.length() > numSlots) {
        QLabel *removedLabel = trafficGraphValues.takeFirst();
        removedLabel->hide();
        delete removedLabel;
    }

    for (int i = 0; i < trafficGraphValues.length(); i++) {
        QLabel *label = trafficGraphValues.at(i);
        int w = ui->trafficGraph->width() / numSlots;
        int h = label->text().toInt() / (double)tcpDumpSniffer->summary.maxPackets * ui->trafficGraph->height() * 0.9;
        int x = ui->trafficGraph->width() - (w * (trafficGraphValues.length() - i));
        int y = ui->trafficGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}

void MainWindow::onTcpDumpSnifferErrored(QString error) {
    ui->stackedWidget->setCurrentIndex(ERROR);
    ui->textBrowser->setText("TCP Dump Sniffer module error!\n" + error + "...");
}

void MainWindow::updateDuration() {
    qint64 secs = duration.elapsed() / 1000;
    ui->durationValue->setText(QString::number(secs) + "s");
}

void MainWindow::finalizeReport() {
    ui->progressBar->setValue(20);
    ui->transitionLabel->setText("Finalizing Report...");

    ui->clientMinValue->setText(QString::number(arpSniffer->summary.min));
    ui->clientMaxValue->setText(QString::number(arpSniffer->summary.max));
    ui->clientNumPointsValue->setText(QString::number(arpSniffer->summary.samplePoints));
    ui->clientAverageValue->setText(QString::number(arpSniffer->summary.average, 'f', 2));
    ui->clientDistinctIpsValue->setText(QString::number(arpSniffer->summary.distinctIp));
    ui->clientDistinctMacsValue->setText(QString::number(arpSniffer->summary.distinctMac));
    ui->clientDistinctClientsValue->setText(QString::number(arpSniffer->summary.distinctClients));
    ui->progressBar->setValue(40);

    drawClientsTrendGraph();
    ui->progressBar->setValue(60);

    ui->trafficTcpPacketsValue->setText(QString::number(tcpDumpSniffer->summary.totalTcpPackets));
    ui->trafficUdpPacketsValue->setText(QString::number(tcpDumpSniffer->summary.totalUdpPackets));
    ui->trafficOtherPacketsValue->setText(QString::number(tcpDumpSniffer->summary.totalOtherPackets));
    ui->trafficTcpLenValue->setText(QString::number(tcpDumpSniffer->summary.avgTcpLen, 'f', 1));
    ui->trafficUdpLenValue->setText(QString::number(tcpDumpSniffer->summary.avgUdpLen, 'f', 1));
    ui->trafficIpValue->setText(tcpDumpSniffer->busiestIp());
    ui->progressBar->setValue(80);

    drawPacketsTrendGraph();
    ui->progressBar->setValue(100);
    ui->transitionLabel->setText("Finished!");

    QApplication::processEvents();
    QTimer *t = new QTimer();
    t->setInterval(1000);
    t->setSingleShot(true);
    connect(t, &QTimer::timeout, this, &MainWindow::navigateToReport);
    t->start();
}

void MainWindow::navigateToReport() {
    ui->stackedWidget->setCurrentIndex(REPORT);
}

void MainWindow::drawClientsTrendGraph() {
    int w = ceil(ui->clientsTrendGraph->width() / (double)arpSniffer->history.length());
    bool needsScrolling = (ui->clientsTrendGraph->width() / (double)arpSniffer->history.length()) < 1;
    if (needsScrolling) {
        ui->clientGraphScroll->setWidgetResizable(true);
        ui->clientGraphScrollContents->setFixedWidth(w * arpSniffer->history.length());
        ui->clientsTrendGraph->setFixedWidth(w * arpSniffer->history.length());
        ui->clientGraphScroll->repaint();
    }
    else {
        int scrollBarHeight = 8;
        QRect cg = ui->clientGraphScroll->geometry();
        cg.translate(0, scrollBarHeight);
        ui->clientGraphScroll->setGeometry(cg);
        QRect cl = ui->clientTrendLabel->geometry();
        cl.translate(0, scrollBarHeight);
        ui->clientTrendLabel->setGeometry(cl);
    }


    for (int i = 0; i < arpSniffer->history.length(); i++) {
        QLabel *label = new QLabel(ui->clientsTrendGraph);
        label->setStyleSheet("background-color: #777;");

        int h = arpSniffer->history.at(i).numConnections / (double)arpSniffer->summary.max * ui->clientsTrendGraph->height() * 0.9;
        int x = i * w;
        int y = ui->clientsTrendGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}

void MainWindow::drawPacketsTrendGraph() {
    int w = ceil(ui->trafficTrendGraph->width() / (double)tcpDumpSniffer->history.length());
    bool needsScrolling = (ui->trafficTrendGraph->width() / (double)tcpDumpSniffer->history.length()) < 1;
    if (needsScrolling) {
        ui->trafficGraphScroll->setWidgetResizable(true);
        ui->trafficGraphScrollContents->setFixedWidth(w * tcpDumpSniffer->history.length());
        ui->trafficTrendGraph->setFixedWidth(w * tcpDumpSniffer->history.length());
        ui->trafficGraphScroll->repaint();
    }
    else {
        int scrollBarHeight = 8;
        QRect tg = ui->trafficGraphScroll->geometry();
        tg.translate(0, scrollBarHeight);
        ui->trafficGraphScroll->setGeometry(tg);
        QRect tl = ui->trafficTrendLabel->geometry();
        tl.translate(0, scrollBarHeight);
        ui->trafficTrendLabel->setGeometry(tl);
    }

    for (int i = 0; i < tcpDumpSniffer->history.length(); i++) {
        QLabel *label = new QLabel(ui->trafficTrendGraph);
        label->setStyleSheet("background-color: #777;");

        int packetsAtI = tcpDumpSniffer->history.at(i).tcpPackets + tcpDumpSniffer->history.at(i).udpPackets + tcpDumpSniffer->history.at(i).otherPackets;

        int h = packetsAtI / (double)tcpDumpSniffer->summary.maxPackets * ui->trafficTrendGraph->height() * 0.9;
        int x = i * w;
        int y = ui->trafficTrendGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}
