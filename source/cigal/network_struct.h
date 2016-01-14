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


#ifndef SCATTERING_STRUCT_HEADER_FILE
#define SCATTERING_STRUCT_HEADER_FILE
#include "./layer_struct.h"
#include "./meta_param.h"
#include "./wav.h"
#include "../PLAIN_VECTOR.h"
// NETWORK CLASS

class ScatteringNetwork{
    private:
        Vector<MetaParam> const& _Params;//list of MetaParam
        void operator=(ScatteringNetwork const& ){};//forbidden operator
    public:
        Vector<Matrix<> > _Layers;//list of layers
        ScatteringNetwork(Matrix<> const& input,Vector<MetaParam> const& param):_Params(param){
            _Layers=Vector<Matrix<> > ();
            //init the first layers
            ScatteringLayer * l=new ScatteringLayer(input,_Params[0]);
            _Layers.append(l->_HighDecomposition);
            delete l;

            //recursively apply the output of layer i as input of layer i+1
            for(std::size_t i=1;i<_Params._N;++i){
                ScatteringLayer * l=new ScatteringLayer(_Layers[i-1],_Params[i]);
                _Layers.append(l->_HighDecomposition);
                delete l;
            }
        }
        ~ScatteringNetwork(){};
        Matrix<> const& operator[](std::size_t const& i){return _Layers[i];}//usefull operator to access
                                                                        //a specific layer
};

#endif

