#include "dserializetext.hpp"

#include "dunicodestring.hpp"

/** 
 *  Text serialization
 */

namespace Destruct
{

SerializeText::SerializeText(DStruct* dstruct, DValue const& args) : DCppObject<SerializeText>(dstruct, args), __stream(args), __depth(0)
{
  this->init(); 
}

SerializeText::SerializeText(SerializeText const& rhs) : DCppObject<SerializeText>(rhs), __stream(rhs.__stream), __depth(0)
{
  this->copy(this, rhs);
}

SerializeText::~SerializeText()
{
}

void    SerializeText::sDObject(DValue const& args)
{
  int x = 0;
  DObject* dobject = args;
  DStruct const* dstruct = dobject->instanceOf();
  //DUnicodeString output = std::string(2 * this->__depth, ' ');
  DUnicodeString output; // = std::string(2 * this->__depth, ' ');
  output += dobject->instanceOf()->name() + "\n" + std::string(2 * this->__depth, ' ') + "{\n";
  this->sDUnicodeString(RealValue<DUnicodeString>(output));
  
  int32_t index = dobject->instanceOf()->findAttribute("iterator");
  if (index != -1)
  {
    DObject* iterator = dobject->call("iterator");
    DType::Type_t returnType = iterator->instanceOf()->attribute("currentItem").type().getReturnType();

    DValue count = dobject->call("size");
    
    for (; iterator->call("isDone").get<DInt8>() != 1; iterator->call("nextItem"))
    {
      DValue value = iterator->call("currentItem");
      if (returnType == DType::DObjectType)
      {
        this->sDUnicodeString(RealValue<DUnicodeString>(output));
        this->__depth += 1;
        this->sDObject(RealValue<DObject*>(value));
        this->__depth -= 1;
        this->sDUnicodeString(RealValue<DUnicodeString>(output));
      }
      else
      {
        output = std::string(2 * this->__depth, ' ');
        output += value.asUnicodeString() + ",\n";
        this->sDUnicodeString(RealValue<DUnicodeString>(output));
      }    
    }
    iterator->destroy();
    output = std::string(2 * this->__depth, ' ') + "};";
    this->sDUnicodeString(RealValue<DUnicodeString>(output));
    return ;
  }

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    DUnicodeString output = "  " + std::string(2 * this->__depth, ' ');
    output += i->type().name(); 
    output += " " + i->name() + " = ";
    this->sDUnicodeString(RealValue<DUnicodeString>(output));
    this->__depth += 1;
    this->call(i->type().name(), dobject->getValue(x));
    this->__depth -= 1;
    output = std::string("\n");
    this->sDUnicodeString(RealValue<DUnicodeString>(output));
  }
  output = std::string(2 * this->__depth, ' ');
  output += "};";
  this->sDUnicodeString(RealValue<DUnicodeString>(output));
}

void    SerializeText::sDStruct(DValue const& args)
{
  DStruct const* dstruct = args.get<DStruct*>();
  DUnicodeString output = dstruct->name() + "\n{\n";

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i)
    output += "  " + (*i).type().name() + " " + (*i).name() + ";\n";

  output += "}\n"; 

  this->sDUnicodeString(RealValue<DUnicodeString>(output)); 
}

void    SerializeText::sDNone(void)
{
  DUnicodeString unicodeString = RealValue<DObject*>(DNone).asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDMethod(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDUnicodeString(DValue const& args)
{
  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(args.asDBuffer()));
}

void    SerializeText::sDBuffer(DValue const& args)
{
  //XXX implement me
  //stream->call("write", RealValue<DBuffer>(args);
}

void    SerializeText::sDInt8(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDInt16(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDInt32(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDInt64(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDUInt8(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDUInt16(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDUInt32(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

void    SerializeText::sDUInt64(DValue const& args)
{
  DUnicodeString unicodeString = args.asUnicodeString();
  DBuffer buffer((uint8_t*)unicodeString.c_str(), (int32_t)unicodeString.size());

  ((DObject*)this->__stream)->call("write", RealValue<DBuffer>(buffer));
}

}
