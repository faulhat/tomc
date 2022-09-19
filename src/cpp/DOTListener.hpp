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

// Generally, the Box shape should be used for special constructions.
// Atoms and variable, function, and type names should use the Blob shape.
enum class Shape {
    Box,
    Blob, // Graphviz default
};

class DOTListener : public CProgramBaseListener
{
private:
    // Traces back up the tree
    stack<const string &> nodeTraceback;

    // The output stream. This cannot be determined by the user.
    stringstream output;

    // Incremented each time a name is used for unique name creation.
    int nameNumber;

    // Returns a string for a new name, and increments nameNumber
    const string &getName();

    // Adds a complete statement to the DOT output
    void addStatement(const string &statement);

    // Constructs a node
    void addNode(const string &name, const string &label, Shape nodeShape);

    // Attaches a node to the last node on the stack
    void attachNode(const string &name);

    // Pushes the node onto the stack, so that calls to attachNode() will connect to this node
    // until it is popped or superseded.
    void pushNode(const string &nodeName);

    // Combines the top three
    void addAttachPush(const string &name, const string &label, Shape nodeShape);

    // Uses getName() for the name parameter automatically.
    // This works fine because the actual node names are only for internal use,
    // and have no bearing on the actual graph.
    void addAttachPushNext(const string &label, Shape nodeShape);

    // Same as addAttachPush but minus the push, for use with terminal nodes (leaves)
    void addAttachTerm(const string &name, const string &label, Shape nodeShape);

    // Automatically uses getName()
    void addAttachNextTerm(const string &label, Shape nodeShape);

    // Shorthand for nodeTraceback.pop()
    void pop();

public:
    // Allows retrieval of output
    // This is const in case a const instance of this class gets passed for ownership reasons.
    const string &getOutput() const;

    DOTListener();

    // Should automatically create and attach nodes for named tokens
    void visitTerminal(tree::TerminalNode *node) override;

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
    void exitMacroArgList(CProgramParser::MacroArgListContext *ctx) override;

    void enterStatement(CProgramParser::StatementContext *ctx) override;
    void exitStatement(CProgramParser::StatementContext *ctx) override;

    void enterElseStmt(CProgramParser::ElseStmtContext *ctx) override;
    void exitElseStmt(CProgramParser::ElseStmtContext *ctx) override;

    void enterIfStmt(CProgramParser::IfStmtContext *ctx) override;
    void exitIfStmt(CProgramParser::IfStmtContext *ctx) override;

    void enterForLoop(CProgramParser::ForLoopContext *ctx) override;
    void exitForLoop(CProgramParser::ForLoopContext *ctx) override;

    void enterWhileLoop(CProgramParser::WhileLoopContext *ctx) override;
    void exitWhileLoop(CProgramParser::WhileLoopContext *ctx) override;

    void enterDeclaration(CProgramParser::DeclarationContext *ctx) override;
    void exitDeclaration(CProgramParser::DeclarationContext *ctx) override;

    void enterVarName(CProgramParser::VarNameContext *ctx) override;
};

#endif
