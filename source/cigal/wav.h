/*
 This file is part of CIGAL.
 CIGAL version 0.1, revised jan 2015  from Randall Balestriero
 randallbalestriero@gmail.com
 CIGAL is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 CIGAL is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with CIGAL.  If not, see <http://www.gnu.org/licenses/>.
 
 We thank SABIOD.ORG project MI CNRS MASTODONS who supported this work,
 Pr. Hervé Glotin for his support, Vincent LOSTA... and Pr Stéphane
 Mallat for their precious discussions and help on SCANET from DATA ENS
 Paris.*/


#ifndef WAV_HEADER_FILE
#define WAV_HEADER_FILE
#include <iostream>
#include <string>
#include <fstream>
#include <armadillo>

class WAV{/* Randall BALESTRIERO
             template class WAv here to import and save
             WAVE files. No need for copy constructor,
             move constructor,assignment operator,
             move assignment operator.
             _Size and _SampleRate
             and _NumberOfChannel are public because might be needed
             by the user. However the data (the array) is private but can be
             accessed through the [] operator.
             a ptr() function give the
             pointer to the data. Header file is as comment of the load and save
             methods. The operators are duplicated*/
private:
    
    WAV(const WAV &);
    WAV(WAV &&);
    WAV & operator=(const WAV &);
    WAV & operator=(WAV &&);

public:
    arma::vec _Values;

    int _SampleRate;
    int _NumberOfChannel;
    inline void operator<<(std::string const&);//to import wav
    inline void operator>>(std::string const&);//to txt file

    WAV(){}
    WAV(std::string const& Name){*this<<Name;}//const char* Name){};
    ~WAV(){}
    inline void PrintWav(char const * const Name);//method to directly print the wav into file Name
    double &      operator[](int const i){return _Values[i];}
    double const& operator[](int const i)const{return _Values[i];}
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//                          STREAM OPERATORS


inline void WAV::operator<<(std::string const& Name){
/*      CANNONICAL WAVEFILE FORM
        4  bit x 4 : "RIFF" or not
        32 bit x 1 : size of the file
        4  bit x 4 : "WAVE" or not
        4  bit x 4 : "fmt " or not
        32 bit x 1 : size of format chunk (in bit)
        16 bit x 1 : format tag (1=no compression)
        16 bit x 1 : number of channel (1=mono 2=stereo)
        32 bit x 1 : sample rate 
        32 bit x 1 : average number of bit per second (sample rate*channels*bit per sample/8)
        16 bit x 1 : block alignment
        16 bit x 1 : bits per sample (8 or 16)
        4  bit x 4 : "data"
        32 bit x 1 : DataChunk_Size
*/
    char id[4];
    FILE * file;
    unsigned int size,Chunk_Size,BitPerSecond,Data_Size;
    size_t bytes;
    short FormatTag,BlockAlignment,BitsPerSample;
    file=fopen(Name.c_str(),"r");
    bytes=fread(id,sizeof(char),4,file);//read first 4 bytes which contain RIFF iff wav
    if(id[0]=='R'&&id[1]=='I'&&id[2]=='F'&&id[3]=='F'){
        bytes=fread(&size,sizeof(unsigned int),1,file);//read 32 bit size value
        bytes=fread(id,sizeof(char),4,file);//string that should contain WAVE
        if(id[0]=='W'&&id[1]=='A'&&id[2]=='V'&&id[3]=='E'){
            bytes=fread(id,sizeof(char),4,file);//read the "fmt " string
            bytes=fread(&Chunk_Size,sizeof(unsigned int),1,file);
            bytes=fread(&FormatTag,sizeof(short),1,file);
            bytes=fread(&_NumberOfChannel,sizeof(short),1,file);
            bytes=fread(&_SampleRate,sizeof(unsigned int),1,file);
            bytes=fread(&BitPerSecond,sizeof(unsigned int),1,file);
            bytes=fread(&BlockAlignment,sizeof(short),1,file);
            bytes=fread(&BitsPerSample,sizeof(short),1,file);
            bytes=fread(&id,sizeof(char),4,file);
            bytes=fread(&Data_Size,sizeof(unsigned int),1,file);

            int _Size=Data_Size/sizeof(short);
            _Values=arma::vec(_Size);
            short int size_buffer;
            for(int i=0;i<_Size;++i){
                bytes=fread(&size_buffer,sizeof(short int),1,file);
                _Values[i]=(double) size_buffer;}
            if(bytes){}
        }
    }
    fclose(file);
}

inline void WAV::operator>>(std::string const& Name){/*
            use like WAV_v>>myfile.txt, use this to directly
            print a wav into a txt file. */
            //O(n)
    std::ofstream file;
    file.open(Name,std::ios::out|std::ios::binary);
    file<<_Values;
    file.close();
}

inline std::ostream& operator<<(std::ostream& os,WAV const& a){
    os<<a._Values;
    return os;
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//                      PRINT UTILITY
inline void WAV::PrintWav(const char * const Name){/*
        uses a special method because the standard output
        stream operator is used for .wav to .txt file.
        basically do the inverse of the << operator, given
        a specific file name. use standard WAVE file header. */

/*      CANNONICAL WAVEFILE FORM
        4  bit x 4 : "RIFF" or not
        32 bit x 1 : size of the file
        4  bit x 4 : "WAVE" or not
        4  bit x 4 : "fmt " or not
        32 bit x 1 : size of format chunk (in bit)
        16 bit x 1 : format tag (1=no compression)
        16 bit x 1 : number of channel (1=mono 2=stereo)
        32 bit x 1 : sample rate 
        32 bit x 1 : average number of bit per second (sample rate*channels*bit per sample/8)
        16 bit x 1 : block alignment
        16 bit x 1 : bits per sample (8 or 16)
        4  bit x 4 : "data"
        32 bit x 1 : DataChunk_Size
*/
    //a mute variable S will hold either the input signal or the already imported wav O(n)

    char id[4]={'R','I','F','F'};
    FILE * file;
    int size=_Values.n_elem+36,Chunk_Size=16,BitPerSecond=_SampleRate*_NumberOfChannel;
    size_t bytes;
    const short int FormatTag=1,BitsPerSample=16,BlockAlignment=_NumberOfChannel*BitsPerSample/8;
    const size_t _SizeOfInt=sizeof(int),_SizeOfChar=sizeof(char),_SizeOfShortInt=sizeof(short int);
    file=fopen(Name,"w");
    bytes=fwrite(&id,_SizeOfChar,4,file);//write RIFF string
    bytes=fwrite(&size,_SizeOfInt,1,file);//write 32 bit size value
    id[0]='W';id[1]='A';id[2]='V';id[3]='E';
    bytes=fwrite(&id,_SizeOfChar,4,file);//write WAVE string
    id[0]='f';id[1]='m';id[2]='t';id[3]=' ';
    bytes=fwrite(&id,_SizeOfChar,4,file);//write the "fmt " string
    bytes=fwrite(&Chunk_Size,_SizeOfInt,1,file);
    bytes=fwrite(&FormatTag,_SizeOfShortInt,1,file);//always 1 for no compression
    bytes=fwrite(&_NumberOfChannel,_SizeOfShortInt,1,file);//input
    bytes=fwrite(&_SampleRate,_SizeOfInt,1,file);//input
    bytes=fwrite(&BitPerSecond,_SizeOfInt,1,file);
    bytes=fwrite(&BlockAlignment,_SizeOfShortInt,1,file);
    bytes=fwrite(&BitsPerSample,_SizeOfShortInt,1,file);
    id[0]='d';id[1]='a';id[2]='t';id[3]='a'; 
    bytes=fwrite(&id,_SizeOfChar,4,file);
    size_t _Size=_Values.n_elem*(3-_NumberOfChannel);
    bytes=fwrite(&_Size,_SizeOfInt,1,file);//here length might be wrong
    if(bytes){}
    for(unsigned int i=0;i<_Values.n_elem;++i){
        const short int buffer=(short int) _Values[i];
        bytes=fwrite(&buffer,_SizeOfShortInt,1,file);
    }
    fclose(file);
}



#endif

