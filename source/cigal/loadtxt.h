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



#ifndef LOADTXT_HEADER_FILE
#define LOADTXT_HEADER_FILE

#include <fstream>
#include <string>
#include <sstream>
#include "../PLAIN_MATRIX.h"



inline Matrix<> loadtxt(const char * name){/*
                template function, import data from txt file to 2D vector of template type.
                Use : vector <vector<float> > D=loadtxt("myfile.txt") for example.
                only the first line is loaded by expanding vector to discover the size.
                For the number of line there is no other way than push_back.*/
    std::ifstream file;
    file.open(name,std::ios::binary);//no need for std::ios::in because we already made a ifstream
    std::string line;//will hold the line characters
    float Value;//tmp variable that will hold each value when
               //reading the file: file -> Value -> 1Dstd::vector -> 2D std::vector
    Matrix<> Data;//variable that will hold the file values (2D)
    while(getline(file,line)){//while it is possible read a line
        std::istringstream I(line);//string stream from the line
        Vector<> DataLine;//tmp vector which will hold the line values
        while(I>>Value){//while possible to read a work (or number here)
            DataLine.append(Value);//push back the value
        }
        Data.append(DataLine);//we add the line to the final 2D array
    }
    file.close();
    return Data;
}



inline Matrix<> loadtxt(ifstream & file){/*
                template function, import data from txt file to 2D vector of template type.
                Use : vector <vector<float> > D=loadtxt("myfile.txt") for example.
                only the first line is loaded by expanding vector to discover the size.
                For the number of line there is no other way than push_back.*/
    std::string line;//will hold the line characters
    float Value;//tmp variable that will hold each value when
               //reading the file: file -> Value -> 1Dstd::vector -> 2D std::vector
    Matrix<> Data;//variable that will hold the file values (2D)
    while(getline(file,line)){//while it is possible read a line
        std::istringstream I(line);//string stream from the line
        Vector<> DataLine;//tmp vector which will hold the line values
        while(I>>Value)//while possible to read a work (or number here)
            DataLine.append(Value);//push back the value
        Data.append(DataLine);//we add the line to the final 2D array
    }
    return Data;
}

#endif
