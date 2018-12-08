/*
 * Jacob Alspaw
 * jaa134@case.edu
 * 12/07/2018
 */

#ifndef TCPDUMPSNIFFER_H
#define TCPDUMPSNIFFER_H

#include <QDebug>
#include <QObject>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QFile>
#include <QList>
#include <QMap>
#include <QTimer>
#include <QThread>

class TcpDumpSniffer : public QObject
{
    Q_OBJECT

    struct Packet {
        QString protocol;
        int length;
        QString ip;
    };

    struct State {
        int tcpPackets;
        int udpPackets;
        int otherPackets;
    };

    struct Report {
        int maxPackets;
        int currentPackets;
        int totalTcpPackets;
        int totalUdpPackets;
        int totalOtherPackets;
        qint64 totalTcpLen;
        qint64 totalUdpLen;
        double avgTcpLen;
        double avgUdpLen;
    };

public:
    static const int timeout = 10;
    explicit TcpDumpSniffer(QObject *parent = nullptr);
    QList<State> history;
    Report summary;
    bool isRunning;
    //searches for the busiest ip
    QString busiestIp();
    //stops the service
    void stop();

private:
    bool isStopRequested;
    QList<Packet> packets;
    QMap<QString, int> packetCountByIp;
    const QString outFolder = "tcpdump";
    QTimer *updateTimer;
    //makes a name of a timestamped text file
    QString makeFileName();
    //parses the result of the system call
    void parseSystemCall(QStringList);
    bool isValidIpAddress(QString);
    //saves the result of a system call as a simplified state
    void saveState();
    //updates the total results of collective system calls
    void updateReport();
    //determines if a scrubbed packet is valid for analysis
    bool isValidPacketInfo(QStringList);
    //extracts packet info from a delineated list of strings
    Packet gatherPacketInfo(QStringList);

signals:
    void updated();
    void stopped();
    void errored(QString);

private slots:
    //updates the service
    void update();
};

#endif // TCPDUMPSNIFFER_H
