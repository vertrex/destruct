#ifndef _DESTRUCT_DPOLICY_HPP
#define _DESTRUCT_DPOLICY_HPP

#include <stdint.h>

#include <iostream>
#include <string>

template< class Derived >
class RefcountPolicy 
{
public:
  RefcountPolicy() : __refCount(1)
  {
  }

  int32_t refCount() const
  { 
    return this->__refCount; 
  }

  void    addRef() 
  {
    this->__refCount++;  
  }

  virtual void  destroy()
  {
    this->__refCount--;
    if (this->__refCount <= 0)
    {
      delete static_cast<Derived*>(this);
    }
  }

//protected:
  void    delRef()
  {
    this->__refCount--;
  }

private:
  volatile int32_t  __refCount;
};

#endif 
