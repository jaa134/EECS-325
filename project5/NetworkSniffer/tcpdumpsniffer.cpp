#include "tcpdumpsniffer.h"

TcpDumpSniffer::TcpDumpSniffer(QObject *parent) : QObject(parent) {
    isStopRequested = false;

    summary.maxPackets = 0;
    summary.currentPackets = 0;
    summary.totalTcpPackets = 0;
    summary.totalUdpPackets = 0;
    summary.totalOtherPackets = 0;
    summary.totalTcpLen = 0;
    summary.totalUdpLen = 0;
    summary.avgTcpLen = 0;
    summary.avgUdpLen = 0;

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
    if (isStopRequested) {
        isRunning = false;
        emit stopped();
        return;
    }

    QDir().mkpath(outFolder);
    QString fileName = makeFileName();

    system(qPrintable("timeout " + QString::number(timeout) + " tcpdump ip -v -t -q -n -N > " + fileName));

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

    updateTimer->start();
}

QString TcpDumpSniffer::makeFileName() {
    QString timeStamp = QDateTime::currentDateTime().toString("yymmdd-hhmmsszzz");
    return outFolder + "/" + timeStamp + ".txt";
}

void TcpDumpSniffer::parseSystemCall(QStringList data) {
    packets.clear();

    foreach (auto rawPacketData, data) {
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
    return scrubbedPacketData.length() != 0
            && isValidIpAddress(scrubbedPacketData.at(0))
            && scrubbedPacketData.contains("proto")
            && scrubbedPacketData.contains("length");
}

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
    QList<QString> keys = packetCountByIp.keys();
    int max = 0;
    foreach (auto k, keys) {
        if (packetCountByIp[k] > max)
            ip = k;
    }
    return ip;
}
