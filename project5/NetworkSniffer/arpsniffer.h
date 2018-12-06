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
    void stop();

private:
    bool isStopRequested;
    QList<Client> connections;
    QList<QString> distinctIp;
    QList<QString> distinctMac;
    QList<Client> distinctClients;
    const QString outFolder = "arp-scan";
    QTimer *updateTimer;
    QString makeFileName();
    void parseSystemCall(QString);
    bool isValidMacAddress(QString);
    bool isValidIpAddress(QString);
    void saveState();
    void updateReport();

signals:
    void updated();
    void stopped();
    void errored(QString);

private slots:
    void update();
};

#endif // ARPSNIFFER_H
