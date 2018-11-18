#ifndef AST_H
#define AST_H

#include <string>
#include <sstream>
#include <vector>

namespace AST {
    class ASTNode {
    public:
        virtual void json(std::ostream &out, unsigned int indent = 0) = 0;
    protected:
        void json_indent(std::ostream& out, unsigned int indent);
        void json_head  (std::ostream& out, unsigned int &indent, std::string node_kind);
        void json_close (std::ostream& out, unsigned int &indent);
        void json_child (std::ostream& out, unsigned int indent, std::string field, ASTNode *child);
        void json_string(std::ostream& out, unsigned int indent, std::string field, std::string str);
        void json_int   (std::ostream& out, unsigned int indent, std::string field, unsigned int val);
        template<class NodeType>
        void json_list  (std::ostream& out, unsigned int indent, std::string field, std::vector<NodeType> const& list);
        
    };
    
    class TypedArg : public ASTNode {
        std::string name_;
        std::string type_;
    public:
        TypedArg(std::string name, std::string type): name_(name), type_(type) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class Statement : public ASTNode {
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
    };
    
    class Class : public ASTNode {
        std::string             name_;
        std::vector<TypedArg*>  *args_;
        std::string             extends_;
        std::vector<Statement*> *stmts_;
        std::vector<Method*>    *mthds_;
    public:
        Class(std::string name, std::vector<TypedArg*> *args, std::string extends, std::vector<Statement*> *stmts, std::vector<Method*> *mthds): name_(name), args_(args), extends_(extends), stmts_(stmts), mthds_(mthds) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class Program : public ASTNode {
        std::vector<Class*> *classes_;
        std::vector<Statement*> *stmts_;
    public:
        Program(std::vector<Class*> *classes, std::vector<Statement*> *stmts): classes_(classes), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class RExpr : public Statement {
    };   
    
    class If : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *if_true_stmts_;
        std::vector<Statement*> *if_false_stmts_;
    public:
        If(RExpr *cond, std::vector<Statement*> *if_stmts, std::vector<Statement*> *else_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(else_stmts) {}
        If(RExpr *cond, std::vector<Statement*> *if_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(new std::vector<Statement*>()) {}
        void set_else(std::vector<Statement*> *else_stmts){ delete this->if_false_stmts_; this->if_false_stmts_ = else_stmts; }
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class While : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *stmts_;
    public:
        While(RExpr *cond, std::vector<Statement*> *stmts): cond_(cond), stmts_(stmts) {}
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
    };
    
    class Return : public Statement {
        bool returnsNone_;
        RExpr *r_expr_;
    public:
        Return(RExpr *r_expr): returnsNone_(false), r_expr_(r_expr) {}
        Return(): returnsNone_(true), r_expr_(nullptr) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class TypeAlt : public ASTNode {
        std::string name_;
        std::string type_;
        std::vector<Statement*> *stmts_;
    public:
        TypeAlt(std::string name, std::string type, std::vector<Statement*> *stmts): name_(name), type_(type), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class Typecase : public Statement {
        RExpr *expr_;
        std::vector<TypeAlt*> *alternatives_;
    public:
        Typecase(RExpr *expr, std::vector<TypeAlt*> *alternatives): expr_(expr), alternatives_(alternatives) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
   
    class IntLit : public RExpr {
        unsigned int val_;
    public:
        IntLit(unsigned int val): val_(val) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class StrLit : public RExpr {
        std::string text_;
    public:
        StrLit(std::string text): text_(text) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Call : public RExpr {
        RExpr *obj_;
        std::string mthd_;
        std::vector<RExpr*> *args_;
    public:
        Call(RExpr *obj, std::string mthd, std::vector<RExpr*> *args): obj_(obj), mthd_(mthd), args_(args) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Constructor : public RExpr {
        std::string name_;
        std::vector<RExpr*> *args_;
    public:
        Constructor(std::string name, std::vector<RExpr*> *args): name_(name), args_(args) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class And : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        And(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Or : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        Or(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Not : public RExpr {
        RExpr *expr_;
    public:
        Not(RExpr *expr): expr_(expr) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
}

#endif