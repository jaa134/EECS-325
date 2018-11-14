#include "unistd.h"
#include "cstring"
#include "iostream"
#include "fcntl.h"
#include "iomanip"
#include "sstream"
#include "map"
#include "netinet/in.h"
#include "netinet/ip.h"
#include "netinet/udp.h"
#include "netinet/tcp.h"
#include "net/ethernet.h"
#include <arpa/inet.h>

using namespace std;

#define MAX_PKT_SIZE 1600
#define USECS_PER_SEC 1000000.0
#define IPWORDMULT 4
#define IPLEN 20
#define TCPWORDMULT 4
#define TCPLEN 20
#define UDPLEN 8
#define PRINTING_PRECISION 6
#define USAGE "Usage: ./proj4 -t trace_file -s|-l|-p|-m [-v]\n"

struct pkt_info
{
    unsigned short caplen;
    double now;
    unsigned char pkt [MAX_PKT_SIZE];
    struct ether_header *ethh;
    struct iphdr *iph;
    struct tcphdr *tcph;
    struct udphdr *udph;
};

struct meta_info
{
    unsigned short caplen;
    unsigned short ignored;
    unsigned int secs;
    unsigned int usecs;
};

class Proj4 {

public:
    void exec(int, char**);

private:
/*************************
SERVER OPERATION VARIABLES
*************************/
    bool useVerboseOutput;
    enum Mode { UNSET, SUMMARY, LENGTH, PRINTING, TRAFFIC };
    Mode mode = Mode::UNSET;
    string traceFile;

/******************************
ERROR HANDLING SPECIFIC METHODS
******************************/
    void debug(string);
    [[noreturn]] void errorExit(string);

/************************
ARGUMENT SPECIFIC METHODS
************************/
    void extractArgs(int, char**);
    void errorIfDupMode();

/*********************
TRACE ANALYSIS METHODS
*********************/
    void invokeSummaryMode();
    void invokeLengthAnalysisMode();
    void invokePacketPrintingMode();
    void invokeTrafficMatrixMode();
    void printInfo_SummaryMode(double firstTime, double lastTime, int totalPackets, int numIpPackets);
    void printInfo_LengthAnalysisMode(pkt_info);
    void printInfo_PacketPrintingMode(pkt_info);
    void printInfo_TrafficMatrixMode(map<string, size_t>);
    int openTraceFile();
    bool nextPacket(int, pkt_info*);
    string tsToString(double);
};
