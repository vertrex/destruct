#include "dserializetext.hpp"

#include "dunicodestring.hpp"

/** 
 *  Text serialization
 */

namespace Destruct
{

SerializeText::SerializeText(DStruct* dstruct, DValue const& args) : DCppObject<SerializeText>(dstruct, args), __stream(args.get<DObject*>())
{
  this->init(); 
}

SerializeText::SerializeText(SerializeText const& copy) : DCppObject<SerializeText>(copy), __stream(copy.__stream)
{
  this->init();
}

SerializeText::~SerializeText()
{
  ((DObject*)__stream)->destroy();
}

void    SerializeText::sDObject(DValue const& args)
{
/*
  DObject* dobject = args.get<DObject*>();
  DStruct const* dstruct = dobject->instanceOf();

  int32_t index = dobject->instanceOf()->find("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator").get<DObject*>();
    DType::Type_t returnType = iterator->instanceOf()->attribute("currentItem").type().getReturnType();

    DValue count = dobject->call("size");
    
    for (; iterator->call("isDOne").get<DInt8>() != 1; iterator->call("nextItem"))
    {
      DValue value = iterator->call("currentItem");
      if (returnType == DType::DObjectType)
      {
        DObject* subDObject = value.get<DObject*>();
        if (subDObject)
        {
           std::string res = std::string(2 * this->__depth, ' ') + subDObject->instanceOf()->name() + " "+ std::endl;
           this->sDUnicodeString(res);
           this->__depth += 1;
           this->sDObject(RealValue<DObject*>(subDObject));
           this->__depth -= 1;    
           res = std::string(2 * this->__depth, ' ') + "};" + std::endl;
           this->sDUnicodeString(res);
           //subDObject->destroy();
           //subDObject->destroy();
        }

      }
      else
      {
        std::string res = std::string(2 * depth, ' ') + value.asUnicodeString + "," + std::endl;
        this->sDUnicodeString(res);
      }    

    }
    //iterator->destroy();
    //iterator->destroy();
    return ;
  }

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {

  }*/
}

void    SerializeText::sDStruct(DValue const& args)
{

}

void    SerializeText::sDNone(void)
{
}

void    SerializeText::sDMethod(DValue const& args)
{
  //pass or throw ?
}

void    SerializeText::sDUnicodeString(DValue const& args)
{
}

void    SerializeText::sDBuffer(DValue const& args)
{

}

void    SerializeText::sDInt8(DValue const& args)
{
}

void    SerializeText::sDInt16(DValue const& args)
{
}

void    SerializeText::sDInt32(DValue const& args)
{
}

void    SerializeText::sDInt64(DValue const& args)
{
}

void    SerializeText::sDUInt8(DValue const& args)
{
}

void    SerializeText::sDUInt16(DValue const& args)
{
}

void    SerializeText::sDUInt32(DValue const& args)
{
}

void    SerializeText::sDUInt64(DValue const& args)
{
}

}
