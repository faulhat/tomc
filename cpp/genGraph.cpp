#include "DOTListener.hpp"
#include "../antlrOut/CProgramLexer.h"

int main(int argc, char *argv[])
{
    string sample = "int i = 1;";

    istream *stream;
    if (argc > 1) {
        char *filename = argv[1];
        stream = new ifstream(filename);
    }
    else {
        stream = new istringstream(sample);
    }

    ANTLRInputStream input(*stream);
    CProgramLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    CProgramParser parser(&tokens);

    DOTListener listener;
    tree::ParseTree *programTree = parser.program();
    tree::ParseTreeWalker::DEFAULT.walk(&listener, programTree);

    cout << listener.getOutput();
    return 0;
}
