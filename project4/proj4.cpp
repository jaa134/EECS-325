/**
* Jacob Alspaw
* jaa134
* proj4.cpp
* 11-14-2018
* 
* Source file for proj4; Implements variables and 
* method used for processing a packet trace file.
* Program will run in one of four modes, as directed 
* on the command line: summary, length analysis,
* packet printing, traffic matrix.
**/

#include "proj4.hpp"

void Proj4::debug(string message) {
    if (useVerboseOutput)
        cout << message << endl;
}

[[noreturn]] void Proj4::errorExit(string message) {
    cerr << message << endl;
    exit(EXIT_FAILURE);
}

void Proj4::exec(int argc, char **argv) {
    extractArgs(argc, argv);

    if (mode == Proj4::Mode::SUMMARY)
        invokeSummaryMode();
    if (mode == Proj4::Mode::LENGTH)
        invokeLengthAnalysisMode();
    if (mode == Proj4::Mode::PRINTING)
        invokePacketPrintingMode();
    if (mode == Proj4::Mode::TRAFFIC)
        invokeTrafficMatrixMode();
}

void Proj4::extractArgs(int argc, char **argv) {
    bool tIsPresent = false;
    useVerboseOutput = false;
    int opt;

    //loop over each given arg and handle accordingly
    while ((opt = getopt(argc, argv, "t:slpmv")) != -1) {
        switch (opt) {
            case 't':
                tIsPresent = true;
                traceFile = optarg;
                break;
            case 's':
                errorIfDupMode();
                mode = Proj4::Mode::SUMMARY;
                break;
            case 'l':
                errorIfDupMode();
                mode = Proj4::Mode::LENGTH;
                break;
            case 'p':
                errorIfDupMode();
                mode = Proj4::Mode::PRINTING;
                break;
            case 'm':
                errorIfDupMode();
                mode = Proj4::Mode::TRAFFIC;
                break;
            case 'v':
                useVerboseOutput = true;
                break;
            default: /* '?' */
                errorExit(USAGE);
        }
    }

    if (!tIsPresent || mode == Proj4::Mode::UNSET) {
        cerr << "ERROR: Missing required flag" << endl;
        errorExit(USAGE);
    }
}

void Proj4::errorIfDupMode() {
    if (mode != Proj4::Mode::UNSET) {
        cerr << "ERROR: More than one mode detected" << endl;
        errorExit(USAGE);
    }
}

void Proj4::invokeSummaryMode() {
    debug("DEBUG: Starting summary mode...");
    int tfd = openTraceFile();

    double firstTime = 0.0;
    double lastTime = 0.0;
    int totalPackets = 0;
    int numIpPackets = 0;

    pkt_info pinfo;
    bool hasFoundFirstTime = false;
    while (nextPacket(tfd, &pinfo)) {
        if (!hasFoundFirstTime) {
            firstTime = pinfo.now;
            hasFoundFirstTime = true;
        }
        lastTime =  pinfo.now;
        totalPackets++;
        if (pinfo.ethh != nullptr && ntohs(pinfo.ethh->ether_type) == ETHERTYPE_IP)
            numIpPackets++;
    }
    close (tfd);

    printInfo_SummaryMode(firstTime, lastTime, totalPackets, numIpPackets);
}

void Proj4::printInfo_SummaryMode(double firstTime, double lastTime, int totalPackets, int numIpPackets) {
    cout << "TIME SPAN: " << tsToString(firstTime) << " - " << tsToString(lastTime) << endl;
    cout << "TOTAL PACKETS: " << totalPackets << endl;
    cout << "IP PACKETS: " << numIpPackets << endl;
}

void Proj4::invokeLengthAnalysisMode() {
    debug("DEBUG: Starting IP length mode...");
    int tfd = openTraceFile();

    pkt_info pinfo;
    while (nextPacket(tfd, &pinfo)) {
        if (pinfo.ethh != nullptr && ntohs(pinfo.ethh->ether_type) == ETHERTYPE_IP) {
            printInfo_LengthAnalysisMode(pinfo);
        }
    }
    close (tfd);
}

void Proj4::printInfo_LengthAnalysisMode(pkt_info pinfo) {
    string ts = tsToString(pinfo.now);
    string caplen = to_string(pinfo.caplen);
    string ip_len = "-";
    string iphl = "-";
    string transport = "-";
    string trans_hl = "-";
    string payload_len = "-";

    if (pinfo.iph != nullptr) {
        u_int16_t tot_len = ntohs(pinfo.iph->tot_len);
        ip_len = to_string(tot_len);
        u_int16_t iph_len = pinfo.iph->ihl * IPWORDMULT;
        iphl = to_string(iph_len);

        if (pinfo.iph->protocol == IPPROTO_TCP) {
            transport = "T";
            if (pinfo.tcph != nullptr) {
                u_int16_t tcph_len = pinfo.tcph->doff * TCPWORDMULT;
                trans_hl = to_string(tcph_len);
                payload_len = to_string(tot_len - iph_len - tcph_len);
            }
        }
        else if (pinfo.iph->protocol == IPPROTO_UDP) {
            transport = "U";
            if (pinfo.udph != nullptr) {
                u_int16_t udph_len = UDPLEN;
                trans_hl = to_string(udph_len);
                payload_len = to_string(tot_len - iph_len - udph_len);
            }
        }
        else {
            transport = "?";
            trans_hl = "?";
            payload_len = "?";
        }
    }

    cout << ts << " "
         << caplen << " "
         << ip_len << " "
         << iphl << " "
         << transport << " "
         << trans_hl << " "
         << payload_len << endl;
}

void Proj4::invokePacketPrintingMode() {
    debug("DEBUG: Starting TCP packet printing mode...");
    int tfd = openTraceFile();

    pkt_info pinfo;
    while (nextPacket(tfd, &pinfo)) {
        if (pinfo.tcph != nullptr && pinfo.iph->protocol == IPPROTO_TCP) {
            printInfo_PacketPrintingMode(pinfo);
        }
    }
    close (tfd);
}

void Proj4::printInfo_PacketPrintingMode(pkt_info pinfo) {
    string ts = tsToString(pinfo.now);
    string src_ip = string(inet_ntoa(*(in_addr *)&pinfo.iph->saddr));
    string src_port = to_string(ntohs(pinfo.tcph->th_sport));
    string dst_ip = string(inet_ntoa(*(in_addr *)&pinfo.iph->daddr));
    string dst_port = to_string(ntohs(pinfo.tcph->th_dport));
    string ip_ttl = to_string(pinfo.iph->ttl);
    string window = to_string(ntohs(pinfo.tcph->th_win));
    string seqno = to_string(ntohl(pinfo.tcph->th_seq));
    string ackno = (pinfo.tcph->th_flags & TH_ACK) ? to_string(ntohl(pinfo.tcph->th_ack)) : "-";

    cout << ts << " "
         << src_ip << " "
         << src_port << " "
         << dst_ip << " "
         << dst_port << " "
         << ip_ttl << " "
         << window << " "
         << seqno << " "
         << ackno << endl;
}

void Proj4::invokeTrafficMatrixMode() {
    debug("DEBUG: Starting traffic matrix mode...");
    int tfd = openTraceFile();

    map<string, size_t> trafficMap;
    pkt_info pinfo;
    while (nextPacket(tfd, &pinfo)) {
        if (pinfo.tcph != nullptr && pinfo.iph->protocol == IPPROTO_TCP) {
            string src_ip = string(inet_ntoa(*(in_addr *)&pinfo.iph->saddr));
            string dst_ip = string(inet_ntoa(*(in_addr *)&pinfo.iph->daddr));
            string key = src_ip + " " + dst_ip;
            u_int16_t tot_len = ntohs(pinfo.iph->tot_len);
            u_int16_t iph_len = pinfo.iph->ihl * IPWORDMULT;
            u_int16_t tcph_len = pinfo.tcph->doff * TCPWORDMULT;
            trafficMap[key] += tot_len - iph_len - tcph_len;
        }
    }
    close (tfd);

    printInfo_TrafficMatrixMode(trafficMap);
}

void Proj4::printInfo_TrafficMatrixMode(map<string, size_t> trafficMap) {
    for (const auto& kv : trafficMap)
        cout << kv.first << " " << kv.second << endl;
}

int Proj4::openTraceFile() {
    debug("DEBUG: Opening tracefile " + traceFile + "...");
    int tfd = open(traceFile.c_str(), O_RDONLY);
    if (tfd < 0)
        errorExit("ERROR: Trace file could not be opened");
    else
        return tfd;
}

bool Proj4::nextPacket(int fd, pkt_info *pinfo) {
    struct meta_info meta;
    unsigned int bytesRead;

    memset(pinfo, 0x0, sizeof(struct pkt_info));
    memset(&meta, 0x0, sizeof(struct meta_info));

    //read in the meta packet info
    bytesRead = read(fd, &meta, sizeof(meta));
    if (bytesRead == 0)
        return false;
    if (bytesRead < sizeof(struct meta_info))
        errorExit("ERROR: Cannot read packet meta information");

    //store the meta info
    pinfo->now = ntohl(meta.secs) + (ntohl(meta.usecs) / USECS_PER_SEC);
    pinfo->caplen = ntohs(meta.caplen);
    if (pinfo->caplen == 0)
        return true;
    if (pinfo->caplen > MAX_PKT_SIZE)
        errorExit("ERROR: Packet is too big");

    //read in the packet info
    bytesRead = read(fd, pinfo->pkt, pinfo->caplen);
    if (bytesRead < pinfo->caplen)
        errorExit("ERROR: Unexpected end of file encountered");

    //extract eth header if entire header is present
    if (pinfo->caplen >= ETH_HLEN) {
        pinfo->ethh = (struct ether_header *)pinfo->pkt;

        //extract fixed ip header if fixed header is present
        if (pinfo->caplen >= ETH_HLEN + IPLEN && ntohs(pinfo->ethh->ether_type) == ETHERTYPE_IP) {
            pinfo->iph = (struct iphdr *)(pinfo->pkt + ETH_HLEN);
            unsigned int adjustedIpHlen = pinfo->iph->ihl * IPWORDMULT;
            //discard the ip header if it was truncatd
            if (pinfo->caplen < ETH_HLEN + adjustedIpHlen)
                pinfo->iph = nullptr;

            //extract tcp header if the fixed header is present
            else if (pinfo->caplen >= ETH_HLEN + adjustedIpHlen + TCPLEN && pinfo->iph->protocol == IPPROTO_TCP) {
                pinfo->tcph = (struct tcphdr *)(pinfo->pkt + ETH_HLEN + adjustedIpHlen);
                unsigned int adjustedTcpHlen = pinfo->tcph->doff * TCPWORDMULT;
                //discard the tcp header if it was truncated
                if (pinfo->caplen < ETH_HLEN + adjustedIpHlen + adjustedTcpHlen)
                    pinfo->tcph = nullptr;
            }

            //extract udp
            else if (pinfo->caplen >= ETH_HLEN + adjustedIpHlen + UDPLEN && pinfo->iph->protocol == IPPROTO_UDP)
                pinfo->udph = (struct udphdr *)(pinfo->pkt + ETH_HLEN + adjustedIpHlen);
        }
    }

    return true;
}

string Proj4::tsToString(double ts) {
    stringstream stream;
    stream << fixed << setprecision(PRINTING_PRECISION) << ts;
    return stream.str();
}
