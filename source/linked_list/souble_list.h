#ifndef DOUBLE_LINKED_LIST_HEADER_FILE
#define DOUBLE_LINKED_LIST_HEADER_FILE

#include <iostream>


//#############################################
//
//          NODE STRUCTURE
//
//#############################################


template<typename T=float>
struct DoubleLinkedNode{
    DoubleLinkedNode<T>* _Previous;//link to the previous node
    DoubleLinkedNode<T>* _Next;//link to the next node
    T _Value;//value of the node

    DoubleLinkedNode(const T=0);//default constructor
    DoubleLinkedNode(DoubleLinkedNode<T> &&);//specialized constructor given other nodes

    ~DoubleLinkedNode();//default destructor

    //ITERATORS
    inline DoubleLinkedNode<T>* operator+(const unsigned int);
    inline DoubleLinkedNode<T>* operator-(const unsigned int);

    inline DoubleLinkedNode<T> const * operator+(const unsigned int)const;
    inline DoubleLinkedNode<T> const * operator-(const unsigned int)const;

};

//DEFAULT CONSTRUCTOR
//All is initialized to 0, needto be that way because the operator new
//till use the default constructor when dynamically allocating memory
template<typename T>
DoubleLinkedNode<T>::DoubleLinkedNode(const T v):
                _Previous(0),_Next(0),_Value(v){}

//SPECIALIZED CONSTRUCTOR
//Given privous and next (by default 0) pointers to nodes, initialize the structure
//this constructor is not necessary for the DoubleLinkedList class
template<typename T>
DoubleLinkedNode<T>::DoubleLinkedNode(DoubleLinkedNode<T> && p):_Previous(p._Previous),_Next(0),_Value(p._Value){}

//DEFAULT DESTRUCTOR
//we will recursiveley delete the nodes by deleting the next one.
//This way, we just need to delete the first node in the DoubleLinkedList and
//the destruction will spread to the end of the linked list.
template<typename T>
DoubleLinkedNode<T>::~DoubleLinkedNode(){
    if(_Next){
        delete[] _Next;
        _Next=0;
    }
}

//ARITHMETIC OPERATORS
//OPERATOR +
template<typename T>
inline DoubleLinkedNode<T>* DoubleLinkedNode<T>::operator+(const unsigned int t){
    DoubleLinkedNode<T>* obj=this;
    for(int i=0;i<t;++i)
        obj=obj->_Next;
    return obj;
}
//OPERATOR +
template<typename T>
inline DoubleLinkedNode<T> const * DoubleLinkedNode<T>::operator+(const unsigned int t)const{
    DoubleLinkedNode<T>* obj=this;
    for(int i=0;i<t;++i)
        obj=obj->_Next;
    return obj;
}


//OPERATOR -
template<typename T>
inline DoubleLinkedNode<T>* DoubleLinkedNode<T>::operator-(const unsigned int t){
    DoubleLinkedNode<T>* obj=this;
    for(int i=0;i<t;++i)
        obj=obj->_Previous;
    return obj;
}

//OPERATOR -
template<typename T>
inline DoubleLinkedNode<T>const* DoubleLinkedNode<T>::operator-(const unsigned int t)const{
    DoubleLinkedNode<T>* obj=this;
    for(int i=0;i<t;++i)
        obj=obj->_Previous;
    return obj;
}

//####################################################
//
//
//          DOUBLE LINKED LIST CLASS
//
//
//####################################################

template<typename T=float>
class DoubleLinkedList{
    private:
        DoubleLinkedNode<T>* _List;//pointer to the first node
    public:
        DoubleLinkedNode<T>* _Iterator;
        unsigned int _IteratorPosition;

        int _N;//size of the linked list

        DoubleLinkedList(const int=1,const T=0);//default constructor
        ~DoubleLinkedList();//default destructor
        DoubleLinkedNode<T>& start(){return _List;}
        DoubleLinkedNode<T>& end(){return _List->_Previous;}

        DoubleLinkedNode<T> const& start()const{return _List;}
        DoubleLinkedNode<T> const& end()const{return _List->_Previous;}

        inline T & operator[](const unsigned int);//access operator
        T const& operator[](const unsigned int)const;//access operator for const instance

        DoubleLinkedList<T>& operator+=(const T);
  //      DoubleLinkedList<T>& operator+=(DoubleLinkedList<T> const&);
    //    DoubleLinkedList<T>& operator+=(DoubleLinkedList<T> &&);
};

//DEFAULT CONSTRUCTOR
//if no args, create a size 1 with value 0 linked list. Otherwise it is
//if one arg is given, it is the size, all initialized at 0
//possible to create a linked list of arbitrary size initialized with the given value if 2 args are given
//NOTE : the first node has a previous link to the last one for easy access of the end of the linked list
template<typename T>
DoubleLinkedList<T>::DoubleLinkedList(const int n,const T v){
    _N=n;//init the size
    _List=new DoubleLinkedNode<T> [1];//create the first node
    _List->_Value=v;//init its value
    DoubleLinkedNode<T>* obj=_List;//prepare the object used for recursive linkage
    for(int i=0;i<n-1;++i){
        obj->_Next=new DoubleLinkedNode<T>[1];//create new node
        obj->_Next->_Previous=obj;//link the previous link of the next node to the current one
        obj->_Next->_Value=v;//init its value
        obj=obj->_Next;//go to the next one
    }
    _List->_Previous=obj;//link the first node previous link to the last node

    _Iterator=_List+_N/2;
    _IteratorPosition=_N/2;
}

    
//DEFAULT DESTRUCTOR
template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList(){
    if(_List){
        delete[] _List;//we simply delete the first node
                        //calling the destructor of DoubleLinkedNode
                        //which will spread the deallocation through all the nodes
        _List=0;
        _Iterator=0;
    }
}

//ACCESS OPERATOR
//supposed to be optimized. If called with a vlue greater than the size,
//it will create the nodes and send back the last one (which corresponds
//to the asked index).
//if the user wants to acces an index, either previous linked or next linked
//is used given the index and the current size of the linked list.
template<typename T>
inline T & DoubleLinkedList<T>::operator[](const unsigned int I){
    if(I<_N){//no need for size increase so just accessing the value
        if(I<_IteratorPosition){//between 0 and IteratorPosition
            if(I<_IteratorPosition/2)//closer to the beginning
                _Iterator=(*_List)+I;
            else//closer to Iterator
                _Iterator=(*_Iterator)-(_IteratorPosition-I);
        }
        else{//between Iterator and the end
            if(I<(_N-_IteratorPosition)/2)//closer to iterator
                _Iterator=(*_Iterator)+(I-_IteratorPosition);
            else//closer to the end
                _Iterator=(*_List)-(_N-I);
        }
        _IteratorPosition=I;
        return _Iterator->_Value;
    }
    else{//we need to increase the size of the currend linked list
        DoubleLinkedNode<T>* obj=_List->_Previous;//we start at the end
        for(int i=0;i<I-_N+1;++i){//increasing the size by creating nodes init at 0
            obj->_Next=new DoubleLinkedNode<T>[1];
            obj->_Next->_Previous=obj;
            obj=obj->_Next;
        }
        _N=I+1;//increasing the new size of the linked list
        _List->_Previous=obj;//updating the previous link of the first node
        return obj->_Value;
    }
    return _Iterator->_Value;//return the reference
}

//ACCESS OPERATOR for const instances
//in this case it is not possible to increase the size of the array
//since it is a const instance. So only the access part is used.
template<typename T>
T const& DoubleLinkedList<T>::operator[](const unsigned int I)const{
    if(I<_N){//no need for size increase so just accessing the value
        if(I<_N/2){//between 0 and IteratorPosition
            return ((*_List)+I)->_Value;
        }
        else//between Iterator and the end
            return ((*_List)-(_N-I))->_Value;
    }
    return ((*_List)-(_N-I))->_Value;
}

//OPERATOR +

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator+=(const T other){
    for(int i=0;i<_N;++i)
        (*this)[i]+=other;
    return *this;
}
/*
template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator+=(DoubleLinkedList<T> const& other){
    int _NMax=(other._N>_N?other._N:_N);
    for(int i=0;i<other._N;++i){
        (*this)[i]+=other[i];
    }
    return *this;
}

template<typename T>
DoubleLinkedList<T>& DoubleLinkedList<T>::operator+=(DoubleLinkedList<T> && other){
    int _NMax=(other._N>_N?other._N:_N);
    for(int i=0;i<other._N;++i){
        (*this)[i]+=other[i];
    }
    return *this;
}

*/




//########################################################
//
//
//
//          OUTPUT STREAM OPERATORS
//
//
//
//########################################################


template<typename T>
ostream& operator<<(ostream& os,DoubleLinkedNode<T> const& n){
    os<<"Address = "<<&n<<" Value = "<<n._Value<<" Previous = "<<n._Previous;
    os<<" Next = "<<n._Next<<endl;
    return os;
}

template<typename T>
ostream& operator<<(ostream& os,DoubleLinkedList<T> const& l){
    //typename DoubleLinkedList<T>::_Iterator obj=l.start();
    if(l._N<20){
        for(int i=0;i<l._N;++i){
            os<<l[i]<<"\t";
        }
    }
    else{
        for(int i=0;i<10;++i){
            os<<l[i]<<"\t";
        }
        os<<" [...] ";
        for(int i=0;i<10;++i){
            os<<l[l._N-i-1]<<"\t";
        }
    }
    return os;
}

//////////////////////////////////////////////////////////////
//
//
    
#endif

/*
int main(){
    DoubleLinkedList<> A(200,999);
   // cout<<A<<endl;
    A+=2;
    A+=2;
    A+=2;
    cout<<A._IteratorPosition<<endl;
    A[0]=0;
    cout<<A._IteratorPosition<<"="<<A[0]<<endl;

    A[1]=1;
    cout<<A._IteratorPosition<<"="<<A[1]<<endl;

    A[2]=2;
    cout<<A._IteratorPosition<<"="<<A[2]<<endl;

    A[3]=3;
    cout<<A._IteratorPosition<<"="<<A[3]<<endl;

//    cout<<A[2]<<endl;
//    cout<<A[3]<<endl;
 //   A+=-100;
 //   cout<<A<<"awd"<<endl;
  // // A+=10;
 //   cout<<A<<endl;
//
    return 0;
}

*/

