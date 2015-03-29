#include "dserialize.hpp"

namespace Destruct
{

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
public:
  /*
  static size_t ownAttributeCount()
  {
    return (0);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      //DAttribute(DType::Type_t::DInt64Type,"read", DType::Type_t::DObjectType), 
      //DAttribute(DType::Type_t::DInt64Type,"write",  DType::Type_t::DObjectType),
    };
    return (attributes);
  }

  static DPointer<DSerializeXML>* memberBegin()
  {
    static DPointer<DSerializeXML> memberPointer[] = 
    {
      //DPointer<DVectorType>(&DVectorType::pushObject, &DVectorType::push),
      //DPointer<DVectorType>(&DVectorType::getObject, &DVectorType::get),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DSerializeXML>*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  } */
};

}
