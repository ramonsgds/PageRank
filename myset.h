#ifndef MYSET_H
#define MYSET_H
#include <set>

template<class T>
class myset : public std::set<T>
{
public:
  myset();
  myset<T> set_intersection(const myset<T>& other);
  myset<T> set_union(const myset<T>& other);
};


 
template<class T> myset<T>::myset() : std::set<T>() {

};

template<class T> myset<T> myset<T>::set_intersection(const myset<T>& other) {
    myset<T> mysetholder;
    for (class std::set<T>::iterator it = this->begin(); it != this->end(); it++){
        if ( other.count(*it) > 0){
              mysetholder.insert(*it);
        }
    }
    return mysetholder;
}

template<class T>myset<T> myset<T>::set_union(const myset<T>& other) {
	myset<T> mysetholder;
    for (class std::set<T>::iterator it = this->begin(); it != this->end(); it++){
          mysetholder.insert(*it);
    }
    for (class std::set<T>::iterator secit = other.begin(); secit != other.end(); secit++){
          if ( mysetholder.count(*secit) == 0 ) {
                 mysetholder.insert(*secit);
          } 
    }
    return mysetholder;
}

#endif
