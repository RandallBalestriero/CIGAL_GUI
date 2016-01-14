#ifndef GEOMETRY_HEADER_FILE
#define GEOMETRY_HEADER_FILE

#include <iostream>
#include <cmath>
#include <armadillo>

inline void Normalize(arma::mat & matrix){
    matrix-=arma::min(arma::min(matrix));
    matrix/=arma::max(arma::max(matrix));
}

inline void Normalize(std::vector<arma::vec> & vec){
    double  Max=arma::max(vec[0]),Min=arma::min(vec[0]);
    for(unsigned int i=1;i<vec.size();++i){
        if(arma::max(vec[i]>Max)) Max=arma::max(vec[i]);
        else if(arma::min(vec[i]<Min)) Min=arma::min(vec[i]);
    }
    for(unsigned int i=0;i<vec.size();++i){
        vec[i]-=Min;
        vec[i]/=Max;
    }
}

inline int B(float x){
    return 255*std::max(0.,std::min(std::min(0.5+4*x,1.),2.5-4*x));
}

inline int G(float x){
    return 255*std::max(0.,std::min(std::min(4*x-0.5,1.),3.5-4*x));
}

inline int R(float x){
    return 255*std::max(0.,std::min(std::min(4*x-1.5,1.),4.5-4*x));
}

template<typename T=float>
class Point2{
public:
    T x,y;
    Point2(T const& a=0,T const& b=0):x(a),y(b){};
    Point2<T>& operator=(T const& a){x=a;y=a;return *this;}
    template<typename T2>
    T operator*(Point2<T2> const& other){return x*other.y-y*other.x;}
//    template<typename T2>
    Point2<T> operator+(Point2<T> const& other)const{return Point2<T>(x+other.x,y+other.y);}
    Point2<T> operator-(Point2<T> const& other)const{return Point2<T>(x-other.x,y-other.y);}
};

template<typename T=float>
class Point3{
public:
    T x,y,z;
    Point3(T const& a,T const& b,T const& c):x(a),y(b),z(c){};
    Point3<T>& operator=(T const& a){x=a;y=a;z=a;return *this;}
};

template<typename T=float>
class Point4{
public:
    T x,y,z,t;
    Point4(T const& a,T const& b,T const& c,T const& d):x(a),y(b),z(c),t(d){};
    Point4<T>& operator=(T const& a){x=a;y=a;z=a;t=a;return *this;}
};



template<typename T=float>
std::ostream& operator<<(std::ostream& os,Point2<T> const& p){
    os<<"("<<p.x<<","<<p.y<<")";
    return os;
}

template<typename T=float>
std::ostream& operator<<(std::ostream& os,Point3<T> const& p){
    os<<"("<<p.x<<","<<p.y<<","<<p.z<<")";
    return os;
}


template<class P>
class Segment{
public:
    P p1,p2;
    Segment(P const& P1,P const& P2):p1(P1),p2(P2){}
};

template<typename T=float>
class Polygon{
public:
    arma::Col<Point2<T> > _Points;
    Point2<T> max()const{
        float xmax=_Points[0].x,ymax=_Points[0].y;
        for(int i=1;i<_Points._N;++i){
            if(_Points[i].x>xmax)xmax=_Points[i].x;
            if(_Points[i].y>ymax)ymax=_Points[i].y;
        }
        return Point2<T>(xmax,ymax);
    }
};

template<typename T>
bool intersect(Segment<Point2<T> > const& s1,Segment<Point2<T> > const& s2){
    Point2<T> r,s;
    float A1,A2,B1,B2,C1,C2;
    A1=s1.p2.y-s1.p1.y;
    A2=s2.p2.y-s2.p1.y;
    B1=s1.p1.x-s1.p2.x;
    B2=s2.p1.x-s2.p2.x;
    C1=A1*s1.p1.x+B1*s1.p1.y;
    C2=A2*s2.p1.x+B2*s2.p1.y;
    float det=A1*B2-A2*B1;
    if(det){
        float x=(B2*C1-B1*C2)/det;
        float y=(A1*C2-A2*C1)/det;
        if(min(s1.p1.x,s1.p2.x)<=x&&x<=min(s1.p1.x,s1.p2.x))
            if(min(s2.p1.x,s2.p2.x)<=x&&x<=min(s2.p1.x,s2.p2.x))
                return 1;
    }
    return 0;

    r=s1.p2-s1.p1;
    s=s2.p2-s2.p1;
    std::cout<<"Points = "<<s1.p1<<"->"<<s1.p2<<" ; "<<s2.p1<<"->"<<s2.p2<<std::endl;
    std::cout<<"r = "<<r<<std::endl;
    std::cout<<"s = "<<s<<std::endl;
    float alpha(s.y*r.x-r.y*s.x);
    std::cout<<"alpha = "<<alpha<<std::endl;
    if(alpha){
        Point2<T> beta(s1.p1-s2.p1);
        std::cout<<"beta = "<<beta<<std::endl;
        float u(beta.x*r.y-r.x*beta.y);
        u/=alpha;
        float t(beta.x*s.y-s.x*beta.y);
        t/=alpha;
        std::cout<<"t = "<<t<<" u = "<<u<<std::endl<<std::endl<<std::endl;
        if(0<=u && u<=1 && 0<=t && t<=1)
            return 1;
    }
    return 0;
}

template<typename T>
bool PointInPolygon(Point2<T> const& p,Polygon<T> const& poly){
    Point2<T> P2(p.x,p.y+poly.max().y);
    Segment<Point2<T> > line(p,P2);
    int count=0;
    for(int i=0;i<poly._Points._N-1;++i){
        count+=intersect(line,Segment<Point2<T> >(poly._Points[i],poly._Points[i+1]));
    }
    std::cout<<"count"<<count<<std::endl;
    return (count&&1)==1;
}







#endif
