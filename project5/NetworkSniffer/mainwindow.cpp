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
    arpSniffer->moveToThread(&arpSnifferThread);
    arpSnifferThread.start();

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
        connect(arpSniffer, &ArpSniffer::stopped, this, &MainWindow::finalizeReport);
        ui->transitionLabel->setText("Stopping service...");
        ui->progressBar->setValue(0);
        arpSniffer->stop();
    }
}

void MainWindow::onArpSnifferUpdated() {
    drawClientsGraph();
}

void MainWindow::drawClientsGraph() {
    ui->clientsMaxValue->setText(QString::number(arpSniffer->summary.max));

    QLabel *newLabel = new QLabel(ui->clientsGraph);
    newLabel->setText(QString::number(arpSniffer->summary.current));
    newLabel->setStyleSheet("background-color: #777; color: rgba(0,0,0,0)");
    newLabel->setGeometry(0, 0, ui->clientsGraph->width(), ui->clientsGraph->height());
    historyValues.append(newLabel);

    int numSlots = 25;
    if (historyValues.length() > numSlots) {
        QLabel *removedLabel = historyValues.takeFirst();
        removedLabel->hide();
        delete removedLabel;
    }

    for (int i = 0; i < historyValues.length(); i++) {
        QLabel *label = historyValues.at(i);
        int w = ui->clientsGraph->width() / numSlots;
        int h = label->text().toInt() / (double)arpSniffer->summary.max * ui->clientsGraph->height() * 0.9;
        int x = ui->clientsGraph->width() - (w * (historyValues.length() - i));
        int y = ui->clientsGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}

void MainWindow::onArpSnifferErrored(QString error) {
    ui->stackedWidget->setCurrentIndex(ERROR);
    ui->textBrowser->setText("Arp Sniffer module error!\n" + error + "...");
}

void MainWindow::updateDuration() {
    qint64 secs = duration.elapsed() / 1000;
    ui->durationValue->setText(QString::number(secs) + "s");
}

void MainWindow::finalizeReport() {
    ui->transitionLabel->setText("Finalizing Report...");

    ui->progressBar->setValue(10);
    ui->clientMinValue->setText(QString::number(arpSniffer->summary.min));
    ui->clientMaxValue->setText(QString::number(arpSniffer->summary.max));
    ui->clientNumPointsValue->setText(QString::number(arpSniffer->summary.samplePoints));
    ui->clientAverageValue->setText(QString::number(arpSniffer->summary.average, 'f', 2));
    ui->clientDistinctIpsValue->setText(QString::number(arpSniffer->summary.distinctIp));
    ui->clientDistinctMacsValue->setText(QString::number(arpSniffer->summary.distinctMac));
    ui->clientDistinctClientsValue->setText(QString::number(arpSniffer->summary.distinctClients));

    ui->progressBar->setValue(20);
    drawClientsTrendGraph();

    ui->progressBar->setValue(100);
    ui->transitionLabel->setText("Finished!");

    QApplication::processEvents();
    QThread::sleep(1);
    ui->stackedWidget->setCurrentIndex(REPORT);
}

void MainWindow::drawClientsTrendGraph() {
    for (int i = 0; i < arpSniffer->history.length(); i++) {
        QLabel *label = new QLabel(ui->clientsTrendGraph);
        label->setStyleSheet("background-color: #777;");

        int w = ceil(ui->clientsTrendGraph->width() / (double)arpSniffer->history.length());
        int h = arpSniffer->history.at(i).connections.length() / (double)arpSniffer->summary.max * ui->clientsTrendGraph->height() * 0.9;
        int x = ui->clientsTrendGraph->width() - (w * (arpSniffer->history.length() - i));
        int y = ui->clientsTrendGraph->height() - h;

        label->setGeometry(x, y, w, h);
        label->show();
    }
}
