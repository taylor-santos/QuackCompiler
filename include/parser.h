//
// The parser driver just glues together a parser object
// and a lexer object.
//

#include "build/lex.yy.h"
#include "include/Messages.h"
#include "include/AST.h"
#include <fstream>

class Driver {
 public:
    explicit Driver(reflex::Input in) : lexer(in), parser(new yy::parser(lexer,
            &root)), root(nullptr), parsed(false), typeChecked(false) {}
    ~Driver() { delete parser; }
    bool parse() {
        int result = parser->parse();
        this->parsed = result == 0 && report::ok();
        return this->parsed;
    }
    bool typeCheck(bool verbose) {
        this->typeChecked = !root->typeCheck(verbose);
	if (!this->typeChecked) {
            std::cerr << "Type check failed" << std::endl;
            return false;
        }
        return true;
    }
    void genCode(const std::string &filename) {
	if (this->parsed && this->typeChecked) {
        std::ostream *stream;
        std::ofstream file;
        file.open(filename);
        stream = &file;
	    this->root->genCode(*stream);
	}
    }
    void json(std::ostream &out){
        root->json(out);
    }
 private:
    yy::Lexer     lexer;
    yy::parser   *parser;
    AST::Program *root;
    bool          parsed;
    bool          typeChecked;
};
