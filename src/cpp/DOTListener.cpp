#include <vector>
#include "DOTListener.hpp"

const string &DOTListener::getName()
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
    output << "\t" << name << "[ label=\"" << label << "\"";
    if (nodeShape == Shape::Box) {
        output << ", shape=\"box\"";
    }

    output << " ];\n";
}

void DOTListener::attachNode(const string &name)
{
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

void DOTListener::addAttachPushNext(const string &label, Shape nodeShape)
{
    addAttachPush(getName(), label, nodeShape);
}

void DOTListener::addAttachTerm(const string &name, const string &label, Shape nodeShape)
{
    addNode(name, label, nodeShape);
    attachNode(name);
}

void DOTListener::addAttachNextTerm(const string &label, Shape nodeShape)
{
    addAttachTerm(getName(), label, nodeShape);
}

void DOTListener::pop()
{
    return nodeTraceback.pop();
}

const string &DOTListener::getOutput() const
{
    return output.str();
}

DOTListener::DOTListener()
    : nameNumber(0)
{}

void DOTListener::visitTerminal(tree::TerminalNode *node)
{
    addAttachNextTerm(node->getText(), Shape::Blob);
}

void DOTListener::enterProgram(CProgramParser::ProgramContext *ctx)
{
    output << "digraph {\n";
    addStatement("rankdir=\"BT\"");
    addAttachPushNext("root", Shape::Box);
}

void DOTListener::exitProgram(CProgramParser::ProgramContext *ctx)
{
    output << "}";
    nodeTraceback.pop();

    // If all has gone correctly, the root node was just popped from the stack.
    assert(nodeTraceback.empty());
}

void DOTListener::enterDefineFlag(CProgramParser::DefineFlagContext *ctx)
{
    stringstream label;
    label << "#define " << ctx->Name()->getText() << " (flag)";
    addAttachNextTerm(label.str(), Shape::Box);
}

void DOTListener::enterDefineConst(CProgramParser::DefineConstContext *ctx)
{
    stringstream label;
    label << "#define " << ctx->Name()->getText() << " (constant)";
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::exitDefineConst(CProgramParser::DefineConstContext *ctx) { pop(); }

void DOTListener::enterDefineMacro(CProgramParser::DefineMacroContext *ctx)
{
    stringstream label;
    label << "#define " << ctx->Name()->getText() << " (macro)";
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::enterDefineMacro(CProgramParser::DefineMacroContext *ctx) { pop(); }

void DOTListener::enterIfDef(CProgramParser::IfDefContext *ctx)
{
    stringstream label;
    label << "#ifdef " << ctx->Name()->getText();
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::enterIfNotDef(CProgramParser::IfNotDefContext *ctx)
{
    stringstream label;
    label << "#ifndef " << ctx->Name()->getText();
    addAttachPushNext(label.str(), Shape::Box);
}

void DOTListener::enterUnDef(CProgramParser::UnDefContext *ctx)
{
    stringstream label;
    label << "#undef " << ctx->Name()->getText();
    addAttachNextTerm(label.str(), Shape::Box);
}

void DOTListener::enterIncludeFile(CProgramParser::IncludeFileContext *ctx)
{
    stringstream label;
    label << "#include " << ctx->String()->getText();
    addAttachNextTerm(label.str(), Shape::Box);
}

void DOTListener::enterMacroArgList(CProgramParser::MacroArgListContext *ctx)
{
    addAttachPushNext("Macro args", Shape::Box);
}

void DOTListener::exitMacroArgList(CProgramParser::MacroArgListContext *ctx) { pop(); }

void DOTListener::enterStatement(CProgramParser::StatementContext *ctx)
{
    addAttachPushNext("S", Shape::Box);
}

void DOTListener::exitStatement(CProgramParser::StatementContext *ctx) { pop(); }

void DOTListener::enterElseStmt(CProgramParser::ElseStmtContext *ctx) {
    addAttachPushNext("Else", Shape::Box);
}

void DOTListener::exitElseStmt(CProgramParser::ElseStmtContext *ctx) { pop(); }

void DOTListener::enterIfStmt(CProgramParser::IfStmtContext *ctx) {
    addAttachPushNext("If", Shape::Box);
}

void DOTListener::exitIfStmt(CProgramParser::IfStmtContext *ctx) { pop(); }

void DOTListener::enterForLoop(CProgramParser::ForLoopContext *ctx) {
    addAttachPushNext("For", Shape::Box);
}

void DOTListener::exitForLoop(CProgramParser::ForLoopContext *ctx) { pop(); }

void DOTListener::enterWhileLoop(CProgramParser::WhileLoopContext *ctx) {
    addAttachPushNext("While", Shape::Box);
}

void DOTListener::exitWhileLoop(CProgramParser::WhileLoopContext *ctx) { pop(); }

void DOTListener::enterDeclaration(CProgramParser::DeclarationContext *ctx) {
    addAttachPushNext("Declaration", Shape::Box);
}

void DOTListener::exitDeclaration(CProgramParser::DeclarationContext *ctx) { pop(); }

void DOTListener::enterVarName(CProgramParser::VarNameContext *ctx) {
    addAttachNextTerm(ctx->getText(), Shape::Box);
}
