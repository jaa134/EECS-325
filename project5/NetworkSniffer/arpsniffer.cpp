#include "arpsniffer.h"

ArpSniffer::ArpSniffer(QObject *parent) : QObject(parent) {
    isStopRequested = false;

    summary.current = 0;
    summary.max = INT_MIN;
    summary.min = INT_MAX;
    summary.total = 0;
    summary.samplePoints = 0;
    summary.average = 0;

    updateTimer = new QTimer(this);
    updateTimer->setInterval(2000);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &ArpSniffer::update);
    updateTimer->start();
    isRunning = true;
}

void ArpSniffer::stop() {
    isStopRequested = true;
}

void ArpSniffer::update() {
    if (isStopRequested) {
        isRunning = false;
        emit stopped();
        return;
    }

    QDir().mkpath(outFolder);
    QString fileName = makeFileName();

    system(qPrintable("arp-scan -l -g -q > " + fileName));

    QFile f(fileName);
    if (f.open(QIODevice::ReadOnly)) {
        QString data = f.readAll();
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

QString ArpSniffer::makeFileName() {
    QString timeStamp = QDateTime::currentDateTime().toString("yymmdd-hhmmsszzz");
    return outFolder + "/" + timeStamp + ".txt";
}

void ArpSniffer::parseSystemCall(QString data) {
    connections.clear();
    QStringList split = data.replace('\n', ' ').replace('\t', ' ').split(' ');
    for (int i = 0; i < split.length() - 1; i++) {
        if (isValidIpAddress(split.at(i)) && isValidMacAddress(split.at(i + 1))) {
            Client c;
            c.ip = split.at(i);
            c.mac = split.at(i + 1).toUpper();
            connections.append(c);
            i++;
        }
    }

    saveState();
    updateReport();
}

bool ArpSniffer::isValidIpAddress(QString ip) {
    QRegExp regExMacAddress("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
    return regExMacAddress.exactMatch(ip);
}

bool ArpSniffer::isValidMacAddress(QString mac) {
    QRegExp regExMacAddress("([0-9A-F]{2}[:-]){5}([0-9A-F]{2})");
    return regExMacAddress.exactMatch(mac.toUpper());
}

void ArpSniffer::saveState() {
    State s;
    s.numConnections = connections.length();
    s.timeStamp = QDateTime::currentDateTime();
    history.append(s);

    foreach (auto c, connections) {
        if (!distinctIp.contains(c.ip))
            distinctIp.append(c.ip);
        if (!distinctMac.contains(c.mac))
            distinctMac.append(c.mac);

        bool isDistinct = true;
        foreach (auto dc, distinctClients) {
            if (c.ip == dc.ip && c.mac == dc.mac) {
                isDistinct = false;
                break;
            }
        }
        if (isDistinct)
            distinctClients.append(c);
    }
}

void ArpSniffer::updateReport() {
    summary.current = connections.length();
    if (summary.current > summary.max)
        summary.max = summary.current;
    if (summary.current < summary.min)
        summary.min = summary.current;
    summary.total += connections.length();
    summary.samplePoints++;
    summary.average = summary.total / (double)summary.samplePoints;
    summary.distinctIp = distinctIp.length();
    summary.distinctMac = distinctMac.length();
    summary.distinctClients = distinctClients.length();
}
