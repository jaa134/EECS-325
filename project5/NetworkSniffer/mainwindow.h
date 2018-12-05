#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "arpsniffer.h"
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
    void drawClientsGraph();
    void drawClientsTrendGraph();

    QElapsedTimer duration;
    QTimer durationUpdateTimer;

    QList<QLabel *> historyValues;
    QThread arpSnifferThread;
    ArpSniffer *arpSniffer;

private slots:
    void onRecord();
    void onReport();
    void onArpSnifferUpdated();
    void onArpSnifferErrored(QString);
    void updateDuration();
    void finalizeReport();
};

#endif // MAINWINDOW_H
