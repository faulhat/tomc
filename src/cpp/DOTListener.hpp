#ifndef DOTListener_HPP
#define DOTListener_HPP

#include <stack>
#include <string>
#include <iostream>
#include "../../grammar/CProgramBaseListener.h"

using namespace std;

class DOTListener : public CProgramBaseListener
{
private:
    stack<string> nodeTraceback;
    ostream& builder;
    int nameNumber;

public:
    DOTListener(ostream &_builder)
        : builder(_builder)
        , nameNumber(0)
    {}
};

#endif
