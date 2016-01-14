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

#ifndef meta_param_header_file
#define meta_param_header_file

#include <cmath>
#include <iostream>

//MetaParam struct that will hold details of a scattering layer

struct MetaParam{
    int _T,_Q,_J,_PE;//variables
    MetaParam(){};//default constructor
    MetaParam(int t,int q=1,int j=0,int pe=1):_T(t),_Q(q),_J(j),_PE(pe){if(!_J)TtoJ();}//if J is not given, compute it from T
    void TtoJ(){//utility to compute J from T alone
        const int Multiplier =1+(_Q==1?1:0);//if dyadic Multiplier =2 otherwise multiplier =1
        _J=round(log2(_T*Multiplier/(4.*_Q)));
    }
};


#endif
