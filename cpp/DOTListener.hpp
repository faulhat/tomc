#ifndef DOTListener_HPP
#define DOTListener_HPP

#include <stack>
#include <string>
#include <sstream>
#include "../antlrOut/CProgramBaseListener.h"
#include "../antlrOut/CProgramParser.h"

using namespace std;
using namespace antlr4;

// Creates a declaration for the enter method for a given node type in the AST
#define ENTER(NAME) \
    void enter##NAME (CProgramParser::NAME##Context *ctx) override;

// Creates a declaration for the exit method as well
#define ENTER_EXIT(NAME) \
    ENTER(NAME) \
    void exit##NAME (CProgramParser::NAME##Context *ctx) override;

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
    stack<string> nodeTraceback;

    // The output stream. This cannot be determined by the user.
    stringstream output;

    // Incremented each time a name is used for unique name creation.
    int nameNumber;

    // Returns a string for a new name, and increments nameNumber
    string getName();

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
    void addAttachPushNext(const string &label, const Shape &nodeShape = Shape::Box);

    // Same as addAttachPush but minus the push, for use with terminal nodes (leaves)
    void addAttachTerm(const string &name, const string &label, Shape nodeShape);

    // Automatically uses getName()
    void addAttachNextTerm(const string &label, const Shape &nodeShape = Shape::Blob);

    // Shorthand for nodeTraceback.pop()
    void pop();

public:
    // Allows retrieval of output
    // This is const in case a const instance of this class gets passed for ownership reasons.
    string getOutput() const;

    DOTListener();

    // Should automatically create and attach nodes for named tokens
    void visitTerminal(tree::TerminalNode *node) override;

    ENTER_EXIT(Program)
    ENTER(DefineFlag)
    ENTER_EXIT(DefineConst)
    ENTER_EXIT(DefineMacro)
    ENTER(IfDef)
    ENTER(IfNotDef)
    ENTER(UnDef)
    ENTER(IncludeFile)
    ENTER_EXIT(MacroArgList)
    ENTER_EXIT(EvalExpr)
    ENTER_EXIT(ReturnExpr)
    ENTER_EXIT(ElseStmt)
    ENTER_EXIT(IfStmt)
    ENTER_EXIT(ForLoop)
    ENTER_EXIT(WhileLoop)
    ENTER_EXIT(Declaration)
    ENTER(SimpleName)
    ENTER_EXIT(ArrName)
    ENTER_EXIT(SingleDef)
    ENTER_EXIT(MultDecl)
    ENTER_EXIT(MultDef)
    ENTER_EXIT(FnDeclaration)
    ENTER_EXIT(FnImplementation)
    ENTER_EXIT(TypeDefinition)
    ENTER_EXIT(ArgDeclList)
    ENTER_EXIT(Block)
    ENTER_EXIT(SuffixOp)
    ENTER_EXIT(Call)
    ENTER_EXIT(Subscript)
    ENTER_EXIT(MemberGet)
    ENTER_EXIT(CompLiteral)
    ENTER_EXIT(PrefixOp)
    ENTER_EXIT(CCast)
    ENTER_EXIT(MultOp)
    ENTER_EXIT(AddOp)
    ENTER_EXIT(ShiftOp)
    ENTER_EXIT(CompareOp)
    ENTER_EXIT(EqOp)
    ENTER_EXIT(BitAndOp)
    ENTER_EXIT(BitOrOp)
    ENTER_EXIT(LojAndOp)
    ENTER_EXIT(LojOrOp)
    ENTER_EXIT(TernaryOp)
    ENTER_EXIT(SetVal)
    ENTER_EXIT(SimpleType)
    ENTER_EXIT(ArrayType)
    ENTER_EXIT(PtrType)
    ENTER_EXIT(ConstType)
    ENTER_EXIT(ComplexType)
    ENTER_EXIT(AnonType)
    ENTER_EXIT(FnPtrType)
};

#endif
