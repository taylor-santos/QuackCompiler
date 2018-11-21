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
template<class K, class V>
std::pair<std::map<K, V>, std::map<K, std::pair<V, V>>> map_intersection(std::map<K, V> a, std::map<K, V> b);
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
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) = 0;
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) = 0;
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
        void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            this->getType(thisClass, thisMethod, failed);
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) = 0;
    };   
    
    class If : public Statement {
        RExpr *cond_;
        std::vector<Statement*> *if_true_stmts_;
        std::vector<Statement*> *if_false_stmts_;
    public:
        If(RExpr *cond, std::vector<Statement*> *if_stmts, std::vector<Statement*> *else_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(else_stmts) {}
        If(RExpr *cond, std::vector<Statement*> *if_stmts): cond_(cond), if_true_stmts_(if_stmts), if_false_stmts_(new std::vector<Statement*>()) {}
        void set_else(std::vector<Statement*> *else_stmts){ delete this->if_false_stmts_; this->if_false_stmts_ = else_stmts; }
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> varCopy1 = vars, varCopy2, fieldCopy1 = fields, fieldCopy2;
            cond_->getVars(varCopy1, fields, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy1, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy1, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            fieldCopy1 = fields;
            varCopy1 = vars;
            for (Statement *s : *if_true_stmts_) {
                s->getVars(varCopy1, fieldCopy1, varTable, fieldTable, inConstructor, failed);
            }
            fieldCopy2 = fields;
            varCopy2 = vars;
            for (Statement *s : *if_false_stmts_) {
                s->getVars(varCopy2, fieldCopy2, varTable, fieldTable, inConstructor, failed);
            }
            fields = intersection(fieldCopy1, fieldCopy2);
            vars = intersection(varCopy1, varCopy2);
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
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            cond_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            fieldCopy = fields;
            varCopy = vars;
            for (Statement *s : *stmts_) {
                s->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
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
        bool isField() {
            return isField_;
        }
        bool isThis() {
            return !isField_ && name_ == "this";
        }
        bool isAssignable() {
            return (isField_ && obj_->isThis()) || (!isField_ && this->builtinIdents.find(name_) == this->builtinIdents.end() && this->name_ != "this");
        }
        std::string const getName(){ return name_; }
        std::string getName() const {
            return name_;
        }
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            if (isField_) {
                if (obj_->isThis()) {
                    if (std::find(fields.begin(), fields.end(), this->name_) == fields.end()) {
                        std::cerr << this->getPosition() << " Error: \"this." << this->name_ << "\" used before initialization" << std::endl;
                        failed = true;
                    }
                }
            } else {
                //Local variable
                if (std::find(vars.begin(), vars.end(), this->name_) == vars.end()) {
                    std::cerr << this->getPosition() << " Error: \"" << this->name_ << "\" used before initialization" << std::endl;
                    failed = true;
                }
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            ClassStruct *objType;
            if (isField_) {
                objType = obj_->getType(thisClass, thisMethod, failed);
                if (objType != nullptr) {
                    if (objType->fieldTable.find(this->name_) != objType->fieldTable.end()) {
                        return objType->fieldTable[this->name_].first;
                    } else {
                        std::cerr << this->getPosition() << " Error: \"" << this->name_ << "\" is not a field of class \"" << objType->name << "\"" << std::endl;
                        failed = true;
                    }
                }
            } else if (this->name_ == "this") { 
                return thisClass;
            } else if (this->builtinIdents.find(this->name_) != this->builtinIdents.end()) {
                return this->builtinIdents[this->name_];
            } else {
                return thisMethod->symbolTable[this->name_].first;
            }
            return nullptr;
        }
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
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            std::string fieldName, varName;
            r_expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            
            if (l_expr_->isField()) { // LExpr is of the form "[RExpr].[Ident]"
                fieldName = l_expr_->getName();
                if (l_expr_->isAssignable()) {
                    if (std::find(fields.begin(), fields.end(), fieldName) == fields.end()) {
                        fields.push_back(fieldName);
                    }
                    if (isTyped_) {
                        if (this->classTable.find(this->type_) != this->classTable.end()) {
                            if (fieldTable.find(fieldName) == fieldTable.end()) {
                                fieldTable[fieldName] = std::pair<ClassStruct*, bool>(this->classTable[this->type_], true);
                            } else if (fieldTable[fieldName].second) {
                                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" cannot have more than one explicit type definition" << std::endl;
                                failed = true;
                            } else {
                                fieldTable[fieldName] = std::pair<ClassStruct*, bool>(this->classTable[this->type_], true);
                            }
                        } else {
                            std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" given unrecognized explicit type \"" << this->type_ << "\"" << std::endl;
                            failed = true;
                        }
                    } else {
                        if (fieldTable.find(fieldName) == fieldTable.end()) {
                            fieldTable[fieldName] = std::pair<ClassStruct*, bool>(nullptr, false);
                        }
                    }
                } else {
                    std::cerr << this->getPosition() << " Error: Cannot assign to field \"" << fieldName << "\" outside of its class's methods" << std::endl;
                    failed = true;
                }
            } else {
                varName = l_expr_->getName();
                if (l_expr_->isAssignable()) {
                    if (std::find(vars.begin(), vars.end(), varName) == vars.end()) {
                        vars.push_back(varName);
                    }
                    if (isTyped_) {
                        if (this->classTable.find(this->type_) != this->classTable.end()) {
                            if (varTable.find(varName) == varTable.end()) {
                                varTable[varName] = std::pair<ClassStruct*, bool>(this->classTable[this->type_], true);
                            } else if (varTable[varName].second) {
                                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" cannot have more than one explicit type definition" << std::endl;
                                failed = true;
                            } else {
                                varTable[varName] = std::pair<ClassStruct*, bool>(this->classTable[this->type_], true);
                            }
                        } else {
                            std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" given unrecognized explicit type \"" << this->type_ << "\"" << std::endl;
                            failed = true;
                        }
                    } else {
                        if (varTable.find(varName) == varTable.end()) {
                            varTable[varName] = std::pair<ClassStruct*, bool>(nullptr, false);
                        }
                    }
                } else {
                    std::cerr << this->getPosition() << " Error: Cannot assign to protected variable \"" << varName << "\"" << std::endl;
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
                                thisClass->fieldTable[l_expr_name] = std::pair<ClassStruct*, bool>(r_expr_type, false);
                                changed = true;
                            } else {
                                LCAType = prevType->LCA[r_expr_type];
                                if (LCAType != prevType) {
                                    if (thisClass->fieldTable[l_expr_name].second) {
                                        std::cerr << this->getPosition() << " Error: Field \"" << l_expr_name << "\" cannot be assigned to type \"" 
                                                << r_expr_type->name << "\" because it is not a subtype of its explicit type \"" << prevType->name << "\"" << std::endl;
                                        failed = true;
                                    } else {
                                        thisClass->fieldTable[l_expr_name] = std::pair<ClassStruct*, bool>(LCAType, false);
                                        changed = true;
                                    }
                                }
                            }
                        }
                    } else {
                        if (thisMethod->symbolTable.find(l_expr_name) == thisMethod->symbolTable.end()) {
                            std::cerr << this->getPosition() << " Error: Variable \"" << l_expr_name << "\" does not exist in the current context" << std::endl;
                            failed = true;
                        } else {
                            prevType = thisMethod->symbolTable[l_expr_name].first;
                            if (prevType == nullptr) {
                                thisMethod->symbolTable[l_expr_name] = std::pair<ClassStruct*, bool>(r_expr_type, false);
                                changed = true;
                            } else {
                                LCAType = prevType->LCA[r_expr_type];
                                if (LCAType != prevType) {
                                    if (thisMethod->symbolTable[l_expr_name].second) {
                                        std::cerr << this->getPosition() << " Error: Variable \"" << l_expr_name << "\" cannot be assigned to type \"" 
                                                << r_expr_type->name << "\" because it is not a subtype of its explicit type \"" << prevType->name << "\"" << std::endl;
                                        failed = true;
                                    } else {
                                        thisMethod->symbolTable[l_expr_name] = std::pair<ClassStruct*, bool>(LCAType, false);
                                        changed = true;
                                    }
                                }
                            }
                        }
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
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            if (inConstructor) {
                std::cerr << this->getPosition() << " Error: Return statement not allowed in constructor" << std::endl;
                failed = true;
            }
            r_expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
        }
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            if (!returnsNone_) {
                r_expr_->updateTypes(thisClass, thisMethod, changed, failed);
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
        std::string getName() const { return name_; }
        std::string getType() const { return type_; }
        std::vector<Statement*>* getStatements() const {
            return stmts_;
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
    
    class Typecase : public Statement {
        RExpr *expr_;
        std::vector<TypeAlt*> *alternatives_;
    public:
        Typecase(RExpr *expr, std::vector<TypeAlt*> *alternatives): expr_(expr), alternatives_(alternatives) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            std::string altName, altType;
            expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (TypeAlt *alt : *alternatives_) {
                altName = alt->getName();
                if (varTable.find(altName) != varTable.end()) {
                    std::cerr << this->getPosition() << " Error: Typecase cannot introduce the variable \"" << altName << "\" because it already exists in this scope" << std::endl;
                    failed = true;
                } else {
                    fieldCopy = fields;
                    varCopy = vars;
                    altType = alt->getType();
                    if (this->classTable.find(altType) == this->classTable.end()) {
                        std::cerr << this->getPosition() << " Error: Typecase cannot introduce variable of unrecognized type \"" << altType << "\"" << std::endl;
                        failed = true;
                    } else {
                        varTable[altName] = std::pair<ClassStruct*, bool>(this->classTable[altType], true);
                        for (Statement *s : *alt->getStatements()) {
                            s->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
                        }
                    }
                }
            }
        }
        virtual void updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod, bool &changed, bool &failed) {
            expr_->updateTypes(thisClass, thisMethod, changed, failed);
            for (TypeAlt *alt : *alternatives_) {
                for (Statement *s : *alt->getStatements()) {
                    s->updateTypes(thisClass, thisMethod, changed, failed);
                }
            }
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
   
    class IntLit : public RExpr {
        unsigned int val_;
    public:
        IntLit(unsigned int val): val_(val) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) { }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            return this->builtinTypes["Int"];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class StrLit : public RExpr {
        std::string text_;
    public:
        StrLit(std::string text): text_(text) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) { }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            return this->builtinTypes["String"];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Call : public RExpr {
        RExpr *obj_;
        std::string mthd_;
        std::vector<RExpr*> *args_;
    public:
        Call(RExpr *obj, std::string mthd, std::vector<RExpr*> *args): obj_(obj), mthd_(mthd), args_(args) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            obj_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (RExpr *arg : *args_) {
                if (inConstructor && arg->isThis()) {
                    std::cerr << this->getPosition() << " Error: Cannot pass 'this' as argument from constructor" << std::endl;
                    failed = true;
                }
                fieldCopy = fields;
                varCopy = vars;
                arg->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
                for (auto varName : difference(varCopy, vars)) {
                    std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                    failed = true;
                }
                for (auto fieldName : difference(fieldCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                    failed = true;
                }
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            std::vector<ClassStruct*> argTypes;
            size_t i;
            ClassStruct *actualArgType, *LCAType, *objType;
            bool allArgs = true;
            objType = obj_->getType(thisClass, thisMethod, failed);
            if (objType != nullptr) {
                if (objType->methodTable.find(mthd_) == objType->methodTable.end()) {
                    std::cerr << this->getPosition() << " Error: \"" << mthd_ << "()\" is not a method of class \"" << objType->name << "\"" << std::endl;
                    failed = true;
                }else{
                    argTypes = objType->methodTable[mthd_]->argTypes;
                    if (args_->size() != argTypes.size()) {
                        std::cerr << this->getPosition() << " Error: Method \"" << mthd_ << "()\" of class \"" << objType->name << "\" takes " << argTypes.size() << " arguments, not " << args_->size() << std::endl;
                        failed = true;
                    } else {
                        for (i=0; i<args_->size(); i++) {
                            actualArgType = (*args_)[i]->getType(thisClass, thisMethod, failed);
                            if (actualArgType != nullptr) {
                                LCAType = actualArgType->LCA[argTypes[i]];
                                if (LCAType != argTypes[i]) {
                                    std::cerr << this->getPosition() << " Error: Argument " << i+1 << " of method \"" << this->mthd_ << "()\" of class \"" << objType->name 
                                            << "\" has type \"" << actualArgType->name 
                                            << "\" which is not a subtype of the required type \"" << argTypes[i]->name << "\"" << std::endl;
                                    failed = true;
                                    allArgs = false;
                                }
                            } else {
                                allArgs = false;
                            }
                        }
                        if (allArgs) {
                            return objType->methodTable[mthd_]->type;
                        }
                    }   
                }
            }
            return nullptr;
        }            
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Constructor : public RExpr {
        std::string name_;
        std::vector<RExpr*> *args_;
    public:
        Constructor(std::string name, std::vector<RExpr*> *args): name_(name), args_(args) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy, varCopy;
            if (this->classTable.find(name_) == this->classTable.end()) {
                std::cerr << this->getPosition() << " Error: \"" << this->name_ << " \" constructor is for an unrecognized class" << std::endl;
                failed = true;
            }
            for (RExpr *arg : *args_) {
                if (inConstructor && arg->isThis()) {
                    std::cerr << this->getPosition() << " Error: Cannot pass 'this' as argument from constructor" << std::endl;
                    failed = true;
                }
                fieldCopy = fields;
                varCopy = vars;
                arg->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
                for (auto varName : difference(varCopy, vars)) {
                    std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                    failed = true;
                }
                for (auto fieldName : difference(fieldCopy, fields)) {
                    std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                    failed = true;
                }
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            std::vector<ClassStruct*> argTypes = this->classTable[name_]->constructor->argTypes;
            size_t i;
            ClassStruct *actualArgType, *LCAType;
            if (args_->size() != argTypes.size()) {
                std::cerr << this->getPosition() << " Error: Constructor for class \"" << name_ << "\" takes " << argTypes.size() << " arguments, not " << args_->size() << std::endl;
                failed = true;
            } else {
                for (i=0; i<args_->size(); i++) {
                    actualArgType = (*args_)[i]->getType(thisClass, thisMethod, failed);
                    if (actualArgType != nullptr) {
                        LCAType = actualArgType->LCA[argTypes[i]];
                        if (LCAType != argTypes[i]) {
                            std::cerr << this->getPosition() << " Error: Argument " << i+1 << " of \"" << this->name_ 
                                    << "\" constructor call has type \"" << actualArgType->name 
                                    << "\" which is not a subtype of the required type \"" << argTypes[i]->name << "\"" << std::endl;
                            failed = true;
                        }
                    }
                }
            }
            return this->classTable[this->name_];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class And : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        And(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            lhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            fieldCopy = fields;
            varCopy = vars;
            rhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            ClassStruct *exprType;
            exprType = lhs_->getType(thisClass, thisMethod, failed);
            if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: Left-hand expression of 'and' is not a Boolean" << std::endl;
                failed = true;
            }
            exprType = rhs_->getType(thisClass, thisMethod, failed);
            if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: Right-hand expression of 'and' is not a Boolean" << std::endl;
                failed = true;
            }
            return this->builtinTypes["Boolean"];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Or : public RExpr {
        RExpr *lhs_;
        RExpr *rhs_;
    public:
        Or(RExpr *lhs, RExpr *rhs): lhs_(lhs), rhs_(rhs) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            lhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
            fieldCopy = fields;
            varCopy = vars;
            rhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            ClassStruct *exprType;
            exprType = lhs_->getType(thisClass, thisMethod, failed);
            if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: Left-hand expression of 'or' is not a Boolean" << std::endl;
                failed = true;
            }
            exprType = rhs_->getType(thisClass, thisMethod, failed);
            if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: Right-hand expression of 'or' is not a Boolean" << std::endl;
                failed = true;
            }
            return this->builtinTypes["Boolean"];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };

    class Not : public RExpr {
        RExpr *expr_;
    public:
        Not(RExpr *expr): expr_(expr) {}
        virtual void getVars(std::vector<std::string> &vars, std::vector<std::string> &fields, std::map<std::string, std::pair<ClassStruct*, bool>> &varTable, std::map<std::string, std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor, bool &failed) {
            std::vector<std::string> fieldCopy = fields, varCopy = vars;
            expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << " Error: Variable \"" << varName << "\" used before initialization" << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << " Error: Field \"this." << fieldName << "\" used before initialization" << std::endl;
                failed = true;
            }
        }
        virtual ClassStruct *getType(ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
            ClassStruct *exprType;
            exprType = expr_->getType(thisClass, thisMethod, failed);
            if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
                std::cerr << this->getPosition() << " Error: Expression right of 'not' is not a Boolean" << std::endl;
                failed = true;
            }
            return this->builtinTypes["Boolean"];
        }
        void json(std::ostream &out, unsigned int indent = 0);
    };
}
    
#endif