#ifndef BASE2_HEADER_FILE
#define BASE2_HEADER_FILE

#include "./1D_function_parser.h"

#include <random>
#include <string>
#include <chrono>


class Node2D{
public:
    virtual float operator()(float const&,float const&)const=0;
};

inline Node2D* Eval2D(std::string &s);

inline bool IsFunc2D(char);


class NormalNode2D:public Node2D{
public:
    float mean,sigma;
    NormalNode2D(float const&,float const&);
    float operator()(float const&,float const&)const;
};


class FunctionNode2D:public Node2D{
public:
    double(*f)(double);
    Node2D* _Inside;
    FunctionNode2D(std::string &);
    float operator()(float const&,float const&)const;
};


//  LEAF NODE

class Leaf2D:public Node2D{
public:
    float V,_X;
    bool _Constant;
    Leaf2D(std::string const&);
    float  operator()(float const&,float const&)const;
};

//      OPERATOR NODE

class OperatorNode2D:public Node2D{
public:
    char _Op;
    Node2D* _Right;
    Node2D* _Left;
    OperatorNode2D(std::string &,std::string & ,char const&);

    float operator()(float const&,float const&)const;
};

class Tree2D{
public:
    Node2D * _Base;
    Tree2D(std::string &);
    float operator()(float const & ,float const&)const;

};

#endif
