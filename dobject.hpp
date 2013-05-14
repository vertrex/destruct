#ifndef _DESTRUCT_DOBJECT_HPP_
#define _DESTRUCT_DOBJECT_HPP_

#include <memory>
#include "dstruct.hpp"

namespace Destruct
{
class DValue;
class BaseValue;


class DObject
{
public:
 DObject(DStruct const* dstructDef);


 virtual DObject* clone() const = 0;
 DStruct const* instanceOf() const;
 virtual DValue getValue(size_t index) const = 0;
 virtual DValue getValue(std::string const& name) const;
 virtual void setValue(size_t idx, DValue const& ) = 0;
 virtual void setValue(std::string const& name, DValue const& );

 int32_t refCount() const
 { 
   return this->__refCount; 
 }

 void   addRef() 
 {  
    this->__refCount++;  
 }

 void   destroy()
 { 
    this->__refCount--;
    if (!this->__refCount)  
      delete this;
 }

protected:
 DObject(const DObject& rhs) : __dstructDef(rhs.__dstructDef) 
 {
 }
 virtual BaseValue* getBaseValue(size_t index) = 0;
 virtual BaseValue const* getBaseValue(size_t index) const = 0;
 static  BaseValue* getBaseValue(DObject*, size_t index);
 static  BaseValue const* getBaseValue(DObject const*, size_t idx);
 virtual ~DObject()
 {
 }
private:
 DStruct const * const __dstructDef;
 volatile int32_t      __refCount;


};

}

#endif
