#include "include/AST.h"

template<class T>
std::vector<T> difference(std::vector<T> a, std::vector<T> b) {
    std::vector<T> ret(a.size() + b.size());
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    auto it = std::set_difference(a.begin(), a.end(), b.begin(), b.end(),
            ret.begin());
    ret.resize(it-ret.begin());
    return ret;
}
template<class T>
std::vector<T> intersection(std::vector<T> a, std::vector<T> b) {
    std::vector<T> ret(a.size() + b.size());
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    auto it = std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
            ret.begin());
    ret.resize(it-ret.begin());
    return ret;
}
template<class K, class V>
std::pair<std::map<K, V>, std::map<K, std::pair<V, V>>> map_intersection(
        std::map<K, V> a, std::map<K, V> b) {
    std::map<K, V> same;
    std::map<K, std::pair<V, V>> diff;
    for (auto kv : a) {
        if (b.find(kv.first) != b.end()) {
            if (kv.second == b[kv.first]) {
                same[kv.first] = kv.second;
            } else {
                diff[kv.first] = std::make_pair(kv.second, b[kv.first]);
            }
        }
    }
    return std::make_pair(same, diff);
}
template<class K, class V>
std::map<K, V> map_difference(std::map<K, V> a, std::map<K, V> b) {
    std::map<K, V> diff;
    for (auto kv : a) {
        if (b.find(kv.first) == b.end()) {
            diff[kv.first] = kv.second;
        }
    }
    return diff;
}

namespace AST {
/* ASTNode */
    std::map<std::string, ClassStruct*> ASTNode::classTable;
    std::map<std::string, ClassStruct*> ASTNode::builtinTypes;
    std::map<std::string, ClassStruct*> ASTNode::builtinIdents;
    unsigned int Statement::tempVarID = 0;
    unsigned int Statement::labelID = 0;
    std::string ASTNode::getPosition() const {
        return std::to_string(first_line) + ":" +
                std::to_string(first_col) + ": ";
    }
    void ASTNode::set_position(int first_line, int first_col, int last_line,
            int last_col, std::string *filename) {
        this->first_line = first_line;
        this->first_col = first_col;
        this->last_line = last_line;
        this->last_col = last_col;
        if (filename) {
            this->filename = *filename;
        }
    }
    void ASTNode::json_indent(std::ostream& out, unsigned int indent) {
        for (unsigned int i = 0; i < indent; i++) {
                out << "  ";
        }
    }
    void ASTNode::json_head(std::ostream& out, unsigned int &indent,
            std::string const &node_kind) {
        out << "{" << std::endl;
        indent++;
        this->json_indent(out, indent);
        out << "\"kind\": \"" << node_kind << "\"";
    }
    void ASTNode::json_close(std::ostream& out, unsigned int &indent) {
        if (indent > 0) indent--;
        out << std::endl;
        this->json_indent(out, indent);
        out << "}";
    }
    void ASTNode::json_child(std::ostream& out, unsigned int indent,
            std::string const &field, ASTNode *child) {
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": ";
        child->json(out, indent);
    }

    void ASTNode::json_string(std::ostream& out, unsigned int indent,
            std::string const &field, std::string const &str) {
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": \"" << str << "\"";
    }
    void ASTNode::json_int(std::ostream& out, unsigned int indent,
            std::string const &field, unsigned int val) {
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": " << val;
    }
    template<class NodeType>
    void ASTNode::json_list(std::ostream& out, unsigned int indent,
            std::string const &field, std::vector<NodeType> const& list) {
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": [";
        std::string sep = "";
        for (NodeType node : list) {
                out << sep << std::endl;
                this->json_indent(out, indent+1);
                node->json(out, indent+1);
                sep = ",";
        }
        if (list.size() > 0) {
            out << std::endl;
            this->json_indent(out, indent);
        }
        out << "]";
    }
    ClassStruct *ASTNode::addBuiltinType(std::string name, ClassStruct* super) {
        ClassStruct *cs = new ClassStruct({name, super});
        if (super) {
            super->children.push_back(cs);
        }
        cs->constructor = new MethodStruct({name, cs, cs});
        this->builtinTypes[name] = cs;
        this->classTable[name] = cs;
        return cs;
    }
    void ASTNode::addBuiltinIdent(std::string name, ClassStruct *type) {
        builtinIdents[name] = type;
    }
    MethodStruct *ASTNode::addBuiltinMethod(ClassStruct *clazz,
            std::string name, ClassStruct *type,
            std::vector<ClassStruct*> const &argTypes) {
        MethodStruct *ms = new MethodStruct({name, clazz, type, argTypes});
        clazz->methodTable[name] = ms;
        return ms;
    }

/* TypedArg */
    void TypedArg::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "TypedArg");
        this->json_string(out, indent, "name_", this->name_);
        this->json_string(out, indent, "type_", this->type_);
        this->json_close(out, indent);
    }

/* Statement */
    std::pair<ClassStruct*, bool> Statement::getReturnType(
        ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
        return std::make_pair(nullptr, false);
    }

/* Method */
    void Method::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Method");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<TypedArg*>(out, indent, "args_", *this->args_);
        if (this->isTyped_) {
            this->json_string(out, indent, "type_", this->type_);
        }
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    std::string Method::getType() const {
        if (isTyped_) {
            return type_;
        } else {
            return "Nothing";
        }
    }
    std::vector<std::string> Method::getArgNames() const {
        std::vector<std::string> vec(this->args_->size());
        for (size_t i = 0; i < this->args_->size(); i++) {
            vec[i] = this->args_->at(i)->getName();
        }
        return vec;
    }

/* Class */
    void Class::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Class");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<TypedArg*>(out, indent, "args_", *this->args_);
        this->json_string(out, indent, "extends_", this->extends_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_list<Method*>(out, indent, "mthds_", *this->mthds_);
        this->json_close(out, indent);
    }
    std::vector<std::string> Class::getArgNames() const {
        std::vector<std::string> vec(this->args_->size());
        for (size_t i = 0; i < this->args_->size(); i++) {
            vec[i] = this->args_->at(i)->getName();
        }
        return vec;
    }

/* Program */
    void Program::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Program");
        this->json_list<Class*>(out, indent, "classes_", *this->classes_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void Program::addBuiltins() {
        ClassStruct *Obj     = this->addBuiltinType("Obj", nullptr);
        ClassStruct *Nothing = this->addBuiltinType("Nothing", Obj);
        ClassStruct *Int     = this->addBuiltinType("Int", Obj);
        ClassStruct *String  = this->addBuiltinType("String", Obj);
        ClassStruct *Boolean = this->addBuiltinType("Boolean", Obj);
        this->addBuiltinIdent("none",  Nothing);
        this->addBuiltinIdent("true",  Boolean);
        this->addBuiltinIdent("false", Boolean);
        this->addBuiltinMethod(Obj, "STR", String,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(Obj, "PRINT",  Nothing,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(Obj, "EQUALS", Boolean,
                std::vector<ClassStruct*>(1, Obj));
        this->addBuiltinMethod(Nothing, "STR", String,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(String, "STR", String,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(String, "PLUS", String,
                std::vector<ClassStruct*>(1, String));
        this->addBuiltinMethod(String, "EQUALS", Boolean,
                std::vector<ClassStruct*>(1, Obj));
        this->addBuiltinMethod(String, "ATLEAST", Boolean,
                std::vector<ClassStruct*>(1, String));
        this->addBuiltinMethod(String, "ATMOST", Boolean,
                std::vector<ClassStruct*>(1, String));
        this->addBuiltinMethod(String, "LESS", Boolean,
                std::vector<ClassStruct*>(1, String));
        this->addBuiltinMethod(String, "MORE", Boolean,
                std::vector<ClassStruct*>(1, String));
        this->addBuiltinMethod(Int, "STR", String,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(Int, "PLUS",   Int,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "MINUS",  Int,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "NEGATE", Int,
                std::vector<ClassStruct*>());
        this->addBuiltinMethod(Int, "TIMES",  Int,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "DIVIDE", Int,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "EQUALS", Boolean,
                std::vector<ClassStruct*>(1, Obj));
        this->addBuiltinMethod(Int, "ATLEAST", Boolean,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "ATMOST", Boolean,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "LESS", Boolean,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Int, "MORE", Boolean,
                std::vector<ClassStruct*>(1, Int));
        this->addBuiltinMethod(Boolean, "STR", String,
                std::vector<ClassStruct*>());
        
    }
    void Program::buildClassMap(bool &failed) {
        std::string name, super;
        ClassStruct *cs;
        // Get class declarations
        for (Class *c : *this->classes_) {
            name = c->getName();
            if (this->builtinIdents.find(name) != this->builtinIdents.end() ||
                    name == "this") {
                std::cerr << "Error: Class \"" << name << "\" cannot share the "
                        << "same identifier as builtin instance \"" << name
                        << "\"" << std::endl;
                failed = true;
            } else if (classTable.find(name) == classTable.end()) {
                cs = new ClassStruct({name, nullptr});
                classTable[name] = cs;
                c->setClassStruct(cs);
            } else {
                std::cerr << c->getPosition()
                        << "Error: Redefinition of class \"" << name << "\""
                        << std::endl;
                failed = true;
            }
        }
        // Find class inheritance
        if (!failed) {
            for (Class *c : *this->classes_) {
                name = c->getName();
                super = c->getExtends();
                if (classTable.find(super) == classTable.end()) {
                    std::cerr << c->getPosition() << "Error: Class \"" << name
                            << "\" extends unrecognized type \"" << super
                            << "\"" << std::endl;
                    failed = true;
                } else if (this->builtinTypes.find(super) !=
                        this->builtinTypes.end() && super != "Obj") {
                    std::cerr << c->getPosition() << "Error: Class \"" << name
                            << "\" cannot derive from builtin type \"" << super
                            << "\"" << std::endl;
                    failed = true;
                } else {
                    c->getClassStruct()->super = classTable[super];
                    classTable[super]->children.push_back(c->getClassStruct());
                }
            }
        }
    }
    void Program::checkClassHierarchy(bool &failed) {
        ClassStruct *curr1, *curr2;
        std::map<ClassStruct*, bool> noCycles;

        buildClassMap(failed);
        for (auto it : this->classTable) {
            ClassStruct *cs = it.second;
            if (!cs->super) {
                noCycles[cs] = true;
            } else {
                noCycles[cs] = false;
            }
        }
        for (auto it : this->classTable) {
            ClassStruct *cs = it.second;
            bool foundCycle = false;
            curr1 = cs;
            if (noCycles[curr1])
                continue;
            curr2 = curr1->super;
            while (!noCycles[curr1] && !noCycles[curr2]) {
                if (curr1 == curr2) {
                    std::cerr << "Error: Class hierarchy contains a cycle "
                            << "involving class \"" << curr1->name << "\""
                            << std::endl;
                    failed = true;
                    foundCycle = true;
                    break;
                }
                if (curr1->super) {
                    curr1 = curr1->super;
                } else {
                    break;
                }
                if (curr2->super && curr2->super->super) {
                    curr2 = curr2->super->super;
                } else {
                    break;
                }
            }
            if (!foundCycle) {
                curr1 = cs;
                while (!noCycles[curr1]) {
                    noCycles[curr1] = true;
                    if (curr1->super) {
                        curr1 = curr1->super;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    void Program::buildLCAs(bool &failed) {
        std::map<ClassStruct*, std::vector<ClassStruct*>> paths;
        ClassStruct *curr;
        for (auto it : this->classTable) {
            curr = it.second;
            while (curr != nullptr) {
                paths[it.second].push_back(curr);
                curr = curr->super;
            }
        }
        for (auto it1 = paths.begin(); it1 != paths.end(); it1++) {
            for (auto it2 = it1; it2 != paths.end(); it2++) {
                auto curr1 = it1->second.rbegin();
                auto curr2 = it2->second.rbegin();
                auto prev = curr1;
                while (*curr1 == *curr2 &&
                        curr1 != it1->second.rend() &&
                        curr2 != it2->second.rend()) {
                    prev = curr1;
                    curr1++;
                    curr2++;
                }
                it1->first->LCA[it2->first] = *prev;
                it2->first->LCA[it1->first] = *prev;
            }
        }
    }
    void Program::buildMethodTables(bool &failed) {
        ClassStruct *argType;
        MethodStruct *ms;
        std::string methodName, argTypeName;
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            for (Method *m : *c->getMethods()) {
                methodName = m->getName();
                if (this->classTable.find(methodName) !=
                        this->classTable.end()) {
                    std::cerr << "Error: Method \"" << cs->name << "."
                            << methodName << "()\" shares the same identifier "
                            << "as class \"" << methodName << "\"" << std::endl;
                    failed = true;
                }
                if (cs->methodTable.find(methodName) != cs->methodTable.end()) {
                    std::cerr << m->getPosition()
                            << "Error: Redefinition of method \"" << cs->name
                            << "." << methodName << "\"" << std::endl;
                    failed = true;
                } else {
                    ms = new MethodStruct({m->getName(), cs});
                    for (TypedArg *arg : *m->getArgs()) {
                        argTypeName = arg->getType();
                        if (this->classTable.find(argTypeName) ==
                                this->classTable.end()) {
                            std::cerr << arg->getPosition()
                                    << "Error: Argument \"" << arg->getName()
                                    << "\" of method \"" << cs->name << "."
                                    << ms->name << "()\" has unrecognized type "
                                    << "\"" << argTypeName << "\"" << std::endl;
                            failed = true;
                        } else {
                            argType = this->classTable[arg->getType()];
                            ms->argTypes.push_back(argType);
                            ms->symbolTable[arg->getName()] =
                                    std::make_pair(argType, true);
                        }
                    }
                    for (auto it : this->builtinIdents) {
                        ms->symbolTable[it.first] =
                            std::make_pair(it.second, true);
                    }
                    ms->type = this->classTable[m->getType()];
                    cs->methodTable[methodName] = ms;
                    m->setMethodStruct(ms);
                }
            }
            cs->constructor = new MethodStruct({cs->name, cs, cs});
            for (TypedArg *arg : *c->getArgs()) {
                argTypeName = arg->getType();
                if (this->classTable.find(argTypeName) ==
                        this->classTable.end()) {
                    std::cerr << arg->getPosition() << "Error: Argument \""
                            << arg->getName()
                            << "\" of class \"" << cs->name
                            << "\" constructor has unrecognized type \""
                            << argTypeName << "\"" << std::endl;
                    failed = true;
                } else {
                    argType = this->classTable[arg->getType()];
                    cs->constructor->argTypes.push_back(argType);
                    cs->constructor->symbolTable[arg->getName()] =
                            std::make_pair(argType, true);
                }
            }
            for (auto it : this->builtinIdents) {
                cs->constructor->symbolTable[it.first] =
                        std::make_pair(it.second, true);
            }
        }
        this->body_ = new MethodStruct({"Body", nullptr, nullptr});
        for (auto it : this->builtinIdents) {
            this->body_->symbolTable[it.first] =
                    std::make_pair(it.second, true);
        }
    }
    void Program::checkMethodInheritance(bool &failed) {
        std::map<ClassStruct*, bool> checked;
        std::stack<ClassStruct*> classStack;
        ClassStruct *curr;
        MethodStruct *ms;
        size_t i;
        for (auto it : this->classTable) {
            if (it.second->super == nullptr) {
                for (auto mthdKeyValue : it.second->methodTable) {
                    it.second->methodOrder.push_back(mthdKeyValue.second);
                }
                checked[it.second] = true;
            } else {
                checked[it.second] = false;
            }
        }
        for (auto it : this->classTable) {
            curr = it.second;
            while (!checked[curr]) {
                classStack.push(curr);
                curr = curr->super;
            }
            while (!classStack.empty()) {
                curr = classStack.top();
                classStack.pop();
                for (MethodStruct *super_ms : curr->super->methodOrder) {
                    if (curr->methodTable.find(super_ms->name) !=
                            curr->methodTable.end()) {
                        ms = curr->methodTable[super_ms->name];
                        if (ms->type->LCA[super_ms->type] != super_ms->type) {
                            std::cerr << "Error: Inherited method \""
                                    << curr->name << "." << ms->name
                                    << "()\" does not return a subtype of its "
                                    << "overridden method" << std::endl;
                            failed = true;
                        }
                        if (ms->argTypes.size() != super_ms->argTypes.size()) {
                            std::cerr << "Error: Inherited method \""
                                    << curr->name << "." << ms->name << "()\" "
                                    << "does not share the same number of "
                                    << "arguments as its overridden method"
                                    << std::endl;
                            failed = true;
                        } else {
                            for (i = 0; i < ms->argTypes.size(); i++) {
                                if (ms->argTypes[i]->LCA[super_ms->argTypes[i]]
                                        != ms->argTypes[i]) {
                                    std::cerr << "Error: Argument " << i+1 <<
                                            " of inherited method \""
                                            << curr->name << "." << ms->name
                                            << "()\" is not a supertype of its "
                                            << "overridden argument"
                                            << std::endl;
                                    failed = true;
                                }
                            }
                        }
                        curr->methodOrder.push_back(
                                curr->methodTable[super_ms->name]);
                    } else {
                        curr->methodTable[super_ms->name] = super_ms;
                        curr->methodOrder.push_back(super_ms);
                    }
                }
                for (auto mthdKeyValue : curr->methodTable) {
                    if (std::find(curr->methodOrder.begin(),
                            curr->methodOrder.end(), mthdKeyValue.second) ==
                            curr->methodOrder.end()) {
                        curr->methodOrder.push_back(mthdKeyValue.second);
                    }
                }
                checked[curr] = true;
            }
        }
    }
    void Program::getVars(bool &failed) {
        std::vector<std::string> fields, fieldCopy, vars;
        std::map<std::string, std::pair<ClassStruct*, bool>> fieldTable;
        MethodStruct *ms;
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            cs->constructor->symbolTable["this"] = std::make_pair(cs, true);
            std::transform(
                cs->constructor->symbolTable.begin(),
                cs->constructor->symbolTable.end(),
                std::back_inserter(vars),
                [](std::pair<std::string, std::pair<ClassStruct*, bool>> v)
                { return v.first; });
            for (Statement *s : *c->getStatements()) {
                s->getVars(vars, fields, cs->constructor->symbolTable,
                        fieldTable, true, failed);
            }
            for (auto symbolKeyValue : cs->constructor->symbolTable) {
                if (this->classTable.find(symbolKeyValue.first) !=
                        this->classTable.end()) {
                    std::cerr << "Error: Local variable \""
                            << symbolKeyValue.first << "\" in \""
                            << cs->name << "\" constructor cannot share the "
                            << "same identifier as class \""
                            << symbolKeyValue.first << "\"" << std::endl;
                    failed = true;
                }
                if (cs->methodTable.find(symbolKeyValue.first) !=
                        cs->methodTable.end()) {
                    std::cerr << "Error: Local variable \""
                            << symbolKeyValue.first << "\" in \""
                            << cs->name << "\" constructor cannot share the "
                            << "same identifier as method \"" << cs->name
                            << "." << symbolKeyValue.first << "()\""
                            << std::endl;
                    failed = true;
                }
            }
            for (std::string field : fields) {
                if (this->classTable.find(field) != this->classTable.end()) {
                    std::cerr << "Error: Field \"" << cs->name << "."
                            << field << "\" cannot share the same identifier "
                            << "as class \"" << field << "\"" << std::endl;
                    failed = true;
                }
                if (cs->methodTable.find(field) !=
                        cs->methodTable.end()) {
                    std::cerr << "Error: Field \"" << cs->name << "." << field
                            << "\" in \"" << cs->name << "\" cannot share the "
                            << "same identifier as method \"" << cs->name
                            << "." << field << "()\"" << std::endl;
                    failed = true;
                }
                cs->fieldTable[field] = fieldTable[field];
            }
            vars.clear();
            fieldTable.clear();
            for (Method *m : *c->getMethods()) {
                ms = m->getMethodStruct();
                ms->symbolTable["this"] = std::make_pair(cs, true);
                std::transform(
                    ms->symbolTable.begin(),
                    ms->symbolTable.end(),
                    std::back_inserter(vars),
                    [](std::pair<std::string, std::pair<ClassStruct*, bool>> v)
                    { return v.first; });
                for (Statement *s : *m->getStatements()) {
                    fieldCopy = fields;
                    s->getVars(vars, fieldCopy, ms->symbolTable, fieldTable,
                            false, failed);
                }
                for (auto symbolKeyValue : ms->symbolTable) {
                    if (this->classTable.find(symbolKeyValue.first) !=
                            this->classTable.end()) {
                        std::cerr << "Error: Local variable \""
                                << symbolKeyValue.first << "\" in \""
                                << cs->name << "." << ms->name << "()\" "
                                << "cannot share the same identifier as class "
                                << "\"" << symbolKeyValue.first << "\""
                                << std::endl;
                        failed = true;
                    }
                    if (cs->methodTable.find(symbolKeyValue.first) !=
                            cs->methodTable.end()) {
                        std::cerr << "Error: Local variable \""
                                << symbolKeyValue.first << " in " << cs->name
                                << "." << ms->name << "()\" cannot "
                                << "share the same identifier as method \""
                                << cs->name << "." << symbolKeyValue.first
                                << "()\"" << std::endl;
                        failed = true;
                    }
                }
                vars.clear();
                fieldTable.clear();
            }
            fields.clear();
        }
        std::transform(
            this->body_->symbolTable.begin(),
            this->body_->symbolTable.end(),
            std::back_inserter(vars),
            [](std::pair<std::string, std::pair<ClassStruct*, bool>> v)
            { return v.first; });
        for (Statement *s : *this->stmts_) {
            s->getVars(vars, fields, this->body_->symbolTable, fieldTable,
                    false, failed);
        }
        for (auto symbolKeyValue : this->body_->symbolTable) {
            if (this->classTable.find(symbolKeyValue.first) !=
                    this->classTable.end()) {
                std::cerr << "Error: Local variable \""
                        << symbolKeyValue.first << "\" in program body cannot "
                        << "share the same identifier as class "
                        << "\"" << symbolKeyValue.first << "\""
                        << std::endl;
                failed = true;
            }
        }
    }
    void Program::inferTypes(bool &failed) {
        bool changed;
        ClassStruct *cs;
        MethodStruct *ms;
        do {
            changed = false;
            for (Class *c : *this->classes_) {
                cs = c->getClassStruct();
                for (Statement *s : *c->getStatements()) {
                    s->updateTypes(cs, cs->constructor, changed, failed);
                }
                for (Method *m : *c->getMethods()) {
                    ms = m->getMethodStruct();
                    for (Statement *s : *m->getStatements()) {
                        s->updateTypes(cs, ms, changed, failed);
                    }
                }
            }
            for (Statement *s : *this->stmts_) {
                s->updateTypes(nullptr, this->body_, changed, failed);
            }
        } while (changed && !failed);
        for (Class *c : *this->classes_) {
            cs = c->getClassStruct();
            for (auto it : cs->fieldTable) {
                if (it.second.first == nullptr) {
                    std::cerr << "Error: Type of field \"" << cs->name << "."
                            << it.first << "\" could not be inferred"
                            << std::endl;
                    failed = true;
                }
            }
            for (Method *m : *c->getMethods()) {
                ms = m->getMethodStruct();
                for (auto it : ms->symbolTable) {
                    if (it.second.first == nullptr) {
                        std::cerr << "Error: Type of variable \"" << it.first
                                << "\" in method \"" << cs->name << "."
                                << ms->name << "()\" could not be inferred"
                                << std::endl;
                        failed = true;
                    }
                }
            }
        }
        for (auto it : this->body_->symbolTable) {
            if (it.second.first == nullptr) {
                std::cerr << "Error: Type of variable \"" << it.first
                        << "\" in program body could not be inferred"
                        << std::endl;
                failed = true;
            }
        }
    }
    void Program::checkFieldInheritance(bool &failed) {
        std::map<ClassStruct*, bool> checked;
        std::stack<ClassStruct*> classStack;
        ClassStruct *curr;
        for (auto it : this->classTable) {
            if (it.second->super == nullptr) {
                checked[it.second] = true;
            } else {
                checked[it.second] = false;
            }
        }
        for (auto it : this->classTable) {
            curr = it.second;
            while (!checked[curr]) {
                classStack.push(curr);
                curr = curr->super;
            }
            while (!classStack.empty()) {
                curr = classStack.top();
                classStack.pop();
                for (auto fieldKeyValue : curr->super->fieldTable) {
                    if (curr->fieldTable.find(fieldKeyValue.first) ==
                            curr->fieldTable.end()) {
                        std::cerr << "Error: Class \"" << curr->name << "\""
                                << " does not initialize field \""
                                << fieldKeyValue.first << "\" inherited from "
                                << "parent class \"" << curr->super->name
                                << "\"" << std::endl;
                        failed = true;
                    } else {
                        ClassStruct *prevType = fieldKeyValue.second.first;
                        ClassStruct *newType =
                                curr->fieldTable[fieldKeyValue.first].first;
                        if (newType->LCA[prevType] != prevType) {
                            std::cerr << "Error: Field \""
                                    << fieldKeyValue.first << "\" of class \""
                                    << curr->name << "\" is not a subtype of "
                                    << "field inherited from parent class \""
                                    << curr->super->name << "\"" << std::endl;
                            failed = true;
                        }
                    }
                }
                checked[curr] = true;
            }
        }
    }
    void Program::checkReturnTypes(bool &failed) {
        for (auto classNode : *this->classes_) {
            ClassStruct *cs = classNode->getClassStruct();
            for (auto mthdNode : *classNode->getMethods()) {
                MethodStruct *ms = mthdNode->getMethodStruct();
                ClassStruct *currType = nullptr;
                bool absoluteReturn = false;
                for (Statement *stmt : *mthdNode->getStatements()) {
                    auto retType = stmt->getReturnType(cs, ms, failed);
                    if (retType.first != nullptr) {
                        if (currType == nullptr) {
                            currType = retType.first;
                        } else {
                            currType = currType->LCA[retType.first];
                        }
                        if (retType.second) {
                            absoluteReturn = true;
                        }
                    }
                }
                if (!absoluteReturn) {
                    if (currType == nullptr) {
                        currType = this->builtinTypes["Nothing"];
                    } else {
                        currType = currType->LCA[this->builtinTypes["Nothing"]];
                    }
                }
                if (currType->LCA[ms->type] != ms->type) {
                    std::cerr << "Error: Method \"" << cs->name << "."
                            << ms->name << "()\" does not return a subtype of "
                            << "its explicit type \"" << ms->type->name << "\"."
                            << " Instead it returns \"" << currType->name
                            << "\"" << std::endl;
                    failed = true;
                }
            }
        }
        for (Statement *stmt : *this->stmts_) {
            auto retType = stmt->getReturnType(nullptr, this->body_, failed);
            if (retType.first != nullptr) {
                std::cerr << stmt->getPosition() << "Error: Return statement "
                        << "not allowed in program body" << std::endl;
                failed = true;
            }
        }
    }
    bool Program::typeCheck(bool verbose) {
        bool failed = false;
        std::string T = "├", Bar = "│", Elbow = "└", Dash = "─";
        std::stack<std::pair<ClassStruct*, std::string>> classStack;
        if (verbose) {
            std::cout << "--------Checking Class Hierarchy--------" << std::endl;
        }
        addBuiltins();
        checkClassHierarchy(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            classStack.push(std::make_pair(this->builtinTypes["Obj"], ""));
            while (!classStack.empty()) {
                auto currPair = classStack.top();
                classStack.pop();
                std::cout << currPair.second << currPair.first->name << std::endl;
                for (size_t i = 0; i < currPair.first->children.size(); i++) {
                    auto child = currPair.first->children[i];
                    std::string prefix = currPair.second;
                    size_t findIndex = 0;
                    while ((findIndex = prefix.find(T, findIndex)) !=
                            std::string::npos) {
                        prefix.replace(findIndex, T.length(), Bar);
                        findIndex += Bar.length();
                    }
                    findIndex = 0;
                    while ((findIndex = prefix.find(Dash, findIndex)) !=
                            std::string::npos) {
                        prefix.replace(findIndex, Dash.length(), " ");
                        findIndex += 1;
                    }
                    findIndex = 0;
                    while ((findIndex = prefix.find(Elbow, findIndex)) !=
                            std::string::npos) {
                        prefix.replace(findIndex, Elbow.length(), " ");
                        findIndex += 1;
                    }

                    if (i == 0) {
                        classStack.push(std::make_pair(child, prefix + Elbow +
                                Dash));
                    } else {
                        classStack.push(std::make_pair(child, prefix + T + Dash));
                    }
                }
            }
            std::cout << "-Building Lowest Common Ancestor Table--"
                    << std::endl;
        }
        buildLCAs(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            for (auto it : this->classTable) {
                std::cout << "\t" << it.first;
            }
            std::cout << std::endl;
            for (auto it1 : this->classTable) {
                std::cout << it1.first << "\t";
                for (auto it2 : this->classTable) {
                    std::cout << it1.second->LCA[it2.second]->name << "\t";
                }
                std::cout << std::endl;
            }
            std::cout << "---------Building Method Tables---------"
                    << std::endl;
        }
        buildMethodTables(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            for (auto it : this->classTable) {
                ClassStruct *cs = it.second;
                for (auto it2 : cs->methodTable) {
                    MethodStruct *ms = it2.second;
                    std::cout << cs->name << "." << ms->name << "(";
                    std::string sep = "";
                    for (ClassStruct *arg : ms->argTypes) {
                        std::cout << sep << arg->name;
                        sep = ", ";
                    }
                    std::cout << ") : " << ms->type->name << std::endl;
                }
            }
            std::cout << "------Checking Method Inheritance-------" << std::endl;
        }
        checkMethodInheritance(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            for (auto it : this->classTable) {
                ClassStruct *cs = it.second;
                for (auto ms : cs->methodOrder) {
                    std::cout << cs->name << "." << ms->name << "(";
                    std::string sep = "";
                    for (ClassStruct *arg : ms->argTypes) {
                        std::cout << sep << arg->name;
                        sep = ", ";
                    }
                    std::cout << ") : " << ms->type->name << std::endl;
                }
            }
            std::cout << "-----Finding Fields and Local Vars------" << std::endl;
        }
        getVars(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            for (auto c : this->classTable) {
                ClassStruct *cs = c.second;
                if (this->builtinTypes.find(cs->name) == this->builtinTypes.end()) {
                    std::cout << "Class " << cs->name << std::endl;
                    std::cout << "    Fields:" << std::endl;
                    for (auto v : cs->fieldTable) {
                        if (this->builtinIdents.find(v.first) ==
                                this->builtinIdents.end()) {
                            std::cout << "        this." << v.first;
                            if (v.second.second) {
                                std::cout << "\t* " << v.second.first->name;
                            }
                            std::cout << std::endl;
                        }
                    }
                    std::cout << "    Constructor Locals:" << std::endl;
                    for (auto v : cs->constructor->symbolTable) {
                        if (this->builtinIdents.find(v.first) ==
                                this->builtinIdents.end()) {
                            std::cout << "        " << v.first;
                            if (v.second.second) {
                                std::cout << "\t* " << v.second.first->name;
                            }
                            std::cout << std::endl;
                        }
                    }
                    for (auto m : cs->methodTable) {
                        MethodStruct *ms = m.second;
                        std::cout << "    " << ms->name << "() Locals:"
                                << std::endl;
                        for (auto v : ms->symbolTable) {
                            if (this->builtinIdents.find(v.first) ==
                                    this->builtinIdents.end()) {
                                std::cout << "        " << v.first;
                                if (v.second.second) {
                                    std::cout << "\t* " << v.second.first->name;
                                }
                                std::cout << std::endl;
                            }
                        }
                    }
                }
            }
            std::cout << "Program Body Locals:" << std::endl;
            for (auto v : this->body_->symbolTable) {
                if (this->builtinIdents.find(v.first) ==
                        this->builtinIdents.end()) {
                    std::cout << "   " << v.first;
                    if (v.second.second) {
                        std::cout << "\t* " << v.second.first->name;
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << "------------Inferring Types-------------" << std::endl;
        }
        inferTypes(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            for (auto c : this->classTable) {
                ClassStruct *cs = c.second;
                if (this->builtinTypes.find(cs->name) ==
                        this->builtinTypes.end()) {
                    std::cout << "Class " << cs->name << std::endl;
                    std::cout << "    Fields:" << std::endl;
                    for (auto v : cs->fieldTable) {
                        if (this->builtinIdents.find(v.first) ==
                                this->builtinIdents.end()) {
                            std::cout << "        this." << v.first;
                            if (v.second.first != nullptr) {
                                if (v.second.second)
                                    std::cout << "\t* ";
                                else
                                    std::cout << "\t  ";
                                std::cout << v.second.first->name;
                            }
                            std::cout << std::endl;
                        }
                    }
                    std::cout << "    Constructor Locals:" << std::endl;
                    for (auto v : cs->constructor->symbolTable) {
                        if (this->builtinIdents.find(v.first) ==
                                this->builtinIdents.end()) {
                            std::cout << "        " << v.first;
                            if (v.second.first != nullptr) {
                                if (v.second.second)
                                    std::cout << "\t* ";
                                else
                                    std::cout << "\t  ";
                                std::cout << v.second.first->name;
                            }
                            std::cout << std::endl;
                        }
                    }
                    for (auto m : cs->methodTable) {
                        MethodStruct *ms = m.second;
                        std::cout << "    " << ms->name << "() Locals:"
                            << std::endl;
                        for (auto v : ms->symbolTable) {
                            if (this->builtinIdents.find(v.first) ==
                                    this->builtinIdents.end()) {
                                std::cout << "        " << v.first;
                                if (v.second.first != nullptr) {
                                    if (v.second.second)
                                        std::cout << "\t* ";
                                    else
                                        std::cout << "\t  ";
                                    std::cout << v.second.first->name;
                                }
                                std::cout << std::endl;
                            }
                        }
                    }
                }
            }
            std::cout << "Program Body Locals:" << std::endl;
            for (auto v : this->body_->symbolTable) {
                if (this->builtinIdents.find(v.first) ==
                        this->builtinIdents.end()) {
                    std::cout << "    " << v.first;
                    if (v.second.first != nullptr) {
                        if (v.second.second)
                            std::cout << "\t* ";
                        else
                            std::cout << "\t  ";
                        std::cout << v.second.first->name;
                    }
                    std::cout << std::endl;
                }
            }
            std::cout << "-------Checking Field Inheritance-------"
                    << std::endl;
        }
        checkFieldInheritance(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            std::cout << "All fields correctly inherited" << std::endl;
            std::cout << "------Checking Method Return Types------" << std::endl;
        }
        checkReturnTypes(failed);
        if (failed) {
            return true;
        }
        if (verbose) {
            std::cout << "All methods return subtypes of their declared type"
                    << std::endl;
        }
        return failed;
    }
    void Program::genCode(std::ostream &file) {
        file << "/*" << std::endl << "   Squawked with love, by ducks"
                << std::endl << "*/" << std::endl << std:: endl;
        file << "#include \"Builtins.h\"" << std::endl;
        file << "#include <stdlib.h>" << std::endl << std::endl;
        
        
        // Class typedefs
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            file << "typedef struct class_" << cs->name << "_struct *class_"
                    << cs->name << ";" << std::endl;
            file << "typedef struct obj_" << cs->name << "_struct *obj_"
                    << cs->name << ";" << std::endl << std::endl;
        }
        
        // Class/Obj structs
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            file << "struct class_" << cs->name << "_struct {" << std::endl;
            file << "\tclass_" << cs->super->name << "\tsuper;" << std::endl;
            for (MethodStruct *ms : cs->methodOrder) {
                file << "\tobj_" << ms->type->name << "\t(*method_" << ms->name
                        << ") (" << "obj_" << ms->clazz->name;
                for (ClassStruct *argType : ms->argTypes) {
                    file << ", obj_" << argType->name;
                }
                file << ");" << std::endl;
            }
            file << "};" << std::endl;
            file << "struct obj_" << cs->name << "_struct {" << std::endl;
            file << "\tclass_" << cs->name << "\tclass;" << std::endl;
            for (auto fieldKeyValue : cs->fieldTable) {
                std::string fieldName = fieldKeyValue.first;
                std::string fieldType = fieldKeyValue.second.first->name;
                file << "\tobj_" << fieldType << "\tfield_" << fieldName << ";"
                        << std::endl;
            }
            file << "};" << std::endl << std::endl;
        }
        file << "// Forward-declare methods" << std::endl;
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            file << "obj_" << cs->name << " new_" << cs->name << "(";
            std::string sep = "";
            for (TypedArg* arg : *c->getArgs()) {
                file << sep << "obj_" << arg->getType() << " local_"
                        << arg->getName();
                sep = ", ";
            }
            file << ");" << std::endl;
            for (Method *m : *c->getMethods()) {
                MethodStruct *ms = m->getMethodStruct();
                std::vector<std::string> argNames = m->getArgNames();
                file << "obj_" << ms->type->name << " " << cs->name
                        << "_method_" << ms->name << "(obj_" << cs->name <<
                        " this";
                for (TypedArg* arg : *m->getArgs()) {
                    file << ", obj_" << arg->getType() << " local_"
                            << arg->getName();
                }
                file << ");" << std::endl;
            }
            file << std::endl;
        }
        file << std::endl;
        
        //Class Instances
        std::stack<ClassStruct*> S;
        S.push(this->builtinTypes["Obj"]);
        while (!S.empty()) {
            ClassStruct *curr = S.top();
            S.pop();
            for (ClassStruct *child : curr->children) {
                S.push(child);
            }
            if (this->builtinTypes.find(curr->name) ==
                    this->builtinTypes.end()) {
                file << "struct class_" << curr->name << "_struct the_class_"
                        << curr->name << "_struct = {" << std::endl;
                file << "\t.super = &the_class_" << curr->super->name
                        << "_struct";
                for (MethodStruct *ms : curr->methodOrder) {
                    file << "," << std::endl << "\t.method_" << ms->name
                            << " = " << ms->clazz->name << "_method_"
                            << ms->name;
                }
                file << std::endl << "};" << std::endl;
                file << "const class_" << curr->name << " the_class_"
                        << curr->name << " = &the_class_" << curr->name
                        << "_struct;" << std::endl;
            }
        }
        
        for (Class *c : *this->classes_) {
            ClassStruct *cs = c->getClassStruct();
            file << "// " << cs->name << " Methods" << std::endl;
            
            // Constructor
            
            file << "obj_" << cs->name << " new_" << cs->name << "(";
            std::string sep = "";
            for (TypedArg* arg : *c->getArgs()) {
                file << sep << "obj_" << arg->getType() << " local_"
                        << arg->getName();
                sep = ", ";
            }
            file << ") {" << std::endl;
            std::vector<std::string> argNames = c->getArgNames();
            for (auto localKeyValue : cs->constructor->symbolTable) {
                if (this->builtinIdents.find(localKeyValue.first) ==
                        this->builtinIdents.end() && localKeyValue.first !=
                        "this" && std::find(argNames.begin(), argNames.end(),
                        localKeyValue.first) == argNames.end()) {
                    file << "\tobj_" << localKeyValue.second.first->name
                            << " local_" << localKeyValue.first << ";"
                            << std::endl;
                }
            }
            file << "\tobj_" << cs->name << " this = (obj_" << cs->name
                    << ")malloc(sizeof(*this));"
                    << std::endl;
            file << "\tthis->class = the_class_" << cs->name << ";" << std::endl;
            file << std::endl;
            for (Statement *s : *c->getStatements()) {
                s->generateCode(file, 1, cs, cs->constructor);
            }
            file << "\treturn this;" << std::endl;
            file << "}" << std::endl;
            
            //Methods
            
            for (Method *m : *c->getMethods()) {
                MethodStruct *ms = m->getMethodStruct();
                argNames = m->getArgNames();
                file << "obj_" << ms->type->name << " " << cs->name
                        << "_method_" << ms->name << "(obj_" << cs->name <<
                        " this";
                for (TypedArg* arg : *m->getArgs()) {
                    file << ", obj_" << arg->getType() << " local_"
                            << arg->getName();
                }
                file << ") {" << std::endl;
                for (auto localKeyValue : ms->symbolTable) {
                    if (this->builtinIdents.find(localKeyValue.first) ==
                            this->builtinIdents.end() && localKeyValue.first !=
                            "this" && std::find(argNames.begin(),
                            argNames.end(), localKeyValue.first) ==
                            argNames.end()) {
                        file << "\tobj_" << localKeyValue.second.first->name
                                << " local_" << localKeyValue.first << ";"
                                << std::endl;
                    }
                }
                file << std::endl;
                bool absoluteReturn = false;
                for (Statement *s : *m->getStatements()) {
                    s->generateCode(file, 1, cs, ms);
                    bool failed = false;
                    auto retType = s->getReturnType(cs, ms, failed);
                    if (retType.first != nullptr) {
                        if (retType.second) {
                            absoluteReturn = true;
                        }
                    }
                }
                if (!absoluteReturn) {
                    file << "\treturn lit_none;" << std::endl;
                }
                file << "}" << std::endl;
            }
            file << std::endl;
        }
        
        //Program Body
        file << "int main() {" << std::endl;
        for (auto localKeyValue : this->body_->symbolTable) {
            if (this->builtinIdents.find(localKeyValue.first) ==
                    this->builtinIdents.end()) {
                file << "\tobj_" << localKeyValue.second.first->name
                        << " local_" << localKeyValue.first << ";"
                        << std::endl;
            }
        }
        file << std::endl;
        for (Statement *s : *this->stmts_) {
            s->generateCode(file, 1, nullptr, this->body_);
        }
        file << "}" << std::endl;
        
    }

/* TypeAlt */
    void TypeAlt::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "TypeAlt");
        this->json_string(out, indent, "name_", this->name_);
        this->json_string(out, indent, "type_", this->type_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }

/* RExpr */
    void RExpr::updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) {
        this->getType(thisClass, thisMethod, failed);
    }
    void RExpr::generateBranchCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod,
            std::string trueLabel, std::string falseLabel) {
        std::string condVar = this->generateRExprCode(file, indent, thisClass,
                thisMethod);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "if (" << condVar << " == lit_true) {" << std::endl;
        for (int i = 0; i < indent + 1; i++) { file << "\t"; }
        file << "goto " << trueLabel << ";" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "}" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "goto " << falseLabel << ";" << std::endl;
    }

/* If */
    void If::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "If");
        this->json_child(out, indent, "cond_", this->cond_);
        this->json_list<Statement*>(out, indent, "if_true_",
                *this->if_true_stmts_);
        this->json_list<Statement*>(out, indent, "if_false_",
                *this->if_false_stmts_);
        this->json_close(out, indent);
    }
    void If::set_else(std::vector<Statement*> *else_stmts) {
        delete this->if_false_stmts_;
        this->if_false_stmts_ = else_stmts;
    }
    void If::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> varCopy1 = vars, varCopy2, fieldCopy1 = fields,
                fieldCopy2;
        cond_->getVars(varCopy1, fields, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy1, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy1, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        fieldCopy1 = fields;
        varCopy1 = vars;
        for (Statement *s : *if_true_stmts_) {
            s->getVars(varCopy1, fieldCopy1, varTable, fieldTable,
                    inConstructor, failed);
        }
        fieldCopy2 = fields;
        varCopy2 = vars;
        for (Statement *s : *if_false_stmts_) {
            s->getVars(varCopy2, fieldCopy2, varTable, fieldTable,
                    inConstructor, failed);
        }
        fields = intersection(fieldCopy1, fieldCopy2);
        vars = intersection(varCopy1, varCopy2);
    }
    void If::updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) {
        ClassStruct *condType = cond_->getType(thisClass, thisMethod, failed);
        if (condType != nullptr && condType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: If statement uses "
                    << "type \"" << condType->name << "\" as condition, must "
                    << "be of type Boolean" << std::endl;
            failed = true;
        }
        for (Statement *s : *if_true_stmts_) {
            s->updateTypes(thisClass, thisMethod, changed, failed);
        }
        for (Statement *s : *if_false_stmts_) {
            s->updateTypes(thisClass, thisMethod, changed, failed);
        }
    }
    std::pair<ClassStruct*, bool> If::getReturnType(ClassStruct *thisClass,
        MethodStruct *thisMethod, bool &failed) {
        ClassStruct *currIfType = nullptr, *currElseType = nullptr;
        bool ifEnforced = false, elseEnforced = false;
        for (Statement *stmt : *if_true_stmts_) {
            auto retType = stmt->getReturnType(thisClass, thisMethod, failed);
            if (retType.first != nullptr) {
                if (currIfType == nullptr) {
                    currIfType = retType.first;
                } else {
                    currIfType = currIfType->LCA[retType.first];
                }
                if (retType.second) {
                    ifEnforced = true;
                }
            }
        }
        for (Statement *stmt : *if_false_stmts_) {
            auto retType = stmt->getReturnType(thisClass, thisMethod, failed);
            if (retType.first != nullptr) {
                if (currElseType == nullptr) {
                    currElseType = retType.first;
                } else {
                    currElseType = currElseType->LCA[retType.first];
                }
                if (retType.second) {
                    elseEnforced = true;
                }
            }
        }
        ClassStruct *retType = nullptr;
        if (currIfType != nullptr) {
            if (currElseType != nullptr) {
                retType = currIfType->LCA[currElseType];
            } else {
                retType = currIfType;
            }
        } else {
            retType = currElseType;
        }
        // This if statement only has a definite return if both true and false
        // branches both have definite returns. The overall type of the method
        // is determined by the LCA of the two branch return types.
        return std::make_pair(retType, ifEnforced && elseEnforced);
    }
    void If::generateCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string trueLabel = "LABEL" + std::to_string(this->labelID++) +
                "_TRUE";
        std::string falseLabel = "LABEL" + std::to_string(this->labelID++) +
                "_FALSE";
        this->cond_->generateBranchCode(file, indent, thisClass, thisMethod,
                trueLabel, falseLabel);
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << trueLabel << ":;" << std::endl;
        for (Statement *s : *this->if_true_stmts_) {
            s->generateCode(file, indent, thisClass, thisMethod);
        }
        std::string done = "LABEL" + std::to_string(this->labelID++) +
                "_DONE";
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "goto " << done << ";" << std::endl;
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << falseLabel << ":;" << std::endl;
        for (Statement *s : *this->if_false_stmts_) {
            s->generateCode(file, indent, thisClass, thisMethod);
        }
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << done << ":;" << std::endl;
    }

/* While */
    void While::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "While");
        this->json_child(out, indent, "cond_", this->cond_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void While::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        cond_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        fieldCopy = fields;
        varCopy = vars;
        for (Statement *s : *stmts_) {
            s->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                    failed);
        }
    }
    void While::updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) {
        ClassStruct *condType = cond_->getType(thisClass, thisMethod, failed);
        if (condType != nullptr && condType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: While statement uses "
                    << "type \"" << condType->name << "\" as condition, must "
                    << "be of type Boolean" << std::endl;
            failed = true;
        }
        for (Statement *s : *stmts_) {
            s->updateTypes(thisClass, thisMethod, changed, failed);
        }
    }
    std::pair<ClassStruct*, bool> While::getReturnType(ClassStruct *thisClass,
        MethodStruct *thisMethod, bool &failed) {
        ClassStruct *retType = nullptr;
        for (Statement *stmt : *stmts_) {
            auto stmtType = stmt->getReturnType(thisClass, thisMethod, failed);
            if (stmtType.first != nullptr) {
                if (retType == nullptr) {
                    retType = stmtType.first;
                } else {
                    retType = retType->LCA[stmtType.first];
                }
            }
        }
        // While loops cannot have a definite return because they can't be
        // guaranteed to run at least once.
        return std::make_pair(retType, false);
    }
    void While::generateCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string testLabel = "LABEL" + std::to_string(this->labelID++) +
                "_TEST";
        std::string againLabel = "LABEL" + std::to_string(this->labelID++) +
                "_AGAIN";
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "goto " << testLabel << ";" << std::endl;
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << againLabel << ":;" << std::endl;
        for (Statement *s : *this->stmts_) {
            s->generateCode(file, indent, thisClass, thisMethod);
        }
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << testLabel << ":;" << std::endl;
        std::string doneLabel = "LABEL" + std::to_string(this->labelID++) +
                "_DONE";
        this->cond_->generateBranchCode(file, indent, thisClass, thisMethod,
                againLabel, doneLabel);
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << doneLabel << ":;" << std::endl;
    }

/* LExpr */
    void LExpr::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "LExpr");
        if (this->isField_)
            this->json_child(out, indent, "obj_", this->obj_);
        this->json_string(out, indent, "name_", this->name_);
        this->json_close(out, indent);
    }
    bool LExpr::isAssignable() const {
        return (isField_ && obj_->isThis()) || (!isField_ &&
                this->builtinIdents.find(name_) == this->builtinIdents.end() &&
                this->name_ != "this");
    }
    void LExpr::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        if (isField_) {
            if (obj_->isThis()) {
                if (std::find(fields.begin(), fields.end(), this->name_) ==
                        fields.end()) {
                    std::cerr << this->getPosition() << "Error: \"this."
                            << this->name_ << "\" used before initialization"
                            << std::endl;
                    failed = true;
                }
            } else {
                std::vector<std::string> fieldCopy = fields, varCopy = vars;
                obj_->getVars(varCopy, fieldCopy, varTable, fieldTable,
                        inConstructor, failed);
                for (auto varName : difference(varCopy, vars)) {
                    std::cerr << this->getPosition() << "Error: Variable \""
                            << varName << "\" used before initialization"
                            << std::endl;
                    failed = true;
                }
                for (auto fieldName : difference(fieldCopy, fields)) {
                    std::cerr << this->getPosition() << "Error: Field \"this."
                            << fieldName << "\" used before initialization"
                            << std::endl;
                    failed = true;
                }
            }
        } else {
            if (std::find(vars.begin(), vars.end(), this->name_) ==
                    vars.end()) {
                std::cerr << this->getPosition() << "Error: \"" << this->name_
                        << "\" used before initialization" << std::endl;
                failed = true;
            }
        }
    }
    ClassStruct *LExpr::getType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) {
        if (isField_) {
            ClassStruct *objType = obj_->getType(thisClass, thisMethod, failed);
            if (objType != nullptr) {
                if (objType->fieldTable.find(this->name_) !=
                        objType->fieldTable.end()) {
                    return objType->fieldTable[this->name_].first;
                } else {
                    std::cerr << this->getPosition() << "Error: \""
                            << this->name_ << "\" is not a field of class \""
                            << objType->name << "\"" << std::endl;
                    failed = true;
                }
            }
        } else if (this->name_ == "this") {
            return thisClass;
        } else if (this->builtinIdents.find(this->name_) !=
                this->builtinIdents.end()) {
            return this->builtinIdents[this->name_];
        } else {
            return thisMethod->symbolTable[this->name_].first;
        }
        return nullptr;
    }
    std::string LExpr::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        if (isField_) {
            if (this->obj_->isThis()) {
                return "this->field_" + this->name_;
            }
            std::string obj_var = obj_->generateRExprCode(file, indent,
                    thisClass, thisMethod);
            bool failed = false;;
            ClassStruct *obj_type = obj_->getType(thisClass, thisMethod, 
                    failed);
            ClassStruct *field_type = obj_type->fieldTable[this->name_].first;
            for (int i=0; i < indent; i++) { file << "\t"; }
            std::string this_var = "temp" + std::to_string(this->tempVarID++);
            file << "obj_" << field_type->name << " " << this_var << " = " 
                    << obj_var << "->field_" << this->name_ << ";" << std::endl;
            return this_var;
        } else if (this->name_ == "this") {
            return "this";
        } else if (this->builtinIdents.find(this->name_) !=
                this->builtinIdents.end()) {
            return "lit_" + this->name_;
        } else {
            return "local_" + this->name_;
        }
        return "";
    }

/* Assignment */
    void Assignment::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Assignment");
        this->json_child(out, indent, "l_expr_", this->l_expr_);
        if (this->isTyped_)
            this->json_string(out, indent, "type_", this->type_);
        this->json_child(out, indent, "r_expr_", this->r_expr_);
        this->json_close(out, indent);
    }
    void Assignment::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        std::string fieldName, varName;
        r_expr_->getVars(varCopy, fieldCopy, varTable, fieldTable,
                inConstructor, failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        if (inConstructor && this->r_expr_->isThis()) {
            std::cerr << this->getPosition() << "Error: Cannot use 'this' on "
                    << "right-hand side of an assignment within its constructor"
                    << std::endl;
            failed = true;
        }
        if (l_expr_->isField()) {
            fieldName = l_expr_->getName();
            if (l_expr_->isAssignable()) {
                if (std::find(fields.begin(), fields.end(), fieldName) ==
                        fields.end()) {
                    fields.push_back(fieldName);
                }
                if (isTyped_) {
                    if (this->classTable.find(this->type_) !=
                            this->classTable.end()) {
                        if (fieldTable.find(fieldName) == fieldTable.end()) {
                            fieldTable[fieldName] = std::make_pair(
                                    this->classTable[this->type_], true);
                        } else if (fieldTable[fieldName].second) {
                            if (fieldTable[fieldName].first !=
                                    this->classTable[this->type_]) {
                                std::cerr << this->getPosition() << "Error: Field "
                                        << "\"this." << fieldName << "\" cannot "
                                        << "have more than one explicit type "
                                        << "definition" << std::endl;
                                failed = true;
                            }
                        } else {
                            fieldTable[fieldName] = std::make_pair(
                                    this->classTable[this->type_], true);
                        }
                    } else {
                        std::cerr << this->getPosition() << "Error: Field "
                                << "\"this." << fieldName << "\" given "
                                << "unrecognized explicit type \""
                                << this->type_ << "\"" << std::endl;
                        failed = true;
                    }
                } else {
                    if (fieldTable.find(fieldName) == fieldTable.end()) {
                        fieldTable[fieldName] = std::make_pair(nullptr, false);
                    }
                }
            } else {
                std::cerr << this->getPosition() << "Error: Cannot assign to "
                        << "field \"" << fieldName << "\" outside of its "
                        << "class's methods" << std::endl;
                failed = true;
            }
        } else {
            varName = l_expr_->getName();
            if (l_expr_->isAssignable()) {
                if (std::find(vars.begin(), vars.end(), varName) ==
                        vars.end()) {
                    vars.push_back(varName);
                }
                if (isTyped_) {
                    if (this->classTable.find(this->type_) !=
                            this->classTable.end()) {
                        if (varTable.find(varName) == varTable.end()) {
                            varTable[varName] = std::make_pair(
                                    this->classTable[this->type_], true);
                        } else if (varTable[varName].second) {
                            if (varTable[varName].first !=
                                    this->classTable[this->type_]) {
                                std::cerr << this->getPosition() << "Error: "
                                        << "Variable \"" << varName << "\" "
                                        << "given contradictory explicit type "
                                        << "definition \"" << this->type_ 
                                        << "\". It was already explicitly "
                                        << "defined to be an instance of \""
                                        << varTable[varName].first->name << "\""
                                        << std::endl;
                                failed = true;
                            }
                        } else {
                            varTable[varName] = std::make_pair(
                                    this->classTable[this->type_], true);
                        }
                    } else {
                        std::cerr << this->getPosition() << "Error: Variable \""
                                << varName << "\" given unrecognized explicit "
                                << "type \"" << this->type_ << "\""
                                << std::endl;
                        failed = true;
                    }
                } else {
                    if (varTable.find(varName) == varTable.end()) {
                        varTable[varName] = std::make_pair(nullptr, false);
                    }
                }
            } else {
                std::cerr << this->getPosition() << "Error: Cannot assign to "
                        << "protected variable \"" << varName << "\""
                        << std::endl;
                failed = true;
            }
        }
    }
    void Assignment::updateTypes(ClassStruct *thisClass,
        MethodStruct *thisMethod, bool &changed, bool &failed) {
                std::string l_expr_name = l_expr_->getName();
        ClassStruct *r_expr_type = r_expr_->getType(thisClass, thisMethod,
                failed);
        ClassStruct *prevType;
        if (r_expr_type) {
            if (l_expr_->isAssignable()) {
                if (l_expr_->isField()) {
                    if (thisClass->fieldTable.find(l_expr_name) ==
                            thisClass->fieldTable.end()) {
                        std::cerr << this->getPosition() << "Error: Class \""
                                << thisClass->name << "\" does not contain the "
                                << "field \"" << l_expr_name << "\""
                                << std::endl;
                        failed = true;
                    } else {
                        prevType = thisClass->fieldTable[l_expr_name].first;
                        if (prevType == nullptr) {
                            thisClass->fieldTable[l_expr_name] = std::make_pair(
                                    r_expr_type, false);
                            changed = true;
                        } else {
                            ClassStruct *LCAType = prevType->LCA[r_expr_type];
                            if (LCAType != prevType) {
                                if (thisClass->fieldTable[l_expr_name].second) {
                                    std::cerr << this->getPosition()
                                            << "Error: Field \"" << l_expr_name
                                            << "\" cannot be assigned to type "
                                            << "\"" << r_expr_type->name
                                            << "\" because it is not a subtype "
                                            << "of its explicit type \""
                                            << prevType->name << "\""
                                            << std::endl;
                                    failed = true;
                                } else {
                                    thisClass->fieldTable[l_expr_name] =
                                            std::make_pair(LCAType, false);
                                    changed = true;
                                }
                            }
                        }
                    }
                } else {
                    if (thisMethod->symbolTable.find(l_expr_name) ==
                            thisMethod->symbolTable.end()) {
                        std::cerr << this->getPosition() << "Error: Variable \""
                                << l_expr_name << "\" does not exist in the "
                                << "current context" << std::endl;
                        failed = true;
                    } else {
                        prevType = thisMethod->symbolTable[l_expr_name].first;
                        if (prevType == nullptr) {
                            thisMethod->symbolTable[l_expr_name] =
                                    std::make_pair(r_expr_type, false);
                            changed = true;
                        } else {
                            ClassStruct *LCAType = prevType->LCA[r_expr_type];
                            if (LCAType != prevType) {
                                if (thisMethod->
                                        symbolTable[l_expr_name].second) {
                                    std::cerr << this->getPosition()
                                            << "Error: Variable \""
                                            << l_expr_name << "\" cannot be "
                                            << "assigned to type \""
                                            << r_expr_type->name
                                            << "\" because it is not a subtype "
                                            << "of its explicit type \""
                                            << prevType->name << "\""
                                            << std::endl;
                                    failed = true;
                                } else {
                                    thisMethod->symbolTable[l_expr_name] =
                                            std::make_pair(LCAType, false);
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            } else {
                std::cerr << this->getPosition() << "Error: Cannot assign to "
                        << "variable \"" << l_expr_name << "\"" << std::endl;
                failed = true;
            }
        }
    }
    void Assignment::generateCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string r_expr_var = r_expr_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string l_expr_var = l_expr_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        bool failed = false;
        ClassStruct *l_expr_type = this->l_expr_->getType(thisClass, thisMethod,
                failed);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << l_expr_var << " = (obj_" << l_expr_type->name << ")"
                << r_expr_var << ";" << std::endl;
    }

/* Return */
    void Return::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Return");
        if (!this->returnsNone_)
            this->json_child(out, indent, "r_expr_", this->r_expr_);
        this->json_close(out, indent);
    }
    void Return::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        if (inConstructor) {
            std::cerr << this->getPosition() << "Error: Return statement not "
                    << "allowed in constructor" << std::endl;
            failed = true;
        }
        if (!this->returnsNone_) {
            r_expr_->getVars(varCopy, fieldCopy, varTable, fieldTable,
                    inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << "Error: Variable \""
                        << varName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << "Error: Field \"this."
                        << fieldName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
        }
    }
    void Return::updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) {
        if (!returnsNone_) {
            r_expr_->updateTypes(thisClass, thisMethod, changed, failed);
        }
    }
    std::pair<ClassStruct*, bool> Return::getReturnType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) {
        if (this->returnsNone_) {
            return std::make_pair(this->builtinTypes["Nothing"], true);
        } else {
            ClassStruct *retType =
                    r_expr_->getType(thisClass, thisMethod, failed);
            return std::make_pair(retType, true);
        }
    }
    void Return::generateCode(std::ostream& file, int indent,
            ClassStruct* thisClass, MethodStruct* thisMethod) {
        if (returnsNone_) {
            for (int i = 0; i < indent; i++) { file << "\t"; }
            file << "return (obj_" << thisMethod->type->name << ")lit_none;"
                    << std::endl;
        } else {
            std::string retVar = this->r_expr_->generateRExprCode(file, indent,
                    thisClass, thisMethod);
            for (int i = 0; i < indent; i++) { file << "\t"; }
            file << "return (obj_" << thisMethod->type->name << ")" << retVar
                    << ";" << std::endl;
        }
    }

/* Typecase */
    void Typecase::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Typecase");
        this->json_child(out, indent, "expr_", this->expr_);
        this->json_list<TypeAlt*>(out, indent, "alternatives_",
                *this->alternatives_);
        this->json_close(out, indent);
    }
    void Typecase::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        std::string altName, altType;
        expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \""
                    << varName << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        for (TypeAlt *alt : *alternatives_) {
            altName = alt->getName();
            if (varTable.find(altName) != varTable.end()) {
                std::cerr << this->getPosition() << "Error: Typecase cannot "
                        << "introduce the variable \"" << altName
                        << "\" because it already exists in this scope"
                        << std::endl;
                failed = true;
            } else {
                fieldCopy = fields;
                varCopy = vars;
                altType = alt->getType();
                if (this->classTable.find(altType) == this->classTable.end()) {
                    std::cerr << this->getPosition() << "Error: Typecase "
                            << "cannot introduce variable of unrecognized type "
                            << "\"" << altType << "\"" << std::endl;
                    failed = true;
                } else {
                    varTable[altName] = std::make_pair(
                            this->classTable[altType], true);
                    if (std::find(varCopy.begin(), varCopy.end(), altName) ==
                            varCopy.end()) {
                        varCopy.push_back(altName);
                    }
                    for (Statement *s : *alt->getStatements()) {
                        s->getVars(varCopy, fieldCopy, varTable, fieldTable,
                                inConstructor, failed);
                    }
                }
            }
        }
    }
    void Typecase::updateTypes(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &changed, bool &failed) {
        expr_->updateTypes(thisClass, thisMethod, changed, failed);
        for (TypeAlt *alt : *alternatives_) {
            for (Statement *s : *alt->getStatements()) {
                s->updateTypes(thisClass, thisMethod, changed, failed);
            }
        }
    }
    std::pair<ClassStruct*, bool> Typecase::getReturnType(
            ClassStruct *thisClass, MethodStruct *thisMethod, bool &failed) {
        ClassStruct *retType = nullptr;
        for (TypeAlt *alt : *alternatives_) {
            for (Statement *stmt : *alt->getStatements()) {
                auto stmtType = stmt->getReturnType(thisClass, thisMethod,
                        failed);
                if (stmtType.first != nullptr) {
                    if (retType == nullptr) {
                        retType = stmtType.first;
                    } else {
                        retType = retType->LCA[stmtType.first];
                    }
                }
            }
        }
        return std::make_pair(retType, false);
    }
    void Typecase::generateCode(std::ostream &file, int indent, ClassStruct *thisClass,
	    MethodStruct *thisMethod) {
        std::string exprVar = this->expr_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string currVar = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "class_Obj " << currVar << " = (class_Obj)" << exprVar << "->class;"
                << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "while (" << currVar << ") {" << std::endl;
        for (int i = 0; i < indent + 1; i++) { file << "\t"; }
        for (TypeAlt *alt : *this->alternatives_) {
            file << "if (" << currVar << " == (class_Obj)the_class_"
                    << alt->getType() << ") {" << std::endl;
            for (int i = 0; i < indent + 2; i++) { file << "\t"; }
            file << "local_" << alt->getName() << " = (obj_"
                    << alt->getType() << ")" << exprVar << ";" << std::endl;
            for (Statement *s : *alt->getStatements()) {
                s->generateCode(file, indent+2, thisClass, thisMethod);
            }
            for (int i = 0; i < indent + 2; i++) { file << "\t"; }
            file << "break;" << std::endl;
            for (int i = 0; i < indent + 1; i++) { file << "\t"; }
            file << "} else ";
        }
        file << "{" << std::endl;
        for (int i = 0; i < indent + 2; i++) { file << "\t"; }
        file << currVar << " = " << currVar << "->super;"
                << std::endl;
        for (int i = 0; i < indent + 1; i++) { file << "\t"; }
        file << "}" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "}" << std::endl;
        
    }

/* IntLit */
    void IntLit::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Int");
        this->json_int(out, indent, "val_", this->val_);
        this->json_close(out, indent);
    }
    std::string IntLit::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string tmpName = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_Int " << tmpName << " = int_literal(" << this->val_ << ");"
                << std::endl;
        return tmpName;
    }

/* StrLit */
    void StrLit::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "String");
        this->json_string(out, indent, "text_", this->text_);
        this->json_close(out, indent);
    }
    std::string StrLit::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string tmpName = "temp" + std::to_string(this->tempVarID++);
        std::string fixedText = this->text_;
        
        std::map<char, std::string> escapes;
        escapes['\0'] = "\\0";
        escapes['\b'] = "\\b";
        escapes['\t'] = "\\t";
        escapes['\n'] = "\\n";
        escapes['\r'] = "\\r";
        escapes['\f'] = "\\f";
        escapes['"'] = "\\\"";
        escapes['\\'] = "\\\\";
        size_t index = 0;
        while (index < fixedText.size()) {
            char c = fixedText[index];
            if (escapes.find(c) != escapes.end()) {
                fixedText.replace(index, 1, escapes[c]);
                index += escapes[c].size();
            } else {
                index++;
            }
        }
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_String " << tmpName << " = str_literal(\"" << fixedText
                << "\");" << std::endl;
        return tmpName;
    }

/* Call */
    void Call::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Call");
        this->json_child(out, indent, "obj_", this->obj_);
        this->json_string(out, indent, "mthd_", this->mthd_);
        this->json_list<RExpr*>(out, indent, "args_", *this->args_);
        this->json_close(out, indent);
    }
    void Call::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        obj_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        if (inConstructor && obj_->isThis()) {
            std::cerr << this->getPosition() << "Error: Cannot call a method of"
                    << " \"this\" from within its own constructor" << std::endl;
            failed = true;
        }
        for (RExpr *arg : *args_) {
            if (inConstructor && arg->isThis()) {
                std::cerr << this->getPosition() << "Error: Cannot pass 'this' "
                        << "as an argument from its constructor" << std::endl;
                failed = true;
            }
            fieldCopy = fields;
            varCopy = vars;
            arg->getVars(varCopy, fieldCopy, varTable, fieldTable,
                    inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << "Error: Variable \""
                        << varName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << "Error: Field \"this."
                        << fieldName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
        }
    }
    ClassStruct *Call::getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) {
        std::vector<ClassStruct*> argTypes;
        ClassStruct *actualArgType, *objType;
        objType = obj_->getType(thisClass, thisMethod, failed);
        if (objType != nullptr) {
            if (objType->methodTable.find(mthd_) ==
                    objType->methodTable.end()) {
                std::cerr << this->getPosition() << "Error: \"" << mthd_
                        << "()\" is not a method of class \"" << objType->name
                        << "\"" << std::endl;
                failed = true;
            } else {
                argTypes = objType->methodTable[mthd_]->argTypes;
                if (args_->size() != argTypes.size()) {
                    std::cerr << this->getPosition() << "Error: Method \""
                            << mthd_ << "()\" of class \"" << objType->name
                            << "\" takes " << argTypes.size() << " arguments, "
                            << "not " << args_->size() << std::endl;
                    failed = true;
                } else {
                    bool allArgs = true;
                    for (size_t i = 0; i < args_->size(); i++) {
                        actualArgType = (*args_)[i]->getType(thisClass,
                                thisMethod, failed);
                        if (actualArgType != nullptr) {
                            ClassStruct *LCAType =
                                    actualArgType->LCA[argTypes[i]];
                            if (LCAType != argTypes[i]) {
                                std::cerr << this->getPosition() << "Error: "
                                        << "Argument " << i+1 << " of method \""
                                        << this->mthd_ << "()\" of class \""
                                        << objType->name << "\" has type \""
                                        << actualArgType->name << "\" which is "
                                        << "not a subtype of the required type "
                                        << "\"" << argTypes[i]->name << "\""
                                        << std::endl;
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
    std::string Call::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string objVar = obj_->generateRExprCode(file, indent, thisClass,
                thisMethod);
        std::vector<std::pair<std::string, ClassStruct*>> args;
        bool failed = false;
        ClassStruct *objType = obj_->getType(thisClass, thisMethod, failed);
        MethodStruct *ms = objType->methodTable[this->mthd_];
        for (size_t i = 0; i < this->args_->size(); i++) {
            std::string name = this->args_->at(i)->generateRExprCode(file,
                    indent, thisClass, thisMethod);
            ClassStruct *type = ms->argTypes[i];
            args.push_back(std::make_pair(name, type));
        }
        ClassStruct *thisType = ms->clazz;
        ClassStruct *retType = objType->methodTable[this->mthd_]->type;
        std::string tmpName = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_" << retType->name << " " << tmpName << " = " << 
                objVar << "->class->method_" << this->mthd_ << "((obj_"
                << thisType->name << ")" << objVar;
        for (auto arg : args) {
            file << ", (obj_" << arg.second->name << ")" << arg.first;
        }
        file << ");" << std::endl;
        return tmpName;
    }

/* Constructor */
    void Constructor::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Constructor");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<RExpr*>(out, indent, "args_", *this->args_);
        this->json_close(out, indent);
    }
    void Constructor::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy, varCopy;
        if (this->classTable.find(name_) == this->classTable.end()) {
            std::cerr << this->getPosition() << "Error: \"" << this->name_
                    << " \" constructor is for an unrecognized class"
                    << std::endl;
            failed = true;
        }
        for (RExpr *arg : *args_) {
            if (inConstructor && arg->isThis()) {
                std::cerr << this->getPosition() << "Error: Cannot pass 'this' "
                        << "as argument from constructor" << std::endl;
                failed = true;
            }
            fieldCopy = fields;
            varCopy = vars;
            arg->getVars(varCopy, fieldCopy, varTable, fieldTable,
                    inConstructor, failed);
            for (auto varName : difference(varCopy, vars)) {
                std::cerr << this->getPosition() << "Error: Variable \""
                        << varName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
            for (auto fieldName : difference(fieldCopy, fields)) {
                std::cerr << this->getPosition() << "Error: Field \"this."
                        << fieldName << "\" used before initialization"
                        << std::endl;
                failed = true;
            }
        }
    }
    ClassStruct *Constructor::getType(ClassStruct *thisClass,
            MethodStruct *thisMethod, bool &failed) {
        std::vector<ClassStruct*> argTypes =
                this->classTable[name_]->constructor->argTypes;
        if (args_->size() != argTypes.size()) {
            std::cerr << this->getPosition() << "Error: Constructor for class "
                    << "\"" << name_ << "\" takes " << argTypes.size()
                    << " arguments, not " << args_->size() << std::endl;
            failed = true;
        } else {
            for (size_t i = 0; i < args_->size(); i++) {
                ClassStruct *actualArgType = (*args_)[i]->getType(thisClass,
                        thisMethod, failed);
                if (actualArgType != nullptr) {
                    ClassStruct *LCAType = actualArgType->LCA[argTypes[i]];
                    if (LCAType != argTypes[i]) {
                        std::cerr << this->getPosition() << "Error: Argument "
                                << i+1 << " of \"" << this->name_
                                << "\" constructor call has type \""
                                << actualArgType->name << "\" which is not a "
                                << "subtype of the required type \""
                                << argTypes[i]->name << "\"" << std::endl;
                        failed = true;
                    }
                }
            }
        }
        return this->classTable[this->name_];
    }
    std::string Constructor::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        ClassStruct *type = this->classTable[this->name_];
        std::string tmpVar = "temp" + std::to_string(this->tempVarID++);
        std::vector<std::pair<std::string, std::string>> argVars;
        std::vector<ClassStruct*> argTypes =
                this->classTable[this->name_]->constructor->argTypes;
        for (size_t i = 0; i < this->args_->size(); i++) {
            std::string argName = this->args_->at(i)->generateRExprCode(file,
                    indent, thisClass, thisMethod);
            std::string argType = argTypes.at(i)->name;
            argVars.push_back(std::make_pair(argType, argName));
        }
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_" << type->name << " " << tmpVar << " = new_" << type->name
                << "(";
        std::string sep = "";
        for (size_t i = 0; i < argVars.size(); i++) {
            file << sep << "(obj_" << argVars[i].first << ")"
                    << argVars[i].second;
            sep = ", ";
        }
        file << ");" << std::endl;
        return tmpVar;
    }

/* And */
    void And::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "And");
        this->json_child(out, indent, "lhs_", this->lhs_);
        this->json_child(out, indent, "rhs_", this->rhs_);
        this->json_close(out, indent);
    }
    void And::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        lhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        fieldCopy = fields;
        varCopy = vars;
        rhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
    }
    ClassStruct *And::getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) {
        ClassStruct *exprType;
        exprType = lhs_->getType(thisClass, thisMethod, failed);
        if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: Left-hand expression "
                    << "of 'and' is not a Boolean" << std::endl;
            failed = true;
        }
        exprType = rhs_->getType(thisClass, thisMethod, failed);
        if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: Right-hand expression "
                    << "of 'and' is not a Boolean" << std::endl;
            failed = true;
        }
        return this->builtinTypes["Boolean"];
    }
    void And::generateBranchCode(std::ostream& file, int indent,
            ClassStruct* thisClass, MethodStruct* thisMethod,
            std::string trueLabel, std::string falseLabel) {
        std::string halfLabel = "LABEL" + std::to_string(this->labelID++) +
                "_HALF";
        this->lhs_->generateBranchCode(file, indent, thisClass, thisMethod,
                halfLabel, falseLabel);
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << halfLabel << ":;" << std::endl;
        this->rhs_->generateBranchCode(file, indent, thisClass, thisMethod,
                trueLabel, falseLabel);
    }
    std::string And::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string lhsVar = this->lhs_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string rhsVar = this->rhs_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string retVar = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_Boolean " << retVar << " = lit_false;" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "if (" << lhsVar << " == lit_true && " << rhsVar
                << " == lit_true) { " << retVar << " = lit_true; }"
                << std::endl;
        return retVar;
    }

/* Or */
    void Or::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Or");
        this->json_child(out, indent, "lhs_", this->lhs_);
        this->json_child(out, indent, "rhs_", this->rhs_);
        this->json_close(out, indent);
    }
    void Or::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        lhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
        fieldCopy = fields;
        varCopy = vars;
        rhs_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
    }
    ClassStruct *Or::getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) {
        ClassStruct *exprType;
        exprType = lhs_->getType(thisClass, thisMethod, failed);
        if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: Left-hand expression "
                    << "of 'or' is not a Boolean" << std::endl;
            failed = true;
        }
        exprType = rhs_->getType(thisClass, thisMethod, failed);
        if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: Right-hand expression "
                    << "of 'or' is not a Boolean" << std::endl;
            failed = true;
        }
        return this->builtinTypes["Boolean"];
    }
    void Or::generateBranchCode(std::ostream& file, int indent,
            ClassStruct* thisClass, MethodStruct* thisMethod,
            std::string trueLabel, std::string falseLabel) {
        std::string halfLabel = "LABEL" + std::to_string(this->labelID++) +
                "_HALF";
        this->lhs_->generateBranchCode(file, indent, thisClass, thisMethod,
                trueLabel, halfLabel);
        for (int i = 0; i < indent - 1; i++) { file << "\t"; }
        file << halfLabel << ":;" << std::endl;
        this->rhs_->generateBranchCode(file, indent, thisClass, thisMethod,
                trueLabel, falseLabel);
    }
    std::string Or::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string lhsVar = this->lhs_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string rhsVar = this->rhs_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string retVar = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_Boolean " << retVar << " = lit_false;" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "if (" << lhsVar << " == lit_true || " << rhsVar
                << " == lit_true) { " << retVar << " = lit_true; }"
                << std::endl;
        return retVar;
    }

/* Not */
    void Not::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Not");
        this->json_child(out, indent, "expr_", this->expr_);
        this->json_close(out, indent);
    }
    void Not::getVars(std::vector<std::string> &vars,
            std::vector<std::string> &fields, std::map<std::string,
            std::pair<ClassStruct*, bool>> &varTable, std::map<std::string,
            std::pair<ClassStruct*, bool>> &fieldTable, bool inConstructor,
            bool &failed) {
        std::vector<std::string> fieldCopy = fields, varCopy = vars;
        expr_->getVars(varCopy, fieldCopy, varTable, fieldTable, inConstructor,
                failed);
        for (auto varName : difference(varCopy, vars)) {
            std::cerr << this->getPosition() << "Error: Variable \"" << varName
                    << "\" used before initialization" << std::endl;
            failed = true;
        }
        for (auto fieldName : difference(fieldCopy, fields)) {
            std::cerr << this->getPosition() << "Error: Field \"this."
                    << fieldName << "\" used before initialization"
                    << std::endl;
            failed = true;
        }
    }
    ClassStruct *Not::getType(ClassStruct *thisClass, MethodStruct *thisMethod,
            bool &failed) {
        ClassStruct *exprType;
        exprType = expr_->getType(thisClass, thisMethod, failed);
        if (exprType != nullptr && exprType != this->builtinTypes["Boolean"]) {
            std::cerr << this->getPosition() << "Error: Expression right of "
                    << "'not' is not a Boolean" << std::endl;
            failed = true;
        }
        return this->builtinTypes["Boolean"];
    }
    void Not::generateBranchCode(std::ostream& file, int indent,
            ClassStruct* thisClass, MethodStruct* thisMethod,
            std::string trueLabel, std::string falseLabel) {
        this->expr_->generateBranchCode(file, indent, thisClass, thisMethod,
                falseLabel, trueLabel);
    }
    std::string Not::generateRExprCode(std::ostream &file, int indent,
            ClassStruct *thisClass, MethodStruct *thisMethod) {
        std::string exprVar = this->expr_->generateRExprCode(file, indent,
                thisClass, thisMethod);
        std::string retVar = "temp" + std::to_string(this->tempVarID++);
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "obj_Boolean " << retVar << " = lit_true;" << std::endl;
        for (int i = 0; i < indent; i++) { file << "\t"; }
        file << "if (" << exprVar << " == lit_true) { " << retVar
                << " = lit_false; }" << std::endl;
        return retVar;
    }

}  // namespace AST
