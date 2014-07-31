#ifndef DESTRUCT_DVECTOR_HPP_
#define DESTRUCT_DVECTOR_HPP_

#include "protocol/dcppobject.hpp"
#include "protocol/dcontainer.hpp"
#include "protocol/dstream.hpp"
#include "protocol/dserialize.hpp"

namespace Destruct
{

template <typename VectorType, DType::Type_t  VectorTypeId>
class DVector : public DContainer, public DCppObject<DVector<VectorType, VectorTypeId> > //herite en DStruct plutot ?
{
  typedef DVector<VectorType, VectorTypeId> DVectorType;
public:
//XXX prendre container en argument ? DStruct* dstruct, DValue args) comme ca ca fait un constructeur 
// par contre faut que new object prenne une value donc modifier destruct / dstruct et tous les dobjects
  DVector(DStruct* dstruct, DValue const& args) : DCppObject<DVector<VectorType, VectorTypeId> >(dstruct, args)
  {
    this->init(); //XXX if not init par lui meme mais par dcppobject push sera pas init on dirait qu il init une copie des object qui sont DCOntainer comme si il herite aussi de DContainer ??
  };

  DVector(const DVectorType& copy) : DCppObject<DVector<VectorType, VectorTypeId> >(copy), __vector(copy.__vector) 
  {
    this->init();
  }

  ~DVector()
  {
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
      throw DException("DContainer::get bad index\n");
      
    return (RealValue<VectorType>(this->__vector[index]));
  }

  DUInt64   size(void)
  {
    return (this->__vector.size());
  }

  DObject*    setItem(DValue const& args)
  {
    DObject*     argumentsObject = args.get<DObject*>();
    DInt64       index = argumentsObject->getValue("index").get<DUInt64>();
    VectorType   item = argumentsObject->getValue("value").get<VectorType>();
     
    if (index >= (DInt64)this->__vector.size()) 
     throw DException("setItem : Index error");    

    this->__vector[index] = item; 
 
    return (DNone);
  }

  DObject*  iterator(void)
  {
    DObject* iterator = Destruct::instance().generate("DIterator", RealValue<DObject*>(this));
    return (iterator);
  }

  DObject*  serializeText(DValue const& args) //dserializer ! 
  {
    //getserializationType ou serializeXML method serializeText method de toute c dynamique donc si on c serialized un type on peut call la method qu il faut
    // c une naming convetion
    //DStream* output = static_cast<DStream*>(args.get<DObject*>());
    DStream* output = static_cast<DStream*>(args.get<DObject*>()->getValue("stream").get<DObject*>());

    *output << "list : {" << std::endl;
    DUInt64 size = static_cast<DFunctionObject*>(this->_size)->call().get<DUInt64>();
    for (DUInt64 idx = 0; idx < size ; idx++) //cout < this->sizeObject()->call()->get<DUint>
    {
      //XXX devrait stocker des RealValue de toute ? plus logique mais prend plus de ram ?
//      if this->__vector->type == object ?

      *output << RealValue<VectorType>(this->__vector[idx]).asUnicodeString() << "," << std::endl; //curent item
    }
    *output << "}" << std::endl;
      
    return RealValue<DObject*>(DNone);
  }

  RealValue<DFunctionObject* >  _serializeText;
  RealValue<DFunctionObject* >  _push;
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
      DAttribute(DType::DObjectType, "serializeText", DType::DObjectType),
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
      DPointer<DVectorType>(&DVectorType::_serializeText, &DVectorType::serializeText),
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
  std::vector<VectorType>   __vector; //XXX sont on stock des dvalue le type doit suffir pour mette n importe koi et etre generic 
};
//template specialization pour les DObject* si la template est dobject il faut call le serialize des object 
template<>
DObject* DVector<DObject*, DType::DObjectType >::serializeText(DValue const& args)
{
  DObject* arguments = args.get<DObject*>();
  DObject* stream = arguments->getValue("stream").get<DObject*>();
  DStream* output = static_cast<DStream*>(stream);
  DInt32 depth = arguments->getValue("depth").get<DInt32>();

  *output << "list : {" <<  std::string(2*depth, ' ') << std::endl;

  //DUInt64 size = static_cast<DFunctionObject*>(this->_size)->call().get<DUInt64>();
  DUInt64 size = this->call("size").get<DUInt64>(); // static_cast<DFunctionObject*>(this->_size)->call().get<DUInt64>();
  std::cout << "size " << size << std::endl;
  for (DUInt64 idx = 0; idx < size ; idx++)
  {
    std::cout << "foring " << std::endl;
    DSerializers::to("Text")->serialize(*output, *this->__vector[idx]);
  }
  *output << std::string(2*depth, ' ')  << "}" << std::endl;

  return RealValue<DObject*>(DNone);
}

}
#endif
