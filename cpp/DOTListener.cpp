#include <vector>
#include <boost/algorithm/string.hpp>
#include "DOTListener.hpp"

// I don't want to type these every time
// This macro produces enter and exit method defintions for non-terminal nodes in the AST.
// NAME is the node type and LABEL is what should go on the graph.
#define NODE_GEN_NONTERM(NAME, LABEL) \
    void DOTListener::enter##NAME (CProgramParser::NAME##Context *ctx) { \
        addAttachPushNext(LABEL); \
    } \
    \
    void DOTListener::exit##NAME (CProgramParser::NAME##Context *ctx) { pop(); }

// The same thing but for terminal nodes
#define NODE_GEN_TERM(NAME, EXPR) \
    void DOTListener::enter##NAME (CProgramParser::NAME##Context *ctx) { \
        addAttachNextTerm(EXPR); \
    }

#define NODE_GEN_VERBATIM(NAME) NODE_GEN_TERM(NAME, ctx->getText())

string DOTListener::getName()
{
    stringstream name;
    name << "n" << this->nameNumber++;
    return name.str();
}

void DOTListener::addStatement(const string &statement)
{
    output << "\t" << statement << ";\n";
}

void DOTListener::addNode(const string &name, const string &label, Shape nodeShape)
{
    string escLabel(label);
    boost::replace_all(escLabel, "\"", "\\\"");
    output << "\t" << name << "[ label=\"" << escLabel << "\"";
    if (nodeShape == Shape::Box) {
        output << ", shape=\"box\"";
    }

    output << " ];\n";
}

void DOTListener::attachNode(const string &name)
{
    if (!nodeTraceback.empty())
        output << "\t" << name << " -> " << nodeTraceback.top() << ";\n";
}

void DOTListener::pushNode(const string &nodeName)
{
    nodeTraceback.push(nodeName);
}

void DOTListener::addAttachPush(const string &name, const string &label, Shape nodeShape)
{
    addNode(name, label, nodeShape);
    attachNode(name);
    pushNode(name);
}

void DOTListener::addAttachPushNext(const string &label, const Shape &nodeShape)
{
    addAttachPush(getName(), label, nodeShape);
}

void DOTListener::addAttachTerm(const string &name, const string &label, Shape nodeShape)
{
    addNode(name, label, nodeShape);
    attachNode(name);
}

void DOTListener::addAttachNextTerm(const string &label, const Shape &nodeShape)
{
    addAttachTerm(getName(), label, nodeShape);
}

void DOTListener::pop()
{
    return nodeTraceback.pop();
}

string DOTListener::getOutput() const
{
    return output.str();
}

DOTListener::DOTListener()
    : nameNumber(0)
{}

void DOTListener::enterProgram(CProgramParser::ProgramContext *ctx)
{
    output << "digraph {\n";
    addStatement("rankdir=\"BT\"");
    addAttachPushNext("root", Shape::Box);
}

void DOTListener::exitProgram(CProgramParser::ProgramContext *ctx)
{
    output << "}\n";
    nodeTraceback.pop();

    // If all has gone correctly, the root node was just popped from the stack.
    assert(nodeTraceback.empty());
}

void DOTListener::enterDefineFlag(CProgramParser::DefineFlagContext *ctx)
{
    stringstream label;
    label << "#define " << ctx->PRE_Name()->getText() << " (flag)";
    addAttachNextTerm(label.str(), Shape::Box);
}

void DOTListener::enterDefineConst(CProgramParser::DefineConstContext *ctx)
{
    stringstream label;
    label << "#define " << ctx->PRE_Name()->getText() << " (constant)";
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::exitDefineConst(CProgramParser::DefineConstContext *ctx) { pop(); }

void DOTListener::enterIfDef(CProgramParser::IfDefContext *ctx)
{
    stringstream label;
    label << "#ifdef " << ctx->PRE_Name()->getText();
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::enterIfNotDef(CProgramParser::IfNotDefContext *ctx)
{
    stringstream label;
    label << "#ifndef " << ctx->PRE_Name()->getText();
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::enterUnDef(CProgramParser::UnDefContext *ctx)
{
    stringstream label;
    label << "#undef " << ctx->PRE_Name()->getText();
    addAttachNextTerm(label.str(), Shape::Box);
}

NODE_GEN_VERBATIM(IncludeFile)
NODE_GEN_VERBATIM(IncludeFromPath)

NODE_GEN_NONTERM(EvalExpr, "S")
NODE_GEN_NONTERM(ReturnExpr, "RETURN")
NODE_GEN_NONTERM(ElseStmt, "ELSE")
NODE_GEN_NONTERM(IfStmt, "IF")
NODE_GEN_NONTERM(ForLoop, "FOR")
NODE_GEN_NONTERM(WhileLoop, "WHILE")
NODE_GEN_VERBATIM(SimpleName)
NODE_GEN_NONTERM(ArrName, "[]")

#define NODE_GEN_OP(NAME) NODE_GEN_NONTERM(NAME, ctx->op->getText())

NODE_GEN_NONTERM(FnDeclaration, "FN")
NODE_GEN_NONTERM(FnImplementation, "FN")
NODE_GEN_NONTERM(TypeDefinition, "TYPE")
NODE_GEN_NONTERM(ArgDeclList, "ARGS")
NODE_GEN_NONTERM(Block, "{ ... }")
NODE_GEN_OP(SuffixOp)
NODE_GEN_NONTERM(Call, "CALL")
NODE_GEN_NONTERM(Subscript, "[]")
NODE_GEN_OP(MemberGet)
NODE_GEN_NONTERM(CompLiteral, "COMPLEX")
NODE_GEN_OP(PrefixOp)
NODE_GEN_NONTERM(CCast, "CAST")
NODE_GEN_OP(MultOp)
NODE_GEN_OP(AddOp)
NODE_GEN_OP(ShiftOp)
NODE_GEN_OP(CompareOp)
NODE_GEN_OP(EqOp)
NODE_GEN_NONTERM(BitAndOp, "&")
NODE_GEN_NONTERM(BitOrOp, "|")
NODE_GEN_NONTERM(LojAndOp, "&&")
NODE_GEN_NONTERM(LojOrOp, "||")
NODE_GEN_NONTERM(TernaryOp, "?:")
NODE_GEN_NONTERM(SetVal, "SET")
NODE_GEN_NONTERM(Declaration, "DECL")
NODE_GEN_NONTERM(Definition, "DEF")

void DOTListener::enterSimpleType(CProgramParser::SimpleTypeContext *ctx)
{
    addAttachPushNext("TYPE");
    addAttachNextTerm(ctx->getText());
    pop();
}

NODE_GEN_NONTERM(PtrType, "* TYPE")
NODE_GEN_NONTERM(ConstType, "CONST")
NODE_GEN_NONTERM(ComplexType, ctx->kind->getText())
NODE_GEN_NONTERM(AnonType, ctx->kind->getText())
NODE_GEN_NONTERM(FnPtrType, "FN *")
NODE_GEN_NONTERM(ArrayType, "[] TYPE")
NODE_GEN_VERBATIM(StrAtom)
NODE_GEN_VERBATIM(NameAtom)
NODE_GEN_VERBATIM(IntAtom)
NODE_GEN_VERBATIM(FloatAtom)
NODE_GEN_VERBATIM(CharAtom)
