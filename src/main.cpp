#include "include/parser.h"
#include <unistd.h>
//#include <string.h>
//#include <unistd.h>
#include <stdlib.h>
#include <fstream>

int main(int argc, char *argv[]) {
    int opt;
    bool verbose = false;
    bool makeExec = false;
    std::string outName = "";
    std::string execName = "";
    std::ifstream inputFile;
    std::istream *input;
    std::ofstream outputFile;
    
    while ((opt = getopt(argc, argv, "vec:o:")) != -1) {
        switch (opt) {
         case 'v':
            verbose = true;
            break;
         case 'e':
            makeExec = true;
            break;
         case 'c':
            outName = std::string(optarg);
            break;
         case 'o':
            makeExec = true;
            execName = std::string(optarg);
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
    } else {
        input = &std::cin;
        if (outName != "") {
            outName = "qk.out.c";
        }
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
        driver.genCode(outName);
    } else {
        std::cerr << "Unable to parse!" << std::endl;
    }
    if (makeExec) {
        if (execName == "") {
            system(("gcc " + outName + " Builtins.c").c_str());
            /*
            char *a0 = strdup("gcc");
            char *a1 = strdup(outName.c_str());
            char *a2 = strdup("Builtins.c");
            char * const  * args = { a0, a1, a2, NULL};
            char * c_s = strdup(outName.c_str());
            char * args[] = {"gcc", c_s, "Builtins.c", NULL};

            execvp(args[0], args);
            */
        }
        else {
            system(("gcc " + outName +  " Builtins.c" + " -o " + execName).c_str());
        }
    }

}
