#ifndef DESTRUCT_DSERIALIZEXML_HPP_
#define DESTRUCT_DSERIALIZEXML_HPP_

#include "protocol/dcppobject.hpp"

namespace Destruct
{

class SerializeXML : public DCppObject<SerializeXML> 
{
public:
  SerializeXML(DStruct* dstruct, DValue const& args);
  SerializeXML(SerializeXML const& copy);

  DObject*   stream(void);

  void       sDInt8(DValue const& args);
  void       sDInt16(DValue const& args);
  void       sDInt32(DValue const& args);
  void       sDInt64(DValue const& args);

  void       sDUInt8(DValue const& args);
  void       sDUInt16(DValue const& args);
  void       sDUInt32(DValue const& args);
  void       sDUInt64(DValue const& args);

  void       sDUnicodeString(DValue const& args);

  void       sDObject(DValue const& args);
  void       sDMethod(DValue const& args);
  void       sDNone(void);
  void       sDStruct(DValue const& args);
  void       sDBuffer(DValue const& args);

protected:
  ~SerializeXML();

public:
  RealValue<DFunctionObject* > _DObject, _DStruct, _DUnicodeString, _DNone, _DMethod, _DBuffer, 
                               _DInt8, _DInt16, _DInt32, _DInt64,
                               _DUInt8, _DUInt16, _DUInt32, _DUInt64;

  RealValue<DObject*>         __stream;
  DUInt32                     __depth;

  static size_t ownAttributeCount()
  {
    return (15);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
       DAttribute(DType::DObjectType, "stream"),

       DAttribute(DType::DNoneType,"DInt8", DType::DInt8Type), 
       DAttribute(DType::DNoneType,"DInt16", DType::DInt16Type), 
       DAttribute(DType::DNoneType,"DInt32", DType::DInt32Type), 
       DAttribute(DType::DNoneType,"DInt64", DType::DInt64Type), 

       DAttribute(DType::DNoneType,"DUInt8", DType::DUInt8Type), 
       DAttribute(DType::DNoneType,"DUInt16", DType::DUInt16Type), 
       DAttribute(DType::DNoneType,"DUInt32", DType::DUInt32Type), 
       DAttribute(DType::DNoneType,"DUInt64", DType::DUInt64Type), 
       
       DAttribute(DType::DNoneType, "DUnicodeString", DType::DUnicodeStringType),

       DAttribute(DType::DNoneType, "DObject", DType::DObjectType), 
       DAttribute(DType::DNoneType, "DMethod", DType::DMethodType),
       DAttribute(DType::DNoneType, "DNone", DType::DNoneType),
       DAttribute(DType::DNoneType, "DStruct", DType::DStructType), 
       DAttribute(DType::DNoneType, "DBuffer", DType::DBufferType), 

    };
    return (attributes);
  }

  static DPointer<SerializeXML>* memberBegin()
  {
    static DPointer<SerializeXML> memberPointer[] = 
    {
       DPointer<SerializeXML>(&SerializeXML::__stream),
        
       DPointer<SerializeXML>(&SerializeXML::_DInt8, &SerializeXML::sDInt8),
       DPointer<SerializeXML>(&SerializeXML::_DInt16, &SerializeXML::sDInt16),
       DPointer<SerializeXML>(&SerializeXML::_DInt32, &SerializeXML::sDInt32),
       DPointer<SerializeXML>(&SerializeXML::_DInt64, &SerializeXML::sDInt64),

       DPointer<SerializeXML>(&SerializeXML::_DUInt8, &SerializeXML::sDUInt8),
       DPointer<SerializeXML>(&SerializeXML::_DUInt16, &SerializeXML::sDUInt16),
       DPointer<SerializeXML>(&SerializeXML::_DUInt32, &SerializeXML::sDUInt32),
       DPointer<SerializeXML>(&SerializeXML::_DUInt64, &SerializeXML::sDUInt64),

       DPointer<SerializeXML>(&SerializeXML::_DUnicodeString, &SerializeXML::sDUnicodeString),

       DPointer<SerializeXML>(&SerializeXML::_DObject, &SerializeXML::sDObject),
       DPointer<SerializeXML>(&SerializeXML::_DMethod, &SerializeXML::sDMethod),
       DPointer<SerializeXML>(&SerializeXML::_DNone, &SerializeXML::sDNone),
       DPointer<SerializeXML>(&SerializeXML::_DStruct, &SerializeXML::sDStruct),
       DPointer<SerializeXML>(&SerializeXML::_DBuffer, &SerializeXML::sDBuffer),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<SerializeXML>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }

private:
  class XMLTag
  {
  public:
    XMLTag(DObject* stream, DUnicodeString const& tag, DUnicodeString const& value = "", int depth = 1, bool enclose = false) : __stream(stream), __tag(tag), __depth(depth), __enclose(enclose)
    {
     DUnicodeString output = (std::string(2*(depth - 1), ' ')); 
     output +=  "<" + tag + ">" + value;   
     this->__stream->call("write", RealValue<DBuffer>(RealValue<DUnicodeString>(output).asDBuffer()));
    }

    ~XMLTag()
    {
     DUnicodeString output;
     if (this->__enclose)
     {
       output = DUnicodeString(std::string(2*(this->__depth - 1), ' '));
       output +=  "</"  + this->__tag + ">\n";
     }
     else
       output =  "</" + this->__tag + ">\n";
     this->__stream->call("write", RealValue<DBuffer>(RealValue<DUnicodeString>(output).asDBuffer()));
    }

  private:
    DObject*             __stream;
    const DUnicodeString __tag;
    int                  __depth;
    bool                 __enclose;
  };
};

/*
class DSerializeXML : public DSerialize//, public DCppObject<DSerializeXML>
{
public :
  DSerializeXML(DStruct* dstruct, DValue const& args);
  DSerializeXML(const DSerializeXML& copy);
  const DUnicodeString name(void);
  DSerializeXML*   create(void);

  bool     serialize(DStream& output, DObject* dobject); 
  bool     serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType);
  bool     serialize(DStream& output, DObject* dobject, int depth);
  bool     serialize(DStream& output, DValue value, DType::Type_t type);
  bool     serialize(DStream& output, DStruct& dstruct); 

  bool     deserialize(DStream&input, DObject* dobject);
  DValue   deserialize(DStream& input, DType::Type_t type);
  DValue   deserialize(DStream& input, DType::Type_t returnType, DType::Type_t argumentType);
  DStruct* deserialize(DStream& output); 
private:

  class XMLTag
  {
  public:
    XMLTag(DStream& output, DUnicodeString const & tag, DUnicodeString const & value = "", int depth = 1, bool enclose = false) : __output(output), __tag(tag), __depth(depth), __enclose(enclose)
    {
      output  << DUnicodeString(std::string(2*(depth - 1), ' ')) <<  '<' << tag << '>' << value;   
    }

    ~XMLTag()
    {
     if (this->__enclose)
       this->__output <<DUnicodeString(std::string(2*(this->__depth - 1), ' ')) <<  "</" <<this->__tag << '>' << std::endl;
     else
       this->__output <<  "</" <<this->__tag << '>' << std::endl;
    }

  private:
    DStream&           __output;
    const DUnicodeString __tag;
    int                __depth;
    bool               __enclose;
  };
};
*/
}

#endif
