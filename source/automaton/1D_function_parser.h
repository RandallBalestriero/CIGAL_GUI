 #ifndef BASE_HEADER_FILE
#define BASE_HEADER_FILE

#include <random>
#include <string>
#include <chrono>
#include <armadillo>


class Node{
public:
    virtual float operator()(float const&)const=0;
    virtual std::string toString()const=0;
};

Node* Eval(std::string &s);
bool IsOp(char);
bool IsFunc(char);
bool OnlyFunc(std::string);
bool OnlyParenth(std::string);
std::string GetFunc(std::string & );
std::string GetContentFunc(std::string &);
std::string GetNumber(std::string & );
std::string GetParenth(std::string & );
double Identity(double);


class NormalNode:public Node{
public:
    float mean,sigma;
    NormalNode(float const&,float const&);
    float operator()(float const&)const;
    std::string toString()const{return "N("+std::to_string(mean)+","+std::to_string(sigma)+")";}
};


class FunctionNode:public Node{
public:
    std::string _Name;
    double(*f)(double);
    Node* _Inside;
    FunctionNode(std::string &);
    float operator()(float const&)const;
    std::string toString()const{return _Name+"("+_Inside->toString()+")";}
};


//  LEAF NODE

class Leaf:public Node{
public:
    float V;
    bool _Constant;
    Leaf(std::string const&);
    float  operator()(float const&)const;
    std::string toString()const{if(_Constant) return std::to_string(V);else return "x";}
};

//      OPERATOR NODE

class OperatorNode:public Node{
public:
    char _Op;
    Node* _Right;
    Node* _Left;
    OperatorNode(std::string &,std::string & ,char const&);

    float operator()(float const&)const;
    std::string toString()const{return "("+_Left->toString()+std::string(1,_Op)+_Right->toString()+")";}
};

class Tree{
public:
    Node * _Base;
    Tree(std::string &);
    float operator()(float const & )const;
    std::string toString()const{return _Base->toString();}
    arma::mat operator()(arma::mat const& matrix){
        arma::mat d(matrix.n_rows,matrix.n_cols);
        for(unsigned int i=0;i<matrix.n_rows;++i)
            for(unsigned int j=0;j<matrix.n_cols;++j)
                d(i,j)=(*_Base)(matrix(i,j));
        return d;
    }
    arma::vec operator()(arma::vec const& matrix)const{
        arma::vec d(matrix.n_elem);
        for(unsigned int i=0;i<matrix.n_elem;++i) d[i]=(*_Base)(matrix[i]);
        return d;
    }

};

#endif
