#ifndef IList_h
#define IList_h

template <class T>
class IList
{
public:
    IList<T>(){}
    virtual size_t size() = 0;
    virtual T get(int index) = 0;
    virtual void add(T data) = 0;
    virtual T operator[](int index) = 0;
    virtual void empty() = 0;
};
#endif