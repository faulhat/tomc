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

    void addStatement(const string &statement);

    void addNode(const string &name, const string &label, Shape nodeShape);

    void attachNode(const string &name);

    void pushNode(const string &nodeName);

    void addAttachPush(const string &name, const string &label, Shape nodeShape);

    void pushDummy();

    void addAttachTerm(const string &name, const string &label, Shape nodeShape);

public:
    const stringstream &getOutput() const;

    DOTListener();

    string getName();

    void enterProgram(CProgramParser::ProgramContext *ctx) override;
    void exitProgram(CProgramParser::ProgramContext *ctx) override;

    void exitEveryRule(ParserRuleContext *ctx) override;

    void enterDefineFlag(CProgramParser::DefineFlagContext *ctx) override;

    void enterDefineConst(CProgramParser::DefineConstContext *ctx) override;

    void enterDefineMacro(CProgramParser::DefineMacroContext *ctx) override;

    void enterIfDef(CProgramParser::IfDefContext *ctx) override;

    void enterIfNotDef(CProgramParser::IfNotDefContext *ctx) override;
};

#endif
