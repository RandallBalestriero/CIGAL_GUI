#include "./1D_function_parser.h"

double Identity(double a){
    return a;
}



bool IsOp(char c){
    if(c=='+'||c=='-'||c=='/'||c=='*' || c=='^') return 1;
    return 0;
}

bool IsFunc(char c){
    if(isalpha(c)&&c!='x'&& c!='(') return 1;
    return 0;
}

bool OnlyFunc(std::string s){
    int NumberOfP=0;
    unsigned int S;
    unsigned int Start=s.find_first_of('(');
    for(int i=Start;;++i){
        if(s[i]=='(') NumberOfP++;
        else if(s[i]==')') NumberOfP--;
        if(NumberOfP==0){S=i; break;}
    }
    if(S==(s.size()-1)) return 1;
    return 0;
}

bool OnlyParenth(std::string s){
    int NumberOfP=0;
    unsigned int S;
    for(int i=0;;++i){
        if(s[i]=='(') NumberOfP++;
        else if(s[i]==')') NumberOfP--;
        if(NumberOfP==0){S=i; break;}
    }
    if(S==s.size()-1) return 1;
    return 0;
}

std::string GetFunc(std::string & s){
    //return s;
    std::string T;
    int NumberOfP=0;
    unsigned int Start=s.find_first_of('(');
    for(unsigned int i=0;i<Start;++i){
        T+=s[0];
        s.erase(s.begin());
    }
    for(;;){
        T+=s[0];
        if(s[0]=='('){ NumberOfP++;}
        else if(s[0]==')') NumberOfP--;
        s.erase(s.begin());
        if(NumberOfP==0) break;
    }
    return T;
}

//  function that returns what is inside the next parenthesis
std::string GetParenth(std::string & s){
    std::string T;
    int NumberOfP=0;
    for(;;){
        T+=s[0];
        if(s[0]=='('){ NumberOfP++;}
        else if(s[0]==')') NumberOfP--;
        s.erase(s.begin());
        if(NumberOfP==0) break;
    }
    T.erase(T.begin());
    T.pop_back();
    return T;
}

std::string GetContentFunc(std::string & s){
    std::string T;
    T=s.substr(s.find_first_of('(')+1,s.find_last_of(')'));
    T.pop_back();
    return T;
}

std::string GetNumber(std::string & s){
    std::string c;
    while(s.size()>0){
        if(IsOp(s[0])|| IsFunc(s[0]) || s[0]==',') break;
        c+=s[0];
        s.erase(s.begin());
    }
    return c;
}


Node* Eval(std::string & s){
    std::string T;
    if(isdigit(s[0])){
        T=GetNumber(s);
        if(s.size()>0){
            if(IsOp(s[0])){
                char Op=s[0];
                s.erase(s.begin());
                return new OperatorNode(T,s,Op);
            }
        }
        else return new Leaf(T);
    }
    if(s[0]=='x'){
        if(s.size()==1) return new Leaf(s);
        else{
            T+=s[0];
            s.erase(s.begin());
            char Op=s[0];
            s.erase(s.begin());

            return new OperatorNode(T,s,Op);
        }
    }
    if(IsFunc(s[0])){
        T=GetFunc(s);
        if(OnlyFunc(T+s)){return new FunctionNode(T);}
        else{
            char op=s[0];
            s.erase(s.begin());
            return new OperatorNode(T,s,op);
        }
    }
    if(s[0]=='('){
        if(OnlyParenth(s)){
            s.erase(s.begin());
            s.pop_back();
            return Eval(s);
        }
        else{
            T=GetParenth(s);
            char op=s[0];
            s.erase(s.begin());
            return new OperatorNode(T,s,op);
        }
    }
   // if(s[0]=='-'){
        T+="0";
        char op=s[0];
        s.erase(s.begin());
        return new OperatorNode(T,s,op);

}






NormalNode::NormalNode(float const& a,float const& b):mean(a),sigma(b){}

float NormalNode::operator()(float const&)const{
    unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 B(seed1);
    std::normal_distribution<float> distribution(mean,sigma);
    float v=distribution(B);
    return v;
}



FunctionNode::FunctionNode(std::string & s){
        if(s[0]=='N'){
            f=Identity;
            s.erase(s.begin());s.erase(s.begin());
            float n=stof(GetNumber(s));s.erase(s.begin());
            float sigma=stof(GetNumber(s));
            _Inside=new NormalNode(n,sigma);
        }
        else{
            if(s[0]=='e' && s[1]=='x' && s[2]=='p'){ f=exp;_Name="Exp";}
            else if(s[0]=='s' && s[1]=='i' && s[2]=='n'){ f=sin;_Name="Sin";}
            else if(s[0]=='c' && s[1]=='o' && s[2]=='s'){ f=cos;_Name="Cos";}
            else if(s[0]=='l' && s[1]=='o' && s[2]=='g'){ f=log;_Name="Log";}
            std::string T=GetContentFunc(s);
           _Inside=Eval(T);
        }
    }
float FunctionNode::operator()(float const& i)const{return f((*_Inside)(i));}



//  LEAF NODE


Leaf::Leaf(std::string const& s){
        if(s[0]=='x')  _Constant=0;
        else {_Constant=1;V=stof(s);}
    }
float Leaf::operator()(float const& i)const{
        if(_Constant) return V;else return i;}


OperatorNode::OperatorNode(std::string & exp1,std::string & exp2,char const& op):_Op(op){
        _Left=Eval(exp1);
        _Right=Eval(exp2);
    }

float OperatorNode::operator()(float const& i)const{
        switch(_Op){
        case '+':
            return (*_Left)(i)+(*_Right)(i);
            break;
        case '-':
            return (*_Left)(i)-(*_Right)(i);
            break;
        case '*':
            return (*_Left)(i)*(*_Right)(i);
            break;
        case '/':
            return (*_Left)(i)/(*_Right)(i);
            break;
        default:
            return pow((*_Left)(i),(*_Right)(i));
        }
    }


Tree::Tree(std::string & s){
        _Base=Eval(s);
    }
float Tree::operator()(float const & i)const{
        return (*_Base)(i);
}



