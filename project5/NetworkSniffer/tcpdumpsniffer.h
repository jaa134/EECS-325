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
    QString busiestIp();
    void stop();

private:
    bool isStopRequested;
    QList<Packet> packets;
    QMap<QString, int> packetCountByIp;
    const QString outFolder = "tcpdump";
    QTimer *updateTimer;
    QString makeFileName();
    void parseSystemCall(QStringList);
    bool isValidIpAddress(QString);
    void saveState();
    void updateReport();
    bool isValidPacketInfo(QStringList);
    Packet gatherPacketInfo(QStringList);

signals:
    void updated();
    void stopped();
    void errored(QString);

private slots:
    void update();
};

#endif // TCPDUMPSNIFFER_H
