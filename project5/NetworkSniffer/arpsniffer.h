/*
 * Jacob Alspaw
 * jaa134@case.edu
 * 12/07/2018
 */

#ifndef ARPSNIFFER_H
#define ARPSNIFFER_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QFile>
#include <QList>
#include <QTimer>
#include <QThread>

class ArpSniffer : public QObject
{
    Q_OBJECT

    struct Client {
        QString ip;
        QString mac;
    };

    struct State {
        int numConnections;
        QDateTime timeStamp;
    };

    struct Report {
        int current;
        int max;
        int min;
        int total;
        int samplePoints;
        double average;
        int distinctIp;
        int distinctMac;
        int distinctClients;
    };

public:
    explicit ArpSniffer(QObject *parent = nullptr);
    QList<State> history;
    Report summary;
    bool isRunning;
    //stops the service
    void stop();

private:
    bool isStopRequested;
    QList<Client> connections;
    QList<QString> distinctIp;
    QList<QString> distinctMac;
    QList<Client> distinctClients;
    const QString outFolder = "arp-scan";
    QTimer *updateTimer;
    //makes a name of a timestamped text file
    QString makeFileName();
    //parse the results of the ststem call
    void parseSystemCall(QString);
    //determines if a string is a valid mac address
    bool isValidMacAddress(QString);
    //determines if a string is a valid Ip Address
    bool isValidIpAddress(QString);
    //saves the result of a system call as a simplified state
    void saveState();
    //updates the total results of collective system calls
    void updateReport();

signals:
    void updated();
    void stopped();
    void errored(QString);

private slots:
    //updates the service
    void update();
};

#endif // ARPSNIFFER_H
