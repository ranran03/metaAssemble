#include <iostream>
#include <getopt.h>
#include <sstream>

#include <boost/format.hpp>

#include <log4cxx/logger.h>

#include "assemble.h"

namespace opt{
    static std::string asqgFile;
    static std::string outContigsFile;
    static std::string outGraphFile;

    static size_t maxEdges;

    //maxOverlap parameters
    static int maxOverlapDelta;

    //Bubbble parameters
    static int numBubbleRounds = 3;

    //Trim parameters
    static int numTrimRounds = 3;
    static size_t trimLengthThreshold = 150;
}

std::string shortopts = "p:m:b:d:x:l:h";

static const option longopts[] = {
    {"prefix",              required_argument,  NULL,   'p'},
    {"max-edges",           required_argument,  NULL,   'm'},
    {"bubble",              required_argument,  NULL,   'b'},
    {"max-overlap-delta",   required_argument,  NULL,   'd'},
    {"cut-terminal",        required_argument,  NULL,   'x'},
    {"min-branch-length",   required_argument,  NULL,   'l'},
    {NULL, 0, NULL, 0},
};

void parseOptions(int argc, char** argv){
    std::string prefix = "default";
    int opt = -1;
    while((opt = getopt_long(argc, argv, shortopts.c_str(), longopts, NULL)) != -1){
        std::istringstream arg(optarg != NULL ? optarg : "");
        switch (opt){
            case 'p':
                arg >> prefix;
                break;
            case 'm':
                arg >> opt::maxEdges;
                break;
            case 'b':
                arg >> opt::numBubbleRounds;
                break;
            case 'd':
                arg >> opt::maxOverlapDelta;
                break;
            case 'x':
                arg >> opt::numTrimRounds;
                break;
            case 'l':
                arg >> opt::trimLengthThreshold;
                break;
            case 'h':
                printHelps();
                exit(EXIT_SUCCESS);
            
        }
    }
    outContigsFile = prefix + "-contigs.fa"; 
    outGraphFile = prefix + "-graph.asqg.gz";

    //parse the input file
    opt::asqgFile = argv[optind];
}

void assemble(){
    //write down the parameter information
    LOG4CXX_INFO(logger, boost::format("input: %s", % opt::asqgFile));
    LOG4CXX_INFO(logger, boost::format("maxEdges: %s", % opt::maxEdges));
    LOG4CXX_INFO(logger, boost::format("maxOverlapDelta: %s", % opt::maxOverlapDelta));
    LOG4CXX_INFO(logger, boost::format("numTrimRounds: %s", % opt::numTrimRounds));
    LOG4CXX_INFO(logger, boost::format("trimLengthThreshold: %s", % opt::trimLengthThreshold));
    LOG4CXX_INFO(logger, boost::format("outContigsFile: %s", % opt::outContigsFile));
    LOG4CXX_INFO(logger, boost::format("outGraphFile: %s", % opt::outGraphFile));
    
    Bigraph g;

}

int printHelps(){
    std::cout << boost:: format(
            "assemble [OPTION] ...ASQGFILE\n"
            "Create contigs from the assembly graph ASQGFILE.\n"
            "\n"
            "   -v, --version                   output the version\n"
            "   -h, --help                      display the help and exit\n"
            "   -p, --prefix=NAME               use NAME as the prefix of the output files(output files will be NAME-contigs.fa, etc)\n"
            "   -m, --max-edges=N               limit each vertex to a maximum of N edges. For highly repetitive regions\n"
            "                                   This helps save memory by culling excessive edges around unsolvable repeats (default: 128)\n"
            "\n"
            "Bubble removal parameters:\n"
            "   -b, --bubble=N                  perform N bubble removal steps (default: 3)\n"
            "\n"
            "MaximalOverlap parameters:\n"
            "   -d, --max-overlap-delta=LEN     remove branches only if they are less than LEN bases in length (default: 0)\n"
            "\n"
            "Trimming parameters:\n"
            "   -x, --cut-terminal=N            cut off terminal braches in N rounds (defalult: 10)\n"
            "   -l, --min-branch-length=LEN     remove terminal braches only if they are less than LEN bases in length (default: 150)\n"
            "\n" 
            ) << std::endl;
    return 256;
}

int main(int argc, char** argv){
    if(argc < 2){
        return printHelps();
    }

    parseOptions(argc, argv);

    return 0;
}
