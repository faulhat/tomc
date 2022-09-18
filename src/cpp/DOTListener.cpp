#include "DOTListener.hpp"

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

void DOTListener::pushDummy()
{
    nodeTraceback.push("");
}

void DOTListener::addAttachTerm(const string &name, const string &label, Shape nodeShape)
{
    addNode(name, label, nodeShape);
    attachNode(name);
    pushDummy();
}

const stringstream &DOTListener::getOutput() const
{
    return output;
}

DOTListener::DOTListener()
    : nameNumber(0)
{}

string DOTListener::getName()
{
    stringstream stream;
    stream << "n" << ++this->nameNumber;
    return stream.str();
}

void DOTListener::enterProgram(CProgramParser::ProgramContext *)
{
    output << "digraph {\n";
    addStatement("rankdir=\"BT\"");
    addStatement("root [label=\"Program\", shape=\"box\"]");
    pushNode("root");
}

void DOTListener::exitProgram(CProgramParser::ProgramContext *)
{
    output << "}";
    nodeTraceback.pop();
    assert(nodeTraceback.empty());
}

void DOTListener::exitEveryRule(ParserRuleContext *ctx)
{
    nodeTraceback.pop();
}

void DOTListener::enterDefineFlag(CProgramParser::DefineFlagContext *ctx)
{
    string name = getName();
    stringstream label;
    label << "#define " << ctx->Name() << " (flag)";
    addAttachTerm(name, label.str(), Shape::Box);
}

void DOTListener::enterDefineConst(CProgramParser::DefineConstContext *ctx)
{
    string name = getName();
    stringstream label;
    label << "#define " << ctx->Name() << " (constant)";
    addAttachPush(name, label.str(), Shape::Box);
}

void DOTListener::enterDefineMacro(CProgramParser::DefineMacroContext *ctx)
{
    string name = getName();
    stringstream label;
    label << "#define " << ctx->Name() << " (macro)";
    addAttachPush(name, label.str(), Shape::Box);
}

void DOTListener::enterIfDef(CProgramParser::IfDefContext *ctx)
{
    string name = getName();
    stringstream label;
    label << "#ifdef " << ctx->Name();
    addAttachPush(name, label.str(), Shape::Box);
}

void DOTListener::enterIfNotDef(CProgramParser::IfNotDefContext *ctx)
{
    string name = getName();
    stringstream label;
    label << "#ifndef " << ctx->Name();
    addAttachTerm(name, label.str(), Shape::Box);
}
