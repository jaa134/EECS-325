/**
* Jacob Alspaw
* jaa134
* proj4.hpp
* 11-14-2018
* 
* Header file for proj4.cpp; Outlines variables and 
* method stubs used for processing a packet trace file.
**/

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
    //print a debug message to stdout when in verbose mode
    void debug(string);
    //print a error message and quit the program with a error flag set
    [[noreturn]] void errorExit(string);

/************************
ARGUMENT SPECIFIC METHODS
************************/
    //extract the execution arguments
    void extractArgs(int, char**);
    //error and quit if duplicate mode is set
    void errorIfDupMode();

/*********************
TRACE ANALYSIS METHODS
*********************/
    //invoke summary mode behavior per project guidelines
    //provides a high level summary of the trace file
    void invokeSummaryMode();
    //invoke length analysis mode behavior per project guidelines
    //prints length information about each IP packet in the packet trace file
    void invokeLengthAnalysisMode();
    //invoke packet printing mode behavior per project guidelines
    //prints information about each TCP packet in the packet trace file
    void invokePacketPrintingMode();
    //invoke traffic matrix mode behavior per project guidelines
    //print the total amount of application layer traffic carried from each host to each of its peers using TCP 
    void invokeTrafficMatrixMode();
    //print info for summary mode per project guidelines
    void printInfo_SummaryMode(double firstTime, double lastTime, int totalPackets, int numIpPackets);
    //print info for length analysis mode per project guidelines
    void printInfo_LengthAnalysisMode(pkt_info);
    //print info for packet printing mode per project guidelines
    void printInfo_PacketPrintingMode(pkt_info);
    //print info for traffic matrix mode per project guidelines
    void printInfo_TrafficMatrixMode(map<string, size_t>);
    //attempts to open the tracefile given in the execution arguments
    //will produce an error and quit if file is not available
    int openTraceFile();
    //finds the next packet in the trace file given by the fd
    //will set the pkt_info values by reference.
    //returns true if packet was found and false otherwise
    //throws errors is tracefile contents corrupt
    bool nextPacket(int, pkt_info*);
    string tsToString(double);
};
