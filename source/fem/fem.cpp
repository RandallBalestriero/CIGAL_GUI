#include "./fem.h"

alpha::alpha(){
    _Domains=arma::mat (9,4);_Values=arma::vec (9);//definition of the defualt values on each sub domain
   for(int i=0;i<3;++i)
       for(int j=0;j<3;++j){
           _Domains(i*3+j,0)=j/3.0;
           _Domains(i*3+j,1)=i/3.0;
           _Domains(i*3+j,2)=((j+1))/3.0;
           _Domains(i*3+j,3)=((i+1))/3.0;
       }
   _Values[0]=1;
   _Values[1]=10;
   _Values[2]=1;
   _Values[3]=10;
   _Values[4]=1;
   _Values[5]=10;
   _Values[6]=1;
   _Values[7]=10;
   _Values[8]=1;
}


double const& alpha::operator()(arma::vec const& Point)const{//access operator return the value for the sub domain of the asked point
        for(unsigned int i=0;i<_Domains.n_elem;++i)
            if(InSquareDomain(Point,_Domains,i))
                return _Values[i];
        }

double const& alpha::operator()(float const& x,float const& y)const{//same with const value
        for(unsigned int i=0;i<_Domains.n_rows;++i)
            if(InSquareDomain(x,y,_Domains,i))
                return _Values[i];
        }


g::g(){values[0]=0;values[1]=1;values[2]=-1;}

double g::operator()(arma::vec const& p)const{
        if(p[0]==0) return values[0];
        else if(p[0]==1) return values[2];
        else if(p[1]==1) return values[1];
        return 0;
    }

double g::operator()(double const& x,double const& y)const{
        if(x==0) return values[0];
        else if(x==1) return values[2];
        else if(y==1) return values[1];
        return 0;
    }






//#############################################
//
//      FUNCTION TO FILL THE SYSTEM
//
//############################################

//compute the gradient of the plane defined by 3 3D points a,b,c
arma::vec grad(arma::vec const& a,arma::vec const& b,arma::vec const& c){
    arma::vec grad1(2);
    const float a1=(c[2]-a[2])*(b[1]-a[1])-(c[1]-a[1])*(b[2]-a[2]);
    const float a2=-(c[2]-a[2])*(b[0]-a[0])+(b[2]-a[2])*(c[0]-a[0]);
    const float a3=(b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0]);
    grad1[0]=-a1/a3;
    grad1[1]=-a2/a3;
    return grad1;
}

