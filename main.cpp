#include "parser.h"

int main(int argc, char *argv[]) {
    Driver driver(std::cin);
    bool verbose = false;
    if (argc >= 2) {
        for (int i=1; i<argc; i++) {
            if (strcmp(argv[i], "-v") == 0) {
                verbose = true;
            }
        }
    }
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
