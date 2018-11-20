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
        std::vector<ClassStruct*> children;
        std::map<ClassStruct*, ClassStruct*> LCA;
        struct MethodStruct *constructor;
        std::map<std::string, struct MethodStruct*> methodTable;
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
        ClassStruct *addBuiltinType(std::string name, ClassStruct* super) {
            ClassStruct *cs = new ClassStruct({name, super});
            if (super)
                super->children.push_back(cs);
            cs->constructor = new MethodStruct({name, cs, cs});
            this->builtinTypes[name] = cs;
            this->classTable[name] = cs;
            return cs;
        }
        void addBuiltinIdent(std::string name, ClassStruct *type) {
            builtinIdents[name] = type;
        }
        MethodStruct *addBuiltinMethod(ClassStruct *clazz, std::string name, ClassStruct *type, std::vector<ClassStruct*> argTypes) {
            MethodStruct *ms = new MethodStruct({name, clazz, type, argTypes});
            clazz->methodTable[name] = ms;
            return ms;
        }
        int first_line, first_col, last_line, last_col;
        std::string filename;
        static std::map<std::string, ClassStruct*> classTable;
        static std::map<std::string, ClassStruct*> builtinTypes;
        static std::map<std::string, ClassStruct*> builtinIdents;
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
        virtual void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) = 0;
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {}
    };
    
    class Method : public ASTNode {
        std::string             name_;
        std::vector<TypedArg*>  *args_;
        bool                    isTyped_;
        std::string             type_;
        std::vector<Statement*> *stmts_;
        MethodStruct            *methodStruct_;
    public:
        Method(std::string name, std::vector<TypedArg*> *args, std::vector<Statement*> *stmts): name_(name), args_(args), isTyped_(false), type_(""), stmts_(stmts) {}
        Method(std::string name, std::vector<TypedArg*> *args, std::string type, std::vector<Statement*> *stmts): name_(name), args_(args), isTyped_(true), type_(type), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
        std::string getType() const {
            if (isTyped_)
                return type_;
            else
                return "Nothing";
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
        MethodStruct *getMethodStruct() const {
            return methodStruct_;
        }

        void setMethodStruct(MethodStruct* methodStruct) {
            this->methodStruct_ = methodStruct;
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

        std::vector<TypedArg*>* getArgs() const {
            return args_;
        }
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
        
    public:
        bool typeCheck();
        Program(std::vector<Class*> *classes, std::vector<Statement*> *stmts): classes_(classes), stmts_(stmts) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class RExpr : public Statement {
    public:
        virtual bool isThis() { return false; }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) { return nullptr; }
    };   
    
    class If : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *if_true_stmts_;
        std::vector<Statement*> *if_false_stmts_;
    public:
        If(RExpr *cond, std::vector<Statement*> *if_stmts, std::vector<Statement*> *else_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(else_stmts) {}
        If(RExpr *cond, std::vector<Statement*> *if_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(new std::vector<Statement*>()) {}
        void set_else(std::vector<Statement*> *else_stmts){ delete this->if_false_stmts_; this->if_false_stmts_ = else_stmts; }
        virtual void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy1 = fields, fieldsCopy2, initCopy1 = init, initCopy2;
            cond_->getVars(fieldsCopy1, initCopy1, table, inConstructor, failed);
            if (fieldsCopy1.size() != fields.size()) {
                for (auto field : difference(fieldsCopy1, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            if (initCopy1.size() != init.size()) {
                for (auto var : difference(initCopy1, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            fieldsCopy1 = fields;
            initCopy1 = init;
            for (Statement *s : *if_true_stmts_) {
                s->getVars(fieldsCopy1, initCopy1, table, inConstructor, failed);
            }
            fieldsCopy2 = fields;
            initCopy2 = init;
            for (Statement *s : *if_false_stmts_) {
                s->getVars(fieldsCopy2, initCopy2, table, inConstructor, failed);
            }
            fields = intersection(fieldsCopy1, fieldsCopy2);    
            init = intersection(initCopy1, initCopy2);
        }
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            ClassStruct *condType = cond_->getType(thisClass, thisMethod, failed);
            if (condType != nullptr && condType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: If statement uses type \"" << condType->name << "\" as condition, must be of type Boolean" << std::endl;
                failed = true;
            }
            for (Statement *s : *if_true_stmts_) {
                s->updateTypes(thisClass, thisMethod, changed, failed);
            }
            for (Statement *s : *if_false_stmts_) {
                s->updateTypes(thisClass, thisMethod, changed, failed);
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class While : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *stmts_;
    public:
        While(RExpr *cond, std::vector<Statement*> *stmts): cond_(cond), stmts_(stmts) {}
        virtual void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy1 = fields, initCopy1 = init;
            cond_->getVars(fieldsCopy1, initCopy1, table, inConstructor, failed);
            if (fieldsCopy1.size() != fields.size()) {
                for (auto field : difference(fieldsCopy1, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            if (initCopy1.size() != init.size()) {
                for (auto var : difference(initCopy1, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl; 
                }
                failed = true;
            }
            fieldsCopy1 = fields;
            initCopy1 = init;
            for (Statement *s : *stmts_) {
                s->getVars(fieldsCopy1, initCopy1, table, inConstructor, failed);
            }           
        }
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            ClassStruct *condType = cond_->getType(thisClass, thisMethod, failed);
            if (condType != nullptr && condType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: While statement uses type \"" << condType->name << "\" as condition, must be of type Boolean" << std::endl;
                failed = true;
            }
            for (Statement *s : *stmts_) {
                s->updateTypes(thisClass, thisMethod, changed, failed);
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
        virtual void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            if (isField_) {
                if (obj_->isThis()) {
                    if (std::find(fields.begin(), fields.end(), this->name_) == fields.end()) {
                        std::cerr << this->getPosition() << " Error: \"this." << this->name_ << "\" used before initialization" << std::endl;
                        failed = true;
                    }
                } else {
                    //Field of different class
                }
            } else {
                //Local variable
                if (std::find(init.begin(), init.end(), this->name_) == init.end()) {
                    std::cerr << this->getPosition() << " Error: \"" << this->name_ << "\" used before initialization" << std::endl;
                    failed = true;
                }
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            if (isField_) {
                if (isThis()) {
                    if (thisClass) {
                        return thisClass;
                    } else {
                        std::cerr << this->getPosition() << " Error: Cannot use 'this' identifier in program body" << std::endl;
                        return nullptr;
                    }
                } else {
                    ClassStruct *objType = obj_->getType(thisClass, thisMethod, failed);
                    if (objType) {
                        if (objType->fieldTable.find(name_) != objType->fieldTable.end()) {
                            return objType->fieldTable[name_].first;
                        } else {
                            std::cerr << this->getPosition() << " Error: Object of type \"" << objType->name << "\" does not contain the field \"" << name_ << "\"" << std::endl;
                            failed = true;
                            return nullptr;
                        }
                    } else {
                        return nullptr;
                    }
                }
            } else {
                return thisMethod->symbolTable[name_].first;
            }
        }
        bool isField() {
            return isField_;
        }
        bool isThis() {
            return !isField_ && name_ == "this";
        }
        bool isAssignable() {
            return (isField_ && obj_->isThis()) || (!isField_ && this->builtinIdents.find(name_) == this->builtinIdents.end());
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            std::string fieldName, varName;
            r_expr_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            
            if (l_expr_->isField()) { // LExpr is of the form "[RExpr].[Ident]"
                fieldName = l_expr_->getName();
                if (l_expr_->isAssignable()) { //LExpr is of the form "this.[Ident]"
                    if (std::find(fields.begin(), fields.end(), fieldName) == fields.end()) {
                        if (inConstructor) { 
                            fields.push_back(fieldName);
                        } else {
                            std::cerr << this->getPosition() << " Error: \"this." << fieldName << "\" used before initialization" << std::endl;
                            failed = true;
                        }
                    }
                } else {
                    std::cerr << this->getPosition() << " Error: Cannot assign to field \"" << fieldName << "\" outside of method" << std::endl;
                    failed = true;
                }
            } else {
                varName = l_expr_->getName();
                if (l_expr_->isAssignable()) {
                    if (std::find(init.begin(), init.end(), varName) == init.end()) {
                        init.push_back(varName);
                        table[varName] = std::pair<ClassStruct*, bool>(nullptr, false);
                    }
                } else {
                    std::cerr << this->getPosition() << " Error: Cannot assign to variable \"" << varName << "\"" << std::endl;
                    failed = true;
                }
            }
        }
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            std::string l_expr_name = l_expr_->getName();
            ClassStruct *r_expr_type = r_expr_->getType(thisClass, thisMethod, failed);
            ClassStruct *LCAType, *prevType;
            if (r_expr_type){
                if (l_expr_->isAssignable()) {
                    if (l_expr_->isField()) {
                        if (thisClass->fieldTable.find(l_expr_name) == thisClass->fieldTable.end()) {
                            std::cerr << this->getPosition() << " Error: Class \"" << thisClass->name << "\" does not contain the field \"" << l_expr_name << "\"" << std::endl;
                            failed = true;
                        } else {
                            prevType = thisClass->fieldTable[l_expr_name].first;
                            if (prevType == nullptr) {
                                thisClass->fieldTable[l_expr_name] = std::pair<ClassTable*, bool>(r_expr_type, false);
                                changed = true;
                            } else {
                                LCAType = prevType->LCA[r_expr_type];
                                if (LCAType != prevType) {
                                    if (thisClass->fieldTable[l_expr_name].second) {
                                        std::cerr << this->getPosition() << " Error: Field \"" << l_expr_name << "\" cannot be assigned to type \"" 
                                                << r_expr_type->name << "\" because it was given explicit type \"" << prevType->name << "\"" << std::endl;
                                    }
                                }
                            }
                        }
                    } else {

                    }
                } else {
                    std::cerr << this->getPosition() << " Error: Cannot assign to variable \"" << l_expr_name << "\"" << std::endl;
                    failed = true;
                }
            }
        }
    };
    
    class Return : public Statement {
        bool returnsNone_;
        RExpr *r_expr_;
    public:
        Return(RExpr *r_expr): returnsNone_(false), r_expr_(r_expr) {}
        Return(): returnsNone_(true), r_expr_(nullptr) {}
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            if (inConstructor) {
                std::cerr << this->getPosition() << " Error: Return statement not allowed in constructor" << std::endl;
                failed = true;
            }
            r_expr_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init, retFields, retInit;
            bool firstCase = true;
            expr_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            for (TypeAlt *alt : *alternatives_) {
                fieldsCopy = fields;
                initCopy = init;
                for (Statement *s : *alt->getStatements()) {
                    s->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
                }
                if (firstCase) {
                    firstCase = false;
                    retFields = fieldsCopy;
                    retInit = initCopy;
                }else{
                    retFields = intersection(fieldsCopy, retFields);
                    retInit = intersection(initCopy, retInit);
                }
            }
            fields = retFields;
            init = retInit;
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
   
    class IntLit : public RExpr {
        unsigned int val_;
    public:
        IntLit(unsigned int val): val_(val) {}
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class StrLit : public RExpr {
        std::string text_;
    public:
        StrLit(std::string text): text_(text) {}
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {}
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Call : public RExpr {
        RExpr *obj_;
        std::string mthd_;
        std::vector<RExpr*> *args_;
    public:
        Call(RExpr *obj, std::string mthd, std::vector<RExpr*> *args): obj_(obj), mthd_(mthd), args_(args) {}
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            obj_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            for (RExpr *arg : *args_) {
                if (inConstructor && arg->isThis()) {
                    std::cerr << this->getPosition() << " Error: Cannot pass 'this' as argument from constructor" << std::endl;
                    failed = true;
                }
                fieldsCopy = fields;
                initCopy = init;
                arg->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
                if (fieldsCopy.size() != fields.size()) {
                    for (auto field : difference(fieldsCopy, fields)) {
                        std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                    }
                    failed = true;
                }  
                if (initCopy.size() != init.size()) {
                    for (auto var : difference(initCopy, init)) {
                        std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy, initCopy;
            for (RExpr *arg : *args_) {
                if (inConstructor && arg->isThis()) {
                    std::cerr << this->getPosition() << " Error: Cannot pass 'this' as argument from constructor" << std::endl;
                    failed = true;
                }
                fieldsCopy = fields;
                initCopy = init;
                arg->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
                if (fieldsCopy.size() != fields.size()) {
                    for (auto field : difference(fieldsCopy, fields)) {
                        std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                    }
                    failed = true;
                }
                if (initCopy.size() != init.size()) {
                    for (auto var : difference(initCopy, init)) {
                        std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            lhs_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            fieldsCopy = fields;
            initCopy = init;
            rhs_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            lhs_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            fieldsCopy = fields;
            initCopy = init;
            rhs_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
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
        void getVars(std::vector<std::string> &fields, std::vector<std::string> &init, std::map<std::string, std::pair<ClassStruct*, bool>> &table, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldsCopy = fields, initCopy = init;
            expr_->getVars(fieldsCopy, initCopy, table, inConstructor, failed);
            if (fieldsCopy.size() != fields.size()) {
                for (auto field : difference(fieldsCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: \"this." << field << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
            if (initCopy.size() != init.size()) {
                for (auto var : difference(initCopy, init)) {
                    std::cerr << this->getPosition() << " Error: \"" << var << "\" used before initialization" << std::endl;
                }
                failed = true;
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
}
    
#endif