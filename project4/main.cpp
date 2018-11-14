/**
* Jacob Alspaw
* jaa134
* main.cpp
* 11-14-2018
* 
* Main method for proj4; Implements proj4 object
* used for processing a packet trace file. Executes
* the project with the command line args.
**/

#include "proj4.hpp"

int main(int argc, char **argv) {
    Proj4 p4;
    p4.exec(argc, argv);
}
