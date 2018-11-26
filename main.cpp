#include "parser.h"
#include <unistd.h>
#include <fstream>

int main(int argc, char *argv[]) {
    int opt;
    bool verbose = false;
    char *outName = nullptr;
    std::ifstream inputFile;
    std::istream *input;
    
    while ((opt = getopt(argc, argv, "vo:")) != -1) {
        switch (opt) {
         case 'v':
            verbose = true;
            break;
         case 'o':
             outName = strdup(optarg);
             break;
        }
    }
    if (optind < argc) {
        inputFile.open(argv[optind]);
        input = &inputFile;
    } else {
        input = &std::cin;
    }
    
    Driver driver(*input);
    bool parse_result = driver.parse();
    if (parse_result) {
        if (verbose) {
            driver.json(std::cout);
            std::cout << std::endl;
            std::cerr << "Finished parse with no errors" << std::endl;
        }
        driver.typeCheck(verbose);
    } else {
        std::cerr << "Unable to parse!" << std::endl;
    }
}
