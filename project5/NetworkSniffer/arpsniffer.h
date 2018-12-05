#ifndef ARPSNIFFER_H
#define ARPSNIFFER_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QFile>
#include <QList>
#include <QDateTime>
#include <QTimer>

class ArpSniffer : public QObject
{
    Q_OBJECT

    struct Client {
        QString ip;
        QString mac;
    };

    struct State {
        QList<Client> connections;
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
    ArpSniffer();
    void update();
    QList<State> history;
    Report summary;
    void stop();

private:
    bool isStopRequested;
    QList<QString> distinctIp;
    QList<QString> distinctMac;
    QList<Client> distinctClients;
    const QString outFolder = "arp";
    QString makeFileName();
    void parseSystemCall(QString);
    bool isValidMacAddress(QString);
    bool isValidIpAddress(QString);
    void saveState(QList<Client> connections);
    void updateReport();
    QTimer *updateTimer;

signals:
    void updated();
    void stopped();
    void errored(QString);
};

#endif // ARPSNIFFER_H
