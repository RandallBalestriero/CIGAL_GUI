#include "./spectrogram.h"

spectrogram::spectrogram(std::string const& Name,const int window,const float overlap,void(*WindowFunction)(arma::vec &)){//can be used as a default constructor with no args
        WAV S(Name);//load the wav
        Perform(S._Values,window,overlap,WindowFunction);//perform the spectrogram with the given arguments
    }

    double & spectrogram::operator()(const std::size_t i,const std::size_t j){return _Coefficients(i,j);}//access operator for element given i and j
    double const& spectrogram::operator()(const std::size_t i,const std::size_t j)const{return _Coefficients(i,j);}

    //PERFORM METHOD WHICH CAN BE CALLED AFTER THE DECLARATION AND MULTIPLE TIMES FOR THE SAME VARIABLE
    void spectrogram::Perform(arma::vec const& signal,const int window,const float overlap,void(*WindowFunction)(arma::vec &)){
        //GENERATE THE NUMBER OF WINDOWS
        int _M=1+(signal.n_elem-window)/((1.-overlap)*window);//output length
        int _N=window/2;//output line number
        _overlapp=overlap;
        _window=window;
        _Coefficients=arma::mat (_N,_M);
        std::cout<<"\n\tSignal size : "<<signal.n_elem<<"\n\tWindow size : "<<window<<"\n\tNumber of windows : "<<_M<<"\n\tOverlap : "<<100*overlap<<" %"<<std::endl;

        arma::cx_vec SignalTmp(window);//for each fft on window
        int Coeff;
        arma::vec Windowing_Coefficients(window);
        WindowFunction(Windowing_Coefficients);//get the windowing coefficients
        //PERFORM DECOMPOSITION
        for(int i=0;i<_M-1;++i){//for each chunk
       //     std::cout<<i<<std::endl;
            Coeff=ceil(i*window*(1-overlap));//position of the chunk
            for(int j=0;j<window;++j)//apodization
                SignalTmp.at(j)=signal.at(Coeff+j)*Windowing_Coefficients.at(j);
            SignalTmp=arma::fft(SignalTmp);
            for(int j=0;j<_N;++j)//fill the spectogram
                _Coefficients.at(j,i)=std::abs(SignalTmp.at(_N+j));//we get the second hald so we have high frequency on the top
        }
        //last window is separated from other for possible padding
        Coeff=(_M)*window*(1-overlap);
        for(unsigned int j=0;j<signal.n_elem-Coeff;++j)
            SignalTmp[j]=signal.at(Coeff+j)*Windowing_Coefficients.at(j);
        if((int)signal.n_elem-Coeff<window)
            for(int j=signal.n_elem-Coeff;j<window;++j)
                SignalTmp.at(j)=0;
        SignalTmp=arma::fft(SignalTmp);
        for(int j=0;j<_N;++j)
            _Coefficients.at(j,_M-1)=std::abs(SignalTmp.at(j));
      //  _Coefficients-=arma::min(arma::min(_Coefficients));
      //  _Coefficients/=arma::max(arma::max(_Coefficients));
    }
