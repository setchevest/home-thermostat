#ifndef IList_h
#define IList_h

template <class T>
class IList
{
public:
    IList(){}
    virtual int getLenght() = 0;
    virtual T get(int index) = 0;
    virtual void add(T data) = 0;
};
#endif