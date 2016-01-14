#include "./2D_function_parser.h"

inline bool IsFunc2D(char c){
    if(isalpha(c)&&c!='x'&& c!='(' && c!='y') return 1;
    return 0;
}



inline Node2D* Eval2D(std::string & s){
    std::string T;
    if(isdigit(s[0])){
        T=GetNumber(s);
        if(s.size()>0){
            if(IsOp(s[0])){
                char Op=s[0];
                s.erase(s.begin());
                return new OperatorNode2D(T,s,Op);
            }
        }
        else return new Leaf2D(T);
    }
    else if(s[0]=='x'||s[0]=='y'){
        if(s.size()==1) return new Leaf2D(s);
        else{
            T+=s[0];
            s.erase(s.begin());
            char Op=s[0];
            s.erase(s.begin());
            return new OperatorNode2D(T,s,Op);
        }
    }
    else if(IsFunc(s[0])){
        T=GetFunc(s);
        if(OnlyFunc(T+s)){return new FunctionNode2D(T);}
        else{
            char op=s[0];
            s.erase(s.begin());
            return new OperatorNode2D(T,s,op);
        }
    }
    if(OnlyParenth(s)){
        s.erase(s.begin());
        s.pop_back();
        return Eval2D(s);
    }
    T=GetParenth(s);
    char op=s[0];
    s.erase(s.begin());
    return new OperatorNode2D(T,s,op);
}






NormalNode2D::NormalNode2D(float const& a,float const& b):mean(a),sigma(b){}

float NormalNode2D::operator()(float const&,float const&)const{
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 B(seed1);
    std::normal_distribution<float> distribution(mean,sigma);
    float v=distribution(B);
    return v;
}



FunctionNode2D::FunctionNode2D(std::string & s){
    if(s[0]=='N'){
        f=Identity;
        s.erase(s.begin());s.erase(s.begin());
        float n=stof(GetNumber(s));s.erase(s.begin());
        float sigma=stof(GetNumber(s));
        _Inside=new NormalNode2D(n,sigma);
    }
    else{
        if(s[0]=='e' && s[1]=='x' && s[2]=='p') f=exp;
        else if(s[0]=='s' && s[1]=='i' && s[2]=='n') f=sin;
        else if(s[0]=='c' && s[1]=='o' && s[2]=='s') f=cos;
        else if(s[0]=='l' && s[1]=='o' && s[2]=='g') f=log;
        std::string T=GetContentFunc(s);
        _Inside=Eval2D(T);
    }
}
float FunctionNode2D::operator()(float const& i,float const& j)const{return f((*_Inside)(i,j));}



//  LEAF NODE


Leaf2D::Leaf2D(std::string const& s){
    if(s[0]=='x'){_Constant=0;_X=1;}
    else if(s[0]=='y'){_Constant=0;_X=0;}
    else {_Constant=1;V=stof(s);}
}
float Leaf2D::operator()(float const& i,float const& j)const{
    if(_Constant) return V;else if(_X) return i;else return j;}


OperatorNode2D::OperatorNode2D(std::string & exp1,std::string & exp2,char const& op):_Op(op){
    _Left=Eval2D(exp1);
    _Right=Eval2D(exp2);
}

float OperatorNode2D::operator()(float const& i,float const& j)const{
    switch(_Op){
    case '+':
        return (*_Left)(i,j)+(*_Right)(i,j);
        break;
    case '-':
        return (*_Left)(i,j)-(*_Right)(i,j);
        break;
    case '*':
        return (*_Left)(i,j)*(*_Right)(i,j);
        break;
    case '/':
        return (*_Left)(i,j)/(*_Right)(i,j);
        break;
    default:
        return pow((*_Left)(i,j),(*_Right)(i,j));
    }
}


Tree2D::Tree2D(std::string & s){
    _Base=Eval2D(s);
}
float Tree2D::operator()(float const & i,float const& j)const{
    return (*_Base)(i,j);
}


