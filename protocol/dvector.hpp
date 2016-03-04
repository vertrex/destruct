#ifndef DESTRUCT_DVECTOR_HPP_
#define DESTRUCT_DVECTOR_HPP_

#include "protocol/dcppobject.hpp"
#include "protocol/dcontainer.hpp"
#include "protocol/dstream.hpp"

namespace Destruct
{

template <typename VectorType, DType::Type_t  VectorTypeId>
class DVector : public DContainer, public DCppObject<DVector<VectorType, VectorTypeId> >
{
  typedef DVector<VectorType, VectorTypeId> DVectorType;
public:
  DVector(DStruct* dstruct, DValue const& args) : DCppObject<DVector<VectorType, VectorTypeId> >(dstruct, args)
  {
    this->init();
  }

  DVector(const DVectorType& copy) : DCppObject<DVector<VectorType, VectorTypeId> >(copy), __vector(copy.__vector) 
  {
    this->init();
  }

  ~DVector()
  {
    this->__vector.clear();
  }

  DUInt64  push(DValue const& args) 
  {
    this->__vector.push_back(args.get<VectorType>());
    return (this->__vector.size() - 1);
  }
   
  DValue  get(DValue const& args)
  {
    DUInt64 index = args.get<DUInt64>();
    if (index >= this->__vector.size())
      throw DException(std::string("DContainer::get bad index\n"));
      
    return (RealValue<VectorType>(this->__vector[index]));
  }

  DUInt64   size(void)
  {
    return (this->__vector.size());
  }

  void  clear(void)
  {
     this->__vector.clear();
  }

  DObject*    setItem(DValue const& args)
  {
    DObject*     argumentsObject = args.get<DObject*>();
    DInt64       index = argumentsObject->getValue("index").get<DUInt64>();
    VectorType   item = argumentsObject->getValue("value").get<VectorType>();

    argumentsObject->destroy();

    if (index >= (DInt64)this->__vector.size())
     throw DException(std::string("setItem : Index error"));

    this->__vector[index] = item; 
 
    return (DNone);
  }

  DObject*  iterator(void)
  {
    DUnicodeString iteratorname = DUnicodeString("DIterator") + DType(VectorTypeId).name();
    return (DStructs::instance().generate(iteratorname, RealValue<DObject*>(this))); 
  }

  RealValue<DFunctionObject* >  _push, _clear;

  /*
   *  DStruct declaration
   */ 
  static size_t ownAttributeCount()
  {
    return (6);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DUInt64Type,"push", VectorTypeId), 
      DAttribute(VectorTypeId, "get",  DType::DUInt64Type),
      DAttribute(DType::DUInt64Type,"size", DType::DNoneType),
      DAttribute(DType::DNoneType, "setItem", DType::DObjectType),
      DAttribute(DType::DObjectType, "iterator", DType::DNoneType),
      DAttribute(DType::DNoneType, "clear", DType::DNoneType),
    };
    return (attributes);
  }

  static DPointer<DVectorType>* memberBegin()
  {
    static DPointer<DVectorType> memberPointer[] = 
    {
      DPointer<DVectorType>(&DVectorType::_push, &DVectorType::push),
      DPointer<DVectorType>(&DVectorType::_get, &DVectorType::get),
      DPointer<DVectorType>(&DVectorType::_size, &DVectorType::size),
      DPointer<DVectorType>(&DVectorType::_setItem, &DVectorType::setItem),
      DPointer<DVectorType>(&DVectorType::_iterator, &DVectorType::iterator),
      DPointer<DVectorType>(&DVectorType::_clear, &DVectorType::clear),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DVectorType >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } 
private:
  std::vector<VectorType>   __vector;
};


/**
 *  DObject template specialization
 */
template<>
inline DVector<DObject*, DType::DObjectType >::~DVector()
{
 std::vector<DObject* >::iterator object = this->__vector.begin();
 for (; object != this->__vector.end(); ++object)
   (*object)->destroy();
 this->__vector.clear();
}

template<>
inline DUInt64 DVector<DObject*, DType::DObjectType >::push(DValue const& args)
{
  DObject* object = args.get<DObject*>();//add ref
  this->__vector.push_back(object);
  return (this->__vector.size() - 1);
}
  
template<> 
inline DValue  DVector<DObject*, DType::DObjectType>::get(DValue const& args)
  {
    DUInt64 index = args.get<DUInt64>();
    if (index >= this->__vector.size())
      throw DException(std::string("DContainer::get bad index\n"));
    DObject* object = this->__vector[index];
    object->addRef(); // ? segfault in python
    return (RealValue<DObject*>(object)); //addRef
  }
}
#endif
