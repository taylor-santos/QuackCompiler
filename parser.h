//
// The parser driver just glues together a parser object
// and a lexer object.
//

#include "lex.yy.h"
#include "EvalContext.h"
#include "Messages.h"
#include "AST.h"

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
        if (root->typeCheck(verbose)) {
            std::cerr << "Type check failed" << std::endl;
            return false;
        }
        return true;
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
