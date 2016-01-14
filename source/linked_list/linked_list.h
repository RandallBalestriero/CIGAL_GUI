//list chainee/linked lists
//
//divided in two parts :
//  -structure used a linked elements
//  -class using the previous structure plus utilities 
//   for access memory management,...
//
#include <iostream>


//###################################################
//
//  STRUCTURE
//
//###################################################
template<typename T=float>
struct List{
        List* _Next;
        T _Value;
        
        List(T v=0):_Next(0),_Value(v){};
        T & operator[](const int);
        T const& operator[](const int)const;
        List<T>* operator()(const int);
        List<T> const* operator()(const int)const;
        ~List(){
            if(_Next)
                delete[] _Next;
        }
};

template<typename T>
T & List<T>::operator[](const int I){
    List<T>* obj=this;
    for(int i=0;i<I;++i)
        obj=obj->_Next;
    return obj->_Value;
}


template<typename T>
T const& List<T>::operator[](const int I)const{
    List<T>* obj=this;
    for(int i=0;i<I;++i)
        obj=obj->_Next;
    return obj->_Value;
}

template<typename T>
List<T>* List<T>::operator()(const int I){
    List<T>* obj=this;
    for(int i=0;i<I;++i)
        obj=obj->_Next;
    return obj;
}

template<typename T>
List<T> const* List<T>::operator()(const int I)const{
    List<T>* obj=this;
    for(int i=0;i<I;++i)
        obj=obj->_Next;
    return obj;
}


//################################
//
//  CLASS LINKED LIST
//
//################################


template<typename T=float>
class CList{
    private:
        List<T>* _List;
        List<T>* _Last;
    public:
        unsigned int _N;
        CList(const T v=0):_List(new List<T>[1]),_N(1){
            _List[0]=v;
            _Last=_List;
        }
        T& operator[](const int);
        T const& operator[](const int)const;
        void Clean();
        ~CList(){Clean();}

        CList<T>& push_back(const T);
        CList<T>& pop_back();
        CList<T>& pop(const int);

};

template<typename T>
void CList<T>::Clean(){
    if(_List)
        delete[] _List;
}

template<typename T>
T& CList<T>::operator[](const int I){
    if(I>=_N){
        List<T>* obj=_Last;
        for(int i=0;i<_N-I;++i){
            obj->_Next=new List<T>[1];
            obj=obj->_Next;
            _N+=1;
        }
        _Last=obj;
        return obj->_Value;
    }
    return (*_List)[I];
}

template<typename T>
T const& CList<T>::operator[](const int I)const{
    return (*_List)[I];
}

template<typename T>
CList<T>& CList<T>::push_back(const T v){
    _N+=1;
    _Last->_Next=new List<T>[1];
    _Last=_Last->_Next;
    _Last->_Value=v;
    return *this;
}

template<typename T>
CList<T>& CList<T>::pop_back(){
    _Last=(*_List)(_N-2);
    delete[] _Last->_Next;
    _Last->_Next=0;
    _N-=1;
    return *this;
}

template<typename T>
CList<T>& CList<T>::pop(const int i){
    List<T>* tmp;
    if(i==0 && _N>1){
        tmp=_List->_Next;
        _List->_Next=0;
        delete[] _List;
        _List=tmp;
        _N-=1;
    }
    else{
        tmp=(*_List)(i+1);
        (*_List)(i)->_Next=0;
        delete[] (*_List)(i);
        (*_List)(i-1)->_Next=tmp;
        _N-=1;
    }
    return *this;
}

//###############################################
//
//  OUTPUT OPERATOR
//
//###############################################


template<typename T>
ostream& operator<<(ostream& os,CList<T> const& L){
    for(int i=0;i<L._N;++i){
        os<<i<<"="<<L[i]<<" ";
    }
    os<<endl;
    return os;
}


