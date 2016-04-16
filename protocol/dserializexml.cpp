#include "dserializexml.hpp"

namespace Destruct
{

SerializeXML::SerializeXML(DStruct* dstruct, DValue const& args) : DCppObject<SerializeXML>(dstruct, args), __stream(args.get<DObject*>()), __depth(1)
{
  this->init(); 
}

SerializeXML::SerializeXML(SerializeXML const& rhs) : DCppObject<SerializeXML>(rhs), __stream(rhs.__stream), __depth(1)
{
  this->copy(this, rhs);
}

SerializeXML::~SerializeXML()
{
}

void    SerializeXML::sDObject(DValue const& args)
{
  int x = 0;
  DObject* dobject = args;
  DStruct const* dstruct = dobject->instanceOf();
  XMLTag tag = XMLTag(this->__stream, dstruct->name(), "\n", this->__depth++, true);

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    //XXX handle vector / map 
    if (i->type().getType() == DType::DObjectType)
    {
       DValue subObject = dobject->getValue(x);

       XMLTag xmlObj  = XMLTag(this->__stream, i->name(), "\n", this->__depth, true);
       this->__depth += 1;
       this->sDObject(subObject);
       this->__depth -= 1;
    }
    else    
      XMLTag(this->__stream, i->name(), dobject->getValue(x).asUnicodeString(), this->__depth);
  }   
}

void    SerializeXML::sDStruct(DValue const& args)
{
  DStruct const* dstruct = args.get<DStruct*>();
  XMLTag ref = XMLTag(this->__stream, dstruct->name(), "\n", 1, true);

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i)
    XMLTag(this->__stream, (*i).name(), (*i).type().name(), 2); 
}

void    SerializeXML::sDNone(void)
{
  DUnicodeString unicodeString = RealValue<DObject*>(DNone).asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDMethod(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDUnicodeString(DValue const& args)
{
  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeXML::sDBuffer(DValue const& args)
{
  //stream->call("write", RealValue<DBuffer>(args);
}

void    SerializeXML::sDInt8(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDInt16(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDInt32(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDInt64(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDUInt8(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDUInt16(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDUInt32(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeXML::sDUInt64(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

}
