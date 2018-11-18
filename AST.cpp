#include "AST.h"
namespace AST
{
    void ASTNode::json_indent(std::ostream& out, unsigned int indent){
        for (unsigned int i=0; i<indent; i++){
                out << "  ";
        }
    }
    void ASTNode::json_head(std::ostream& out, unsigned int &indent, std::string node_kind){
        out << "{" << std::endl;
        indent++;
        this->json_indent(out, indent);
        out << "\"kind\": \"" << node_kind << "\"";
    }
    void ASTNode::json_close(std::ostream& out, unsigned int &indent){
        if (indent > 0) indent--;
        out << std::endl;
        this->json_indent(out, indent);
        out << "}";
    }
    void ASTNode::json_child(std::ostream& out, unsigned int indent, std::string field, ASTNode *child){
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": ";
        child->json(out, indent);
    }
    
    void ASTNode::json_string(std::ostream& out, unsigned int indent, std::string field, std::string str){
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": \"" << str << "\"";
    }
    void ASTNode::json_int(std::ostream& out, unsigned int indent, std::string field, unsigned int val){
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": " << val;
    }
    template<class NodeType>
    void ASTNode::json_list(std::ostream& out, unsigned int indent, std::string field, std::vector<NodeType> const& list){
        out << "," << std::endl;
        this->json_indent(out, indent);
        out << "\"" << field << "\": [";
        std::string sep = "";
        for (NodeType node: list) {
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
    void TypedArg::json(std::ostream &out, unsigned int indent){
        this->json_head(out, indent, "TypedArg");
        this->json_string(out, indent, "name_", this->name_);
        this->json_string(out, indent, "type_", this->type_);
        this->json_close(out, indent);
    }
    void Method::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Method");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<TypedArg*>(out, indent, "args_", *this->args_);
        if (this->isTyped_){
            this->json_string(out, indent, "type_", this->type_);
        }
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void Class::json(std::ostream &out, unsigned int indent){
        this->json_head(out, indent, "Class");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<TypedArg*>(out, indent, "args_", *this->args_);
        this->json_string(out, indent, "extends_", this->extends_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_list<Method*>(out, indent, "mthds_", *this->mthds_);
        this->json_close(out, indent);
    }
    void Program::json(std::ostream &out, unsigned int indent){
        this->json_head(out, indent, "Program");
        this->json_list<Class*>(out, indent, "classes_", *this->classes_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void If::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "If");
        this->json_child(out, indent, "cond_", this->cond_);
        this->json_list<Statement*>(out, indent, "if_true_", *this->if_true_stmts_);
        this->json_list<Statement*>(out, indent, "if_false_", *this->if_false_stmts_);
        this->json_close(out, indent);
    }
    void While::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "While");
        this->json_child(out, indent, "cond_", this->cond_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void LExpr::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "LExpr");
        if (this->isField_)
            this->json_child(out, indent, "obj_", this->obj_);
        this->json_string(out, indent, "name_", this->name_);
        this->json_close(out, indent);
    }
    void Assignment::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Assignment");
        this->json_child(out, indent, "l_expr_", this->l_expr_);
        if (this->isTyped_)
            this->json_string(out, indent, "type_", this->type_);
        this->json_child(out, indent, "r_expr_", this->r_expr_);
        this->json_close(out, indent);
    }
    void Return::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Return");
        if (!this->returnsNone_)
            this->json_child(out, indent, "r_expr_", this->r_expr_);
        this->json_close(out, indent);
    }
    void TypeAlt::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "TypeAlt");
        this->json_string(out, indent, "name_", this->name_);
        this->json_string(out, indent, "type_", this->type_);
        this->json_list<Statement*>(out, indent, "stmts_", *this->stmts_);
        this->json_close(out, indent);
    }
    void Typecase::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Typecase");
        this->json_child(out, indent, "expr_", this->expr_);
        this->json_list<TypeAlt*>(out, indent, "alternatives_", *this->alternatives_);
        this->json_close(out, indent);
    }
    void IntLit::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Int");
        this->json_int(out, indent, "val_", this->val_);
        this->json_close(out, indent);
    }
    void StrLit::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "String");
        this->json_string(out, indent, "text_", this->text_);
        this->json_close(out, indent);
    }
    void Call::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Call");
        this->json_child(out, indent, "obj_", this->obj_);
        this->json_string(out, indent, "mthd_", this->mthd_);
        this->json_list<RExpr*>(out, indent, "args_", *this->args_);
        this->json_close(out, indent);
    }
    void Constructor::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Constructor");
        this->json_string(out, indent, "name_", this->name_);
        this->json_list<RExpr*>(out, indent, "args_", *this->args_);
        this->json_close(out, indent);
    }
    void And::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "And");
        this->json_child(out, indent, "lhs_", this->lhs_);
        this->json_child(out, indent, "rhs_", this->rhs_);
        this->json_close(out, indent);
    }
    void Or::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Or");
        this->json_child(out, indent, "lhs_", this->lhs_);
        this->json_child(out, indent, "rhs_", this->rhs_);
        this->json_close(out, indent);
    }
    void Not::json(std::ostream &out, unsigned int indent) {
        this->json_head(out, indent, "Not");
        this->json_child(out, indent, "expr_", this->expr_);
        this->json_close(out, indent);
    }
}