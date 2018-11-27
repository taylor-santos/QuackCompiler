#include "parser.h"
#include <unistd.h>
#include <fstream>

int main(int argc, char *argv[]) {
    int opt;
    bool verbose = false;
    std::string outName = "";
    std::ifstream inputFile;
    std::istream *input;
    std::ofstream outputFile;
    std::ostream *output;
    
    while ((opt = getopt(argc, argv, "vo:")) != -1) {
        switch (opt) {
         case 'v':
            verbose = true;
            break;
         case 'o':
            outName = std::string(optarg);
            break;
        }
    }
    if (optind < argc) {
        std::string inFilename(argv[optind]);
        if (inFilename.compare(inFilename.size()-3, 3, ".qk") != 0) {
            std::cerr << "Error: Input file (" << inFilename << ") must end in "
                    << "\".qk\"" << std::endl;
            return 1;
        }
        inputFile.open(argv[optind]);
        input = &inputFile;
        int slashIndex = inFilename.find_last_of("/");
        slashIndex = slashIndex == 0 ? 0 : slashIndex + 1;
        int len = inFilename.size() - slashIndex - 3;
        if (outName == "") {
            outName = inFilename.substr(slashIndex, len) + ".c";
        }
        outputFile.open(outName);
        output = &outputFile;
    } else {
        input = &std::cin;
        outputFile.open("qk.out.c");
        output = &outputFile;
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
        driver.genCode(*output);
    } else {
        std::cerr << "Unable to parse!" << std::endl;
    }
}
