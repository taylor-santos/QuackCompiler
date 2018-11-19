#ifndef AST_H
#define AST_H

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

namespace AST {
    struct ClassStruct {
        std::string name;
        ClassStruct *super;
        std::map<ClassStruct*, ClassStruct*> LCA;
        std::map<std::string, struct MethodStruct*> methodTable;
        std::map<std::string, ClassStruct*> fieldTable;
    };
    struct MethodStruct {
        std::string name;
        ClassStruct *clazz;
        ClassStruct *type;
        std::vector<ClassStruct*> argTypes;
        std::map<std::string, ClassStruct*> symbolTable;
    };
    class ASTNode {
    public:
        virtual void json(std::ostream &out, unsigned int indent = 0) = 0;
        void set_position(int first_line, int first_col, int last_line, int last_col, std::string *filename){
            this->first_line = first_line;
            this->first_col = first_col;
            this->last_line = last_line;
            this->last_col = last_col;
            if (filename)
                this->filename = *filename;
        }
        std::string getFilename() const { return filename; }
        int getLast_col() const { return last_col; }
        int getLast_line() const { return last_line; }
        int getFirst_col() const { return first_col; }
        int getFirst_line() const { return first_line; }
        std::string getPosition() const { return std::to_string(first_line) + ":" + std::to_string(first_col) + ":"; }
    protected:
        void json_indent(std::ostream& out, unsigned int indent);
        void json_head  (std::ostream& out, unsigned int &indent, std::string node_kind);
        void json_close (std::ostream& out, unsigned int &indent);
        void json_child (std::ostream& out, unsigned int indent, std::string field, ASTNode *child);
        void json_string(std::ostream& out, unsigned int indent, std::string field, std::string str);
        void json_int   (std::ostream& out, unsigned int indent, std::string field, unsigned int val);
        template<class NodeType>
        void json_list  (std::ostream& out, unsigned int indent, std::string field, std::vector<NodeType> const& list);
        static std::map<std::string, ClassStruct*> classTable_;
        int first_line, first_col, last_line, last_col;
        std::string filename;
    };
    
    class TypedArg : public ASTNode {
        std::string name_;
        std::string type_;
    public:
        TypedArg(std::string name, std::string type): name_(name), type_(type) {}
        void json(std::ostream &out, unsigned int indent = 0);

        std::string getType() const {
            return type_;
        }

        std::string getName() const {
            return name_;
        }
    };
    
    class Statement : public ASTNode {
    public:
        virtual void getFields(std::vector<std::string> &fields, bool &failed) = 0;
    };
    
    class Method : public ASTNode {
        std::string             name_;
        std::vector<TypedArg*>  *args_;
        bool                    isTyped_;
        std::string             type_;
        std::vector<Statement*> *stmts_;
    public:
        Method(std::string name, std::vector<TypedArg*> *args, std::vector<Statement*> *stmts): name_(name), args_(args), isTyped_(false), type_(""), stmts_(stmts) {}
        Method(std::string name, std::vector<TypedArg*> *args, std::string type, std::vector<Statement*> *stmts): name_(name), args_(args), isTyped_(true), type_(type), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
        std::string getType() const {
            return type_;
        }

        std::vector<TypedArg*> *getArgs() const {
            return args_;
        }

        std::string getName() const {
            return name_;
        }

        std::vector<Statement*> *getStatements() const {
            return stmts_;
        }
        
    };
    
    class Class : public ASTNode {
        std::string             name_;
        std::vector<TypedArg*>  *args_;
        std::string             extends_;
        std::vector<Statement*> *stmts_;
        std::vector<Method*>    *mthds_;
        ClassStruct             *classStruct_;
    public:
        Class(std::string name, std::vector<TypedArg*> *args, std::string extends, std::vector<Statement*> *stmts, std::vector<Method*> *mthds): name_(name), args_(args), extends_(extends), stmts_(stmts), mthds_(mthds) {}
        void json(std::ostream &out, unsigned int indent = 0);
        std::string getExtends() const { return extends_; }
        std::string getName() const { return name_; }

        void setClassStruct(ClassStruct* classStruct) {
            this->classStruct_ = classStruct;
        }

        ClassStruct* getClassStruct() const {
            return this->classStruct_;
        }

        std::vector<Method*>* getMethods() const {
            return mthds_;
        }

        std::vector<Statement*>* getStatements() const {
            return stmts_;
        }
    };
    
    class Program : public ASTNode {
        std::vector<Class*> *classes_;
        std::vector<Statement*> *stmts_;
        void buildClassMap(bool &failed);
        void checkClassHierarchy(bool &failed);
        void buildLCAs(bool &failed);
        void buildMethodTables(bool &failed);
        void checkMethodInheritance(bool &failed);
        void getClassFields(bool &failed);
    public:
        bool typeCheck();
        Program(std::vector<Class*> *classes, std::vector<Statement*> *stmts): classes_(classes), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class RExpr : public Statement {
    public:
        virtual bool isThis() { return false; }
    };   
    
    class If : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *if_true_stmts_;
        std::vector<Statement*> *if_false_stmts_;
    public:
        If(RExpr *cond, std::vector<Statement*> *if_stmts, std::vector<Statement*> *else_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(else_stmts) {}
        If(RExpr *cond, std::vector<Statement*> *if_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(new std::vector<Statement*>()) {}
        void set_else(std::vector<Statement*> *else_stmts){ delete this->if_false_stmts_; this->if_false_stmts_ = else_stmts; }
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy1 = fields, fieldsCopy2;
            cond_->getFields(fieldsCopy1, failed);
            if (fieldsCopy1.size() != fields.size()) {
                for (auto field : difference(fieldsCopy1, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            fieldsCopy1 = fields;
            for (Statement *s : *if_true_stmts_) {
                s->getFields(fieldsCopy1, failed);
            }
            fieldsCopy2 = fields;
            for (Statement *s : *if_false_stmts_) {
                s->getFields(fieldsCopy2, failed);
            }
            
            fields = intersection(fieldsCopy1, fieldsCopy2);            
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class While : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *stmts_;
    public:
        While(RExpr *cond, std::vector<Statement*> *stmts): cond_(cond), stmts_(stmts) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy1 = fields;
            cond_->getFields(fieldsCopy1, failed);
            if (fieldsCopy1.size() != fields.size()) {
                for (auto field : difference(fieldsCopy1, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            fieldsCopy1 = fields;
            for (Statement *s : *stmts_) {
                s->getFields(fieldsCopy1, failed);
            }           
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class LExpr : public RExpr {
        bool isField_;
        RExpr *obj_;
        std::string name_;
    public:
        LExpr(RExpr *obj, std::string name): isField_(true), obj_(obj), name_(name) {}
        LExpr(std::string name): isField_(false), obj_(nullptr), name_(name) {}
        void json(std::ostream &out, unsigned int indent = 0);
        void getFields(std::vector<std::string> &fields, bool &failed) {
            if (isField_ && obj_->isThis()) {
                if (std::find(fields.begin(), fields.end(), this->name_) == fields.end()) {
                    std::cerr << this->getPosition() << " Error: \"this." << this->name_ << "\" used before initialization" << std::endl;
                    failed = true;
                }
            }
        }
        bool isThis() {
            return !isField_ && name_ == "this";
        }
        bool isAssignable() {
            return obj_->isThis();
        }
        std::string const getName(){ return name_; }

        std::string getName() const {
            return name_;
        }
    };
    
    class Assignment : public Statement {
        LExpr       *l_expr_;
        bool         isTyped_;
        std::string  type_;
        RExpr       *r_expr_;
    public:
        Assignment(LExpr *l_expr, std::string type, RExpr *r_expr): l_expr_(l_expr), isTyped_(true), type_(type), r_expr_(r_expr) {} 
        Assignment(LExpr *l_expr, RExpr *r_expr): l_expr_(l_expr), isTyped_(false), type_(""), r_expr_(r_expr) {} 
        void json(std::ostream &out, unsigned int indent = 0);
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            r_expr_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (l_expr_->isAssignable()) {
                fields.push_back(l_expr_->getName());
            } else {
                std::cerr << this->getPosition() << " Error: Cannot assign to field \"" << l_expr_->getName() << "\" outside of method" << std::endl;
                failed = true;
            }
        }
    };
    
    class Return : public Statement {
        bool returnsNone_;
        RExpr *r_expr_;
    public:
        Return(RExpr *r_expr): returnsNone_(false), r_expr_(r_expr) {}
        Return(): returnsNone_(true), r_expr_(nullptr) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            r_expr_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class TypeAlt : public ASTNode {
        std::string name_;
        std::string type_;
        std::vector<Statement*> *stmts_;
    public:
        TypeAlt(std::string name, std::string type, std::vector<Statement*> *stmts): name_(name), type_(type), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);

        std::vector<Statement*>* getStatements() const {
            return stmts_;
        }
    };
    
    class Typecase : public Statement {
        RExpr *expr_;
        std::vector<TypeAlt*> *alternatives_;
    public:
        Typecase(RExpr *expr, std::vector<TypeAlt*> *alternatives): expr_(expr), alternatives_(alternatives) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, ret;
            bool firstCase = true;
            expr_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
           
            for (TypeAlt *alt : *alternatives_) {
                fieldsCopy = fields;
                for (Statement *s : *alt->getStatements()) {
                    s->getFields(fieldsCopy, failed);
                }
                if (firstCase) {
                    firstCase = false;
                    ret = fieldsCopy;
                }else{
                    ret = intersection(fieldsCopy, ret);
                }
            }
            fields = ret;
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
   
    class IntLit : public RExpr {
        unsigned int val_;
    public:
        IntLit(unsigned int val): val_(val) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class StrLit : public RExpr {
        std::string text_;
    public:
        StrLit(std::string text): text_(text) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Call : public RExpr {
        RExpr *obj_;
        std::string mthd_;
        std::vector<RExpr*> *args_;
    public:
        Call(RExpr *obj, std::string mthd, std::vector<RExpr*> *args): obj_(obj), mthd_(mthd), args_(args) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            obj_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            for (RExpr *arg : *args_) {
                fieldsCopy = fields;
                arg->getFields(fieldsCopy, failed);
                if (fieldsCopy.size() != fields.size()) {
                    for (auto field : difference(fieldsCopy, fields)) {
                        std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                    }
                    failed = true;
                }     
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Constructor : public RExpr {
        std::string name_;
        std::vector<RExpr*> *args_;
    public:
        Constructor(std::string name, std::vector<RExpr*> *args): name_(name), args_(args) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            for (RExpr *arg : *args_) {
                fieldsCopy = fields;
                arg->getFields(fieldsCopy, failed);
                if (fieldsCopy.size() != fields.size()) {
                    for (auto field : difference(fieldsCopy, fields)) {
                        std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                    }
                    failed = true;
                }     
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class And : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        And(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            lhs_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            fieldsCopy = fields;
            rhs_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Or : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        Or(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            lhs_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            fieldsCopy = fields;
            rhs_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Not : public RExpr {
        RExpr *expr_;
    public:
        Not(RExpr *expr): expr_(expr) {}
        void getFields(std::vector<std::string> &fields, bool &failed) {
            std::vector<std::string> fieldsCopy = fields;
            expr_->getFields(fieldsCopy, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
}
    
#endif