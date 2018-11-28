#ifndef AST_H_
#define AST_H_

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <algorithm>

template<class T>
std::vector<T> difference(std::vector<T> a, std::vector<T> b);
template<class T>
std::vector<T> intersection(std::vector<T> a, std::vector<T> b);
template<class K, class V>
std::pair<std::map<K, V>, std::map<K, std::pair<V, V>>> map_intersection(
        std::map<K, V> a, std::map<K, V> b);
template<class K, class V>
std::map<K, V> map_difference(std::map<K, V> a, std::map<K, V> b);
namespace AST {
struct ClassStruct {
    std::string name;
    ClassStruct *super;
    std::vector<ClassStruct*> children;
    std::map<ClassStruct*, ClassStruct*> LCA;
    struct MethodStruct *constructor;
    std::map<std::string, struct MethodStruct*> methodTable;
    std::vector<MethodStruct*> methodOrder;
    std::map<std::string, std::pair<ClassStruct*, bool>> fieldTable;
};
struct MethodStruct {
    std::string name;
    ClassStruct *clazz;
    ClassStruct *type;
    std::vector<ClassStruct*> argTypes;
    std::map<std::string, std::pair<ClassStruct*, bool>> symbolTable;
};

class ASTNode {
 public:
    virtual void json(std::ostream &out, unsigned int indent = 0) = 0;
    std::string getFilename() const { return filename; }
    int getLast_col() const { return last_col; }
    int getLast_line() const { return last_line; }
    int getFirst_col() const { return first_col; }
    int getFirst_line() const { return first_line; }
    std::string getPosition() const;
    void set_position(int first_line, int first_col, int last_line,
    int last_col, std::string *filename);

 protected:
    static std::map<std::string, ClassStruct*> classTable, builtinTypes,
            builtinIdents;
    int first_line, first_col, last_line, last_col;
    std::string filename;
    void json_indent(std::ostream& out, unsigned int indent);
    void json_head(std::ostream& out, unsigned int &indent,
            std::string const &node_kind);
    void json_close(std::ostream& out, unsigned int &indent);
    void json_child(std::ostream& out, unsigned int indent,
            std::string const &field, ASTNode *child);
    void json_string(std::ostream& out, unsigned int indent,
            std::string const &field, std::string const &str);
    void json_int(std::ostream& out, unsigned int indent,
            std::string const &field, unsigned int val);
    template<class NodeType>
    void json_list(std::ostream& out, unsigned int indent,
            std::string const &field, std::vector<NodeType> const& list);
    ClassStruct *addBuiltinType(std::string name, ClassStruct* super);
    void addBuiltinIdent(std::string name, ClassStruct *type);
    MethodStruct *addBuiltinMethod(ClassStruct *clazz, std::string name,
            ClassStruct *type, std::vector<ClassStruct*> const &argTypes);
};

class TypedArg : public ASTNode {
    std::string name_;
    std::string type_;
 public:
    TypedArg(std::string name, std::string type): name_(name), type_(type) {}
    virtual void json(std::ostream &out, unsigned int indent = 0) override;
    std::string getType() const { return type_; }
    std::string getName() const { return name_; }
};

class Statement : public ASTNode {
 protected:
    static unsigned int tempVarID;
    static unsigned int labelID;
 public:
    virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &faile5d) = 0;
    virtual void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) = 0;
    virtual std::pair<ClassStruct*, bool> getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed);
    virtual void generateCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) = 0;
};

class Method : public ASTNode {
    std::string             name_;
    std::vector<TypedArg*>  *args_;
    bool                    isTyped_;
    std::string             type_;
    std::vector<Statement*> *stmts_;
    MethodStruct            *methodStruct_;
 public:
    Method(std::string name, std::vector<TypedArg*> *args,
            std::vector<Statement*> *stmts): name_(name), args_(args),
            isTyped_(false), type_(""), stmts_(stmts) {}
    Method(std::string name, std::vector<TypedArg*> *args, std::string type,
            std::vector<Statement*> *stmts): name_(name), args_(args),
            isTyped_(true), type_(type), stmts_(stmts) {}
    virtual void json(std::ostream &out, unsigned int indent = 0) override;
    std::string getType() const;
    std::vector<TypedArg*> *getArgs() const { return args_; }
    std::vector<std::string> getArgNames() const;
    std::string getName() const { return name_; }
    std::vector<Statement*> *getStatements() const { return stmts_; }
    MethodStruct *getMethodStruct() const { return methodStruct_; }
    void setMethodStruct(MethodStruct* methodStruct) {
            this->methodStruct_ = methodStruct; }
};

class Class : public ASTNode {
    std::string             name_;
    std::vector<TypedArg*>  *args_;
    std::string             extends_;
    std::vector<Statement*> *stmts_;
    std::vector<Method*>    *mthds_;
    ClassStruct             *classStruct_;
 public:
    Class(std::string name, std::vector<TypedArg*> *args, std::string extends,
            std::vector<Statement*> *stmts, std::vector<Method*> *mthds):
            name_(name), args_(args), extends_(extends), stmts_(stmts),
            mthds_(mthds) {}
    virtual void json(std::ostream &out, unsigned int indent = 0) override;
    std::string getExtends() const { return extends_; }
    std::string getName() const { return name_; }
    ClassStruct* getClassStruct() const { return this->classStruct_; }
    std::vector<Method*>* getMethods() const { return mthds_; }
    std::vector<Statement*>* getStatements() const { return stmts_; }
    std::vector<TypedArg*>* getArgs() const { return args_; }
    std::vector<std::string> getArgNames() const;
    void setClassStruct(ClassStruct* classStruct) {
            this->classStruct_ = classStruct; }
};

class Program : public ASTNode {
    std::vector<Class*> *classes_;
    std::vector<Statement*> *stmts_;
    MethodStruct *body_;
    void addBuiltins();
    void buildClassMap(bool &failed);
    void checkClassHierarchy(bool &failed);
    void buildLCAs(bool &failed);
    void buildMethodTables(bool &failed);
    void checkMethodInheritance(bool &failed);
    void getVars(bool &failed);
    void inferTypes(bool &failed);
    void checkFieldInheritance(bool &failed);
    void checkReturnTypes(bool &failed);
 public:
    Program(std::vector<Class*> *classes, std::vector<Statement*> *stmts):
            classes_(classes), stmts_(stmts) {}
    virtual void json(std::ostream &out, unsigned int indent = 0) override;
    bool typeCheck(bool verbose);
    void genCode(std::ostream &file);
};

class TypeAlt : public ASTNode {
    std::string name_;
    std::string type_;
    std::vector<Statement*> *stmts_;
 public:
    TypeAlt(std::string name, std::string type, std::vector<Statement*> *stmts):
            name_(name), type_(type), stmts_(stmts) {}
    std::string getName() const { return name_; }
    std::string getType() const { return type_; }
    std::vector<Statement*>* getStatements() const {
        return stmts_;
    }
    virtual void json(std::ostream &out, unsigned int indent = 0) override;
};

class RExpr : public Statement {
 public:
    virtual bool isThis() const { return false; }
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) final;
    virtual ClassStruct *getType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) = 0;
    void getVars(std::vector<std::string> &vars,
        std::vector<std::string> &fields, std::map<std::string,
        std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
        std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
        bool &failed) override {}
    void generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) final { this->generateRExprCode(file,
		    indent, thisClass, thisMethod); }
    virtual std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) = 0;
    virtual void generateBranchCode(std::ostream &file, int indent,
	    ClassStruct *thisClass, MethodStruct *thisMethod,
	    std::string trueLabel, std::string falseLabel);
};

class If : public Statement {
    RExpr *cond_;
    std::vector<Statement*> *if_true_stmts_;
    std::vector<Statement*> *if_false_stmts_;
 public:
    If(RExpr *cond, std::vector<Statement*> *if_stmts,
            std::vector<Statement*> *else_stmts): cond_(cond),
            if_true_stmts_(if_stmts), if_false_stmts_(else_stmts) {}
    If(RExpr *cond, std::vector<Statement*> *if_stmts): cond_(cond),
            if_true_stmts_(if_stmts),
            if_false_stmts_(new std::vector<Statement*>()) {}
    void set_else(std::vector<Statement*> *else_stmts);
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) override;
    std::pair<ClassStruct*, bool> getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) override;
};

class While : public Statement {
    RExpr *cond_;
    std::vector<Statement*> *stmts_;
 public:
    void json(std::ostream &out, unsigned int indent = 0) override;
    While(RExpr *cond, std::vector<Statement*> *stmts): cond_(cond),
            stmts_(stmts) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    std::pair<ClassStruct*, bool> getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) override;
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) override;
    void generateCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class LExpr : public RExpr {
    bool isField_;
    RExpr *obj_;
    std::string name_;
 public:
    void json(std::ostream &out, unsigned int indent = 0) override;
    LExpr(RExpr *obj, std::string name): isField_(true), obj_(obj),
            name_(name) {}
    explicit LExpr(std::string name): isField_(false), obj_(nullptr),
            name_(name) {}
    bool isField() const { return isField_; }
    bool isThis() const override { return !isField_ && name_ == "this"; }
    bool isAssignable() const;
    std::string getName() const { return name_; }
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class Assignment : public Statement {
    LExpr       *l_expr_;
    bool         isTyped_;
    std::string  type_;
    RExpr       *r_expr_;
 public:
    Assignment(LExpr *l_expr, std::string type, RExpr *r_expr): l_expr_(l_expr),
            isTyped_(true), type_(type), r_expr_(r_expr) {}
    Assignment(LExpr *l_expr, RExpr *r_expr): l_expr_(l_expr), isTyped_(false),
            type_(""), r_expr_(r_expr) {}
    void json(std::ostream &out, unsigned int indent = 0) override;
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) override;
    void generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) override;
    
};

class Return : public Statement {
    bool returnsNone_;
    RExpr *r_expr_;
 public:
    Return(): returnsNone_(true), r_expr_(nullptr) {}
    explicit Return(RExpr *r_expr): returnsNone_(false), r_expr_(r_expr) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) override;
    std::pair<ClassStruct*, bool> getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) override;
};

class Typecase : public Statement {
    RExpr *expr_;
    std::vector<TypeAlt*> *alternatives_;
 public:
    Typecase(RExpr *expr, std::vector<TypeAlt*> *alternatives): expr_(expr),
            alternatives_(alternatives) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) override;
    std::pair<ClassStruct*, bool> getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) override;
};

class IntLit : public RExpr {
    unsigned int val_;
 public:
    explicit IntLit(unsigned int val): val_(val) {}
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override { return this->builtinTypes["Int"]; }
    void json(std::ostream &out, unsigned int indent = 0) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class StrLit : public RExpr {
    std::string text_;
 public:
    explicit StrLit(std::string text): text_(text) {}
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override { return this->builtinTypes["String"]; }
    void json(std::ostream &out, unsigned int indent = 0) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class Call : public RExpr {
    RExpr *obj_;
    std::string mthd_;
    std::vector<RExpr*> *args_;
 public:
    Call(RExpr *obj, std::string mthd, std::vector<RExpr*> *args): obj_(obj),
            mthd_(mthd), args_(args) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class Constructor : public RExpr {
    std::string name_;
    std::vector<RExpr*> *args_;
 public:
    Constructor(std::string name, std::vector<RExpr*> *args): name_(name),
            args_(args) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class And : public RExpr {
    RExpr *lhs_;
    RExpr *rhs_;
 public:
    And(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateBranchCode(std::ostream &file, int indent,
	    ClassStruct *thisClass, MethodStruct *thisMethod,
	    std::string trueLabel, std::string falseLabel) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class Or : public RExpr {
    RExpr *lhs_;
    RExpr *rhs_;
 public:
    Or(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateBranchCode(std::ostream &file, int indent,
	    ClassStruct *thisClass, MethodStruct *thisMethod,
	    std::string trueLabel, std::string falseLabel) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};

class Not : public RExpr {
    RExpr *expr_;
 public:
    explicit Not(RExpr *expr): expr_(expr) {}
    void getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) override;
    ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) override;
    void json(std::ostream &out, unsigned int indent = 0) override;
    void generateBranchCode(std::ostream &file, int indent,
	    ClassStruct *thisClass, MethodStruct *thisMethod,
	    std::string trueLabel, std::string falseLabel) override;
    std::string generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) override;
};
}  // namespace AST
#endif  // AST_H_
