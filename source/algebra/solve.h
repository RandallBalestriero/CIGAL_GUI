#ifndef SOLVER_HEADER_FILE
#define SOLVER_HEADER_FILE

#include <armadillo>
#include <fstream>
#include <iostream>

//##############################################
//
//      JACOBI
//
//##############################################


inline arma::vec Jacobi(arma::mat const& A,arma::vec const& B,arma::vec & x0,double const& seuil=0.0000001,unsigned int const& ver=1){
    arma::mat D(A.n_rows,A.n_cols);
    arma::mat R(A.n_rows,A.n_cols);
    arma::vec a(A.n_rows);
    arma::mat b(A.n_rows,A.n_cols);
    arma::vec e(A.n_rows);
    for(unsigned int i=0;i<A.n_rows;++i){
        D(0,i)=1/A(i,i);
        R(i,i)=0;
    }
    a=D*B;
    b=-D*R;
    for(int I=0;;++I){
        x0=a+b*x0;
        if(I%ver==0){//checking every ver time
            e=A*x0-B;
            arma::pow(e,2);
            if(arma::sum(e)<seuil) break;
        }
    }
    return x0;
}

//################################################
//
//      GAUSS SEIDEL
//
//################################################

inline arma::vec& GaussSeidel(arma::mat const& A,arma::vec const& B,arma::vec & x0,double const& seuil=0.0000001,unsigned int const& ver=1){

    arma::mat L(A.n_rows,A.n_cols);
    arma::mat U(A.n_rows,A.n_cols);
    L=A;
    U=A;
    for(unsigned int i=0;i<A.n_rows;++i)
        U(i,0)=0;
    arma::mat invL;
    invL=L.i();

    arma::vec a(A.n_rows);
    a=invL*B;
    arma::mat b(A.n_rows,A.n_cols);
    b=-invL*U;
    arma::vec e(A.n_rows);
    for(int I=0;;++I){
        x0=a+b*x0;
        if(I%ver==0){//checking every ver time
            e=A*x0-B;
            arma::pow(e,2);
            if(arma::sum(e)<seuil) break;
        }
    }
    return x0;
}

//################################################
//
//      GRADIENT DESCENT
//
//################################################

inline arma::vec& GD(arma::mat const& A,arma::vec const& b,arma::vec & x,double const& seuil=0.00000001,unsigned int const& p=1){
    float alpha=0;
    unsigned int const N=A.n_rows;
    arma::vec grad(N);
    arma::vec Ag(N);
    grad=A*x-b;
    double alpha_a,alpha_b;//alpha=alpha_a/alpha_b
    for(int I=0;;++I){
        Ag=(A*grad);
        alpha_a=0;alpha_b=0;
        for(unsigned int i=0;i<N;++i){
            alpha_a+=pow(grad(i,0),2);
            alpha_b+=grad(i,0)*Ag(i,0);
        }
        alpha=alpha_a/alpha_b;
        for(unsigned int i=0;i<N;++i){
            x(i,0)-=alpha*grad(i,0);
            grad(i,0)-=alpha*Ag(i,0);
        }
        if(I%p==0){
            grad=A*x-b;
            float e=0;
            for(unsigned int i=0;i<N;++i)
                e+=pow(grad[i],2);
            if(e<seuil) break;
        }
    }
    return x;
}


//##############################################
//
//          CONJUGATE GRADIENT DESCENT
//
//##############################################

inline arma::vec& CGD( arma::mat const& A,arma::vec const& b,arma::vec & x,double const& seuil=0.00000001,unsigned int const& P=1){
    unsigned int const N=b.n_elem;
    arma::vec grad1(N);
    arma::vec grad2(N);
    arma::vec p(N);
    arma::vec Ap(N);
    grad1=A*x-b;
    for(unsigned int i=0;i<N;++i)
        p[i]=-grad1[i];
    double alpha_a,alpha_b,lambda_a,lambda_b,lambda,alpha;
    for(int I=0;;++I){
        alpha_a=0;alpha_b=0;lambda_a=0;lambda_b=0;
        grad2=grad1;
        Ap=(A*p);
        for(unsigned int i=0;i<N;++i){
            alpha_a+=grad1[i]*p[i];
            alpha_b+=p[i]*Ap[i];
        }
        alpha=alpha_a/alpha_b;
        for(unsigned int i=0;i<N;++i){
            x[i]-=alpha*p[i];
            grad2[i]-=alpha*Ap[i];
            lambda_a+=pow(grad2[i],2);
            lambda_b+=pow(grad1[i],2);
        }
        lambda=lambda_a/lambda_b;
        for(unsigned int i=0;i<N;++i)
            p[i]=-grad2[i]+lambda*p[i];
        grad1=grad2;
        if(I%P==0){
            grad1=A*x-b;
            float e=0;
            for(unsigned int i=0;i<N;++i)
                e+=pow(grad1[i],2);
            if(e<seuil) break;
        }
    }
    return x;
}

//#########################################################
//
//      CONJUGATE GRADIENT DESCENT FOR SPARSE MATRICES
//
//#########################################################


inline arma::vec&  CGD(arma::sp_mat const& A,arma::vec const& b,arma::vec & x,double const& seuil=0.00000001,unsigned int const& P=1){
    unsigned int const N=b.n_elem;
    arma::vec grad1(N);
    arma::vec grad2(N);
    arma::vec p(N);
    arma::vec Ap(N);
    grad1=A*x-b;
    for(unsigned int i=0;i<N;++i)
        p[i]=-grad1[i];
    double alpha_a,alpha_b,lambda_a,lambda_b,lambda,alpha;
    for(unsigned int I=0;;++I){
        alpha_a=0;alpha_b=0;lambda_a=0;lambda_b=0;
        grad2=grad1;
        Ap=(A*p);
        for(unsigned int i=0;i<N;++i){
            alpha_a+=grad1[i]*p[i];
            alpha_b+=p[i]*Ap[i];
        }
        alpha=alpha_a/alpha_b;
        for(unsigned int i=0;i<N;++i){
            x[i]-=alpha*p[i];
            grad2[i]-=alpha*Ap[i];
            lambda_a+=pow(grad2[i],2);
            lambda_b+=pow(grad1[i],2);
        }
        lambda=lambda_a/lambda_b;
        for(unsigned int i=0;i<N;++i)
            p[i]=-grad2[i]+lambda*p[i];
        grad1=grad2;
        if(I%P==0){
            grad1=A*x-b;
            float e=0;
            for(unsigned int i=0;i<N;++i)
                e+=pow(grad1[i],2);
            std::cout<<"e="<<e<<" "<<I<<std::endl;
            if(e<seuil){std::cout<<"Done in "<<I<<" iterations"<<std::endl;break;}
        }
    }
    return x;
}

//#########################################################
//
//      GRADIENT DESCENT FOR SPARSE MATRIX
//
//#########################################################


inline arma::vec & GD(arma::sp_mat const& A,arma::vec const& b,arma::vec & x,double const& seuil=0.00000001,unsigned int const& p=1){
    float alpha=0;
    unsigned int const N=b.n_elem;
    arma::vec grad(N);
    arma::vec Ag(N);
    grad=A*x-b;
    double alpha_a,alpha_b;//alpha=alpha_a/alpha_b
    for(int I=0;;++I){
        Ag=(A*grad);
        alpha_a=0;alpha_b=0;
        for(unsigned int i=0;i<N;++i){
            alpha_a+=pow(grad(i,0),2);
            alpha_b+=grad(i,0)*Ag(i,0);
        }
        alpha=alpha_a/alpha_b;
        for(unsigned int i=0;i<N;++i){
            x(i,0)-=alpha*grad(i,0);
            grad(i,0)-=alpha*Ag(i,0);
        }
        if(I%p==0){
            grad=A*x-b;
            float e=0;
            for(unsigned int i=0;i<N;++i)
                e+=pow(grad[i],2);
            if(e<seuil) {std::cout<<"Done in "<<I<<" iterations"<<std::endl; break;};
        }
    }
    return x;
}











#endif



