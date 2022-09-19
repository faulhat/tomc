#ifndef DOTListener_HPP
#define DOTListener_HPP

#include <stack>
#include <string>
#include <sstream>
#include <antlr4-runtime/ParserRuleContext.h>
#include "../../grammar/CProgramBaseListener.h"
#include "../../grammar/CProgramParser.h"

using namespace std;
using namespace antlr4;

enum class Shape {
    Box, Blob
};

class DOTListener : public CProgramBaseListener
{
private:
    stack<const string &> nodeTraceback;
    stringstream output;
    int nameNumber;

    string getName();

    void addStatement(const string &statement);

    void addNode(const string &name, const string &label, Shape nodeShape);

    void attachNode(const string &name);

    void pushNode(const string &nodeName);

    void addAttachPush(const string &name, const string &label, Shape nodeShape);

    void addAttachPushNext(const string &label, Shape nodeShape);

    void addAttachTerm(const string &name, const string &label, Shape nodeShape);

    void addAttachNextTerm(const string &label, Shape nodeShape);

    void pop();

public:
    const stringstream &getOutput() const;

    DOTListener();

    void enterProgram(CProgramParser::ProgramContext *ctx) override;
    void exitProgram(CProgramParser::ProgramContext *ctx) override;

    void enterDefineFlag(CProgramParser::DefineFlagContext *ctx) override;

    void enterDefineConst(CProgramParser::DefineConstContext *ctx) override;
    void exitDefineConst(CProgramParser::DefineConstContext *ctx) override;

    void enterDefineMacro(CProgramParser::DefineMacroContext *ctx) override;
    void exitDefineMacro(CProgramParser::DefineMacroContext *ctx) override;

    void enterIfDef(CProgramParser::IfDefContext *ctx) override;

    void enterIfNotDef(CProgramParser::IfNotDefContext *ctx) override;

    void enterUnDef(CProgramParser::UnDefContext *ctx) override;

    void enterIncludeFile(CProgramParser::IncludeFileContext *ctx) override;

    void enterMacroArgList(CProgramParser::MacroArgListContext *ctx) override;

    void enterStatement(CProgramParser::StatementContext *ctx) override;
    void exitStatement(CProgramParser::StatementContext *ctx) override;

    void enterElseStmt(CProgramParser::ElseStmtContext *ctx) override;
    void exitElseStmt(CProgramParser::ElseStmtContext *ctx) override;

    void enterIfStmt(CProgramParser::IfStmtContext *ctx) override;
    void exitIfStmt(CProgramParser::IfStmtContext *ctx) override;

    void enterForLoop(CProgramParser::ForLoopContext *ctx) override;
    void exitForLoop(CProgramParser::ForLoopContext *ctx) override;
};

#endif
