#include "fft.h"

// SET COEFFICIENTS METHOD WHICH WILL INITiALIZE ALL THE VARIABLES FOR THE PERFORM METHOD
// THIS MEANS PADDING IF NECESSARY

void fft::PerformBitReversal(){
    complex<double> tmp(0,0);
    int m,target=0;
    for(unsigned int i=0;i<_Coefficients.n_elem;++i){
        if(target>(int)i){
            tmp=_Coefficients[target];
            _Coefficients[target]=_Coefficients[i];
            _Coefficients[i]=tmp;
        }
        m=_Coefficients.n_elem;
        while(target&(m>>=1))
            target&=~m;
        target|=m;
    }
}

//DECOMPOSITION ALGORITHM
 void fft::PerformDecomposition(){
    int T1,T2,bT2,T1pbT2,T4;
    double Twiddletmp1;
    complex<double> tmp;
    for(int level=0;level<log2(_Coefficients.n_elem);++level){//main loop
        T1=1<<level;
        T2=2<<level;
        Twiddletmp1=2.*M_PI/T2;
        complex<double> * const w=new complex<double> [T1];
        for(int i=0;i<T1;++i) w[i]=W(i*Twiddletmp1);//compute twiddle factors
        for(unsigned int block=0;block<(_Coefficients.n_elem/T2);++block){//go through the blocks for each level
            bT2=block*T2;
            T1pbT2=T1+bT2;
            for(int line=0;line<T1;++line){//inside blocks
                T4=T1pbT2+line;
                _Coefficients[T4]*=w[line];
                tmp=_Coefficients[bT2+line];
                _Coefficients[bT2+line]+=_Coefficients[T4];
                _Coefficients[T4]=tmp-_Coefficients[T4];
            }
        }
        delete[] w;
    }
}

//COMPUTE FFT METHOD WHICH WILL CALL THE THREE METHODS : SETCOEFF, BIT REVERSAL, DECOMPOSITION
 void fft::ComputeFFT(arma::vec const&  signal,const bool pad){
    Set_Coefficients(signal,pad);
    PerformBitReversal();
    PerformDecomposition();
}

//SAME FOR IFFT USING THE GIVEN ALGORITHM TO PERFORM IFFT USING THE FFT
 void fft::ComputeIFFT(arma::cx_vec const& signal,const bool pad){
    Set_Coefficients(signal,pad);
    for(unsigned int i=0;i<_Coefficients.n_elem;++i)
        _Coefficients[i]=conj(_Coefficients[i]);
    PerformBitReversal();
    PerformDecomposition();
    const complex<double> t(_Coefficients.n_elem,0);
    for(unsigned int i=0;i<_Coefficients.n_elem;++i)
        _Coefficients[i]=conj(_Coefficients[i])/t;
}
//inplace FFT

 void InplaceFFT(arma::cx_vec & _Coefficients){
    complex<double> tmp(0,0);
    int m,target=0;
    for(unsigned int i=0;i<_Coefficients.n_elem;++i){
        if(target>(int)i){
            tmp=_Coefficients[target];
            _Coefficients[target]=_Coefficients[i];
            _Coefficients[i]=tmp;
        }
        m=_Coefficients.n_elem;
        while(target&(m>>=1))
            target&=~m;
        target|=m;
    }
    int T1,T2,bT2,T1pbT2,T4;
    double Twiddletmp1;
    for(int level=0;level<log2(_Coefficients.n_elem);++level){//main loop
        T1=1<<level;
        T2=2<<level;
        Twiddletmp1=2.*M_PI/T2;
        complex<double> * const w=new complex<double> [T1];
        for(int i=0;i<T1;++i) w[i]=W(i*Twiddletmp1);//compute twiddle factors
        for(unsigned int block=0;block<(_Coefficients.n_elem/T2);++block){//go through the blocks for each level
            bT2=block*T2;
            T1pbT2=T1+bT2;
            for(int line=0;line<T1;++line){//inside blocks
                T4=T1pbT2+line;
                _Coefficients[T4]*=w[line];
                tmp=_Coefficients[bT2+line];
                _Coefficients[bT2+line]+=_Coefficients[T4];
                _Coefficients[T4]=tmp-_Coefficients[T4];
            }
        }
        delete[] w;
    }
}

//inplace IFFT
 void InplaceIFFT(arma::cx_vec & _Coefficients){
    for(unsigned int i=0;i<_Coefficients.n_elem;++i)
        _Coefficients[i]=conj(_Coefficients[i]);
    complex<double> tmp(0,0);
    int m,target=0;
    for(unsigned int i=0;i<_Coefficients.n_elem;++i){
        if(target>(int)i){
            tmp=_Coefficients[target];
            _Coefficients[target]=_Coefficients[i];
            _Coefficients[i]=tmp;
        }
        m=_Coefficients.n_elem;
        while(target&(m>>=1))
            target&=~m;
        target|=m;
    }
    int T1,T2,bT2,T1pbT2,T4;
    double Twiddletmp1;
    for(int level=0;level<log2(_Coefficients.n_elem);++level){//main loop
        T1=1<<level;
        T2=2<<level;
        Twiddletmp1=2.*M_PI/T2;
        complex<double> * const w=new complex<double> [T1];
        for(int i=0;i<T1;++i) w[i]=W(i*Twiddletmp1);//compute twiddle factors
        for(unsigned int block=0;block<(_Coefficients.n_elem/T2);++block){//go through the blocks for each level
            bT2=block*T2;
            T1pbT2=T1+bT2;
            for(int line=0;line<T1;++line){//inside blocks
                T4=T1pbT2+line;
                _Coefficients[T4]*=w[line];
                tmp=_Coefficients[bT2+line];
                _Coefficients[bT2+line]+=_Coefficients[T4];
                _Coefficients[T4]=tmp-_Coefficients[T4];
            }
        }
        delete[] w;
    }
    _Coefficients/=_Coefficients.n_elem;
}

