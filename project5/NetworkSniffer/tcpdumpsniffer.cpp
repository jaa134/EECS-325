/*
 * Jacob Alspaw
 * jaa134@case.edu
 * 12/07/2018
 */

#include "tcpdumpsniffer.h"

TcpDumpSniffer::TcpDumpSniffer(QObject *parent) : QObject(parent) {
    isStopRequested = false;

    //reset our summary
    summary.maxPackets = 0;
    summary.currentPackets = 0;
    summary.totalTcpPackets = 0;
    summary.totalUdpPackets = 0;
    summary.totalOtherPackets = 0;
    summary.totalTcpLen = 0;
    summary.totalUdpLen = 0;
    summary.avgTcpLen = 0;
    summary.avgUdpLen = 0;

    //make a timer that will update the service repeatedly
    updateTimer = new QTimer(this);
    updateTimer->setInterval(500);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &TcpDumpSniffer::update);
    updateTimer->start();
    isRunning = true;
}

void TcpDumpSniffer::stop() {
    isStopRequested = true;
}

void TcpDumpSniffer::update() {
    //possibly stop the service
    if (isStopRequested) {
        isRunning = false;
        emit stopped();
        return;
    }

    //ensure our output folder exists
    QDir().mkpath(outFolder);
    QString fileName = makeFileName();

    //make the system command
    system(qPrintable("timeout " + QString::number(timeout) + " tcpdump ip -v -t -q -n -N > " + fileName));

    //open the file and parse it
    QFile f(fileName);
    if (f.open(QIODevice::ReadOnly)) {
        QStringList data;
        while(!f.atEnd()) {
            data.append(f.readLine() + f.readLine());
        }
        f.close();

        if (data.isEmpty()) {
            qDebug() << "Failed System Call";
            emit errored("Failed System Call");
            return;
        }

        parseSystemCall(data);
        emit updated();
    }
    else {
        qDebug() << "File IO";
        emit errored("File IO");
    }

    //start the timer that pauses thread for brief time before next update
    updateTimer->start();
}

QString TcpDumpSniffer::makeFileName() {
    //file name is a timestamped text file
    QString timeStamp = QDateTime::currentDateTime().toString("yymmdd-hhmmsszzz");
    return outFolder + "/" + timeStamp + ".txt";
}

void TcpDumpSniffer::parseSystemCall(QStringList data) {
    //remove previous system call data
    packets.clear();
    //append each packet
    foreach (auto rawPacketData, data) {
        //remove invalid chars
        QStringList scrubbedPacketData = rawPacketData
                .replace('\n', ' ')
                .replace('\t', ' ')
                .replace(',', ' ')
                .replace('(', ' ')
                .replace(')', ' ')
                .split(' ');
        scrubbedPacketData.removeAll("");
        if (isValidPacketInfo(scrubbedPacketData))
            packets.append(gatherPacketInfo(scrubbedPacketData));
    }

    saveState();
    updateReport();
}

bool TcpDumpSniffer::isValidPacketInfo(QStringList scrubbedPacketData) {
    //valid packets have an ip address, protocol, and a length
    return scrubbedPacketData.length() != 0
            && isValidIpAddress(scrubbedPacketData.at(0))
            && scrubbedPacketData.contains("proto")
            && scrubbedPacketData.contains("length");
}

//find the positions of important information and use it
TcpDumpSniffer::Packet TcpDumpSniffer::gatherPacketInfo(QStringList scrubbedPacket) {
    Packet p;

    int ipPos = scrubbedPacket.indexOf(">") - 1;
    QString ip = scrubbedPacket.at(ipPos);
    bool hasSubnet = ip.count('.') == 4;
    p.ip = hasSubnet ? ip.left(ip.lastIndexOf(QChar('.'))) : ip;

    int protocolPos = scrubbedPacket.indexOf("proto") + 1;
    p.protocol = scrubbedPacket.at(protocolPos).toUpper();

    int lengthPos = scrubbedPacket.indexOf("length") + 1;
    p.length = scrubbedPacket.at(lengthPos).toInt();

    return p;
}

bool TcpDumpSniffer::isValidIpAddress(QString ip) {
    QRegExp regExMacAddress("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
    return regExMacAddress.indexIn(ip) || regExMacAddress.exactMatch(ip);
}

//update statistics for this run of the system command
void TcpDumpSniffer::saveState() {
    State s;
    s.tcpPackets = 0;
    s.udpPackets = 0;
    s.otherPackets = 0;

    foreach (auto p, packets) {
        if (p.protocol == "TCP")
            s.tcpPackets++;
        else if (p.protocol == "UDP")
            s.udpPackets++;
        else
            s.otherPackets++;

        if (!packetCountByIp.contains(p.protocol))
            packetCountByIp.insert(p.ip, 1);
        else
            packetCountByIp[p.ip]++;
    }
    history.append(s);
}

//update the tcpdump sniffer report values
void TcpDumpSniffer::updateReport() {
    State s = history.last();
    if ((s.tcpPackets + s.udpPackets + s.otherPackets) > summary.maxPackets)
        summary.maxPackets = s.tcpPackets + s.udpPackets + s.otherPackets;
    summary.currentPackets = s.tcpPackets + s.udpPackets + s.otherPackets;
    summary.totalTcpPackets += s.tcpPackets;
    summary.totalUdpPackets += s.udpPackets;
    summary.totalOtherPackets += s.otherPackets;

    foreach (auto p, packets) {
        if (p.protocol == "TCP")
            summary.totalTcpLen += p.length;
        else if (p.protocol == "UDP")
            summary.totalUdpLen += p.length;
    }
    summary.avgTcpLen = summary.totalTcpLen / (double)summary.totalTcpPackets;
    summary.avgUdpLen = summary.totalUdpLen / (double)summary.totalUdpPackets;
}

QString TcpDumpSniffer::busiestIp() {
    QString ip;
    //retrive all keys
    QList<QString> keys = packetCountByIp.keys();
    int max = 0;
    //look at the value of each key and find the max
    foreach (auto k, keys) {
        if (packetCountByIp[k] > max)
            ip = k;
    }
    return ip;
}
