#ifndef DESTRUCT_DSERIALIZE_HPP_
#define DESTRUCT_DSERIALIZE_HPP_

#include <fstream>
#include <iostream> 
#include <vector>

#include "dstream.hpp"
#include "protocol/dcppobject.hpp"

namespace Destruct
{

class DStruct;
class DStream;

class DSerialize 
{
public:
  virtual const std::string  name(void) = 0;
  virtual bool serialize(DStream& output, DObject&  dobject)  = 0;
  virtual bool deserialize(DStream& output, DObject& dobject) = 0;
  
  virtual bool serialize(DStream& output, DStruct& dstruct) = 0;
  virtual DStruct* deserialize(DStream& output) = 0;
  virtual bool serialize(DStream& output, DValue value, DType::Type_t type) = 0;
  virtual DValue deserialize(DStream& input, DType::Type_t type) = 0; //inplky type is in descriptor/databuff

  virtual DSerialize* create(void) = 0;
  virtual ~DSerialize() {};
};

class DSerializeBinary : public DSerialize
{
/*
 *  This dserialize in a binary compatible format. That can be used for an archive format 
 */
public:
  DSerializeBinary()
  {
  }

  const std::string name(void);
  DSerializeBinary*   create(void);

  bool serialize(DStream& output, DObject& dobject); 
  bool deserialize(DStream& input, DObject& dobject);

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& input);

// pascal string deserialization
  bool serialize(DStream& output, const std::string& str);
  bool deserialize(DStream& input, std::string& str);

  bool serialize(DStream& output, DValue value, DType::Type_t type);
  DValue deserialize(DStream& input, DType::Type_t type);
};

class DSerializeText : public DSerialize
{
public:
  DSerializeText()
  {
  }

  const std::string name(void);
  DSerializeText*   create(void);
  bool serialize(DStream& output, DObject& dobject); 
  bool serialize(DStream& output, DObject& dobject, int depth);
  bool deserialize(DStream& input, DObject& dobject);
  bool serialize(DStream& output, DValue value, DType::Type_t type);
  DValue deserialize(DStream& input, DType::Type_t type);

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& output); //can overload return type ...
};

class DSerializeRaw : public DSerialize
{
/*
 *  This desrialize and deserialize as binary 'raw' 'flat' type without markers. 
 *  That can be used to read file produce by other tools.
 */
public:
  DSerializeRaw()
  {
  };

  const std::string name(void);
  DSerializeRaw*   create(void);

  bool serialize(DStream& output, DObject& dobject); 
  bool deserialize(DStream& input, DObject& dobject);

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& output); 


  bool serialize(DStream& output, DValue value, DType::Type_t type);
  DValue deserialize(DStream& input, DType::Type_t type);
};

class DSerializeXML : public DSerialize, public DCppObject<DSerializeXML>
{
public :
  //DSerializeXML()
  //{
  //}

  DSerializeXML(DStruct* dstruct, DValue const& args) : DCppObject<DSerializeXML>(dstruct, args)
  {
  }
  DSerializeXML(const DSerializeXML& copy) : DCppObject<DSerializeXML>(copy) 
  {
  }

  const std::string name(void);
  DSerializeXML*   create(void);
  bool serialize(DStream& output, DObject& dobject); 
  bool serialize(DStream& output, DObject& dobject, int depth);
  bool deserialize(DStream&input, DObject& dobject);
  bool serialize(DStream& output, DValue value, DType::Type_t type);
  DValue deserialize(DStream& input, DType::Type_t type);

  bool     serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& output); 
private:

  class XMLTag
  {
  public:
    XMLTag(DStream& output, std::string const & tag, std::string const & value = "", int depth = 1, bool enclose = false) : __output(output), __tag(tag), __depth(depth), __enclose(enclose)
    {
      output  << std::string(2*(depth - 1), ' ') <<  '<' << tag << '>' << value;   
    }

    ~XMLTag()
    {
     if (this->__enclose)
       this->__output << std::string(2*(this->__depth - 1), ' ') <<  "</" <<this->__tag << '>' << std::endl;
     else
       this->__output <<  "</" <<this->__tag << '>' << std::endl;
    }

  private:
    DStream&           __output;
    const std::string  __tag;
    int                __depth;
    bool               __enclose;
  };
public:
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
  } 
};

class DSerializers
{
public:
  DSerializers();
  ~DSerializers(); 
  static DSerialize* to(const std::string type);
  static DSerialize* to(size_t id);
  static size_t      count(void);
  static bool        registerSerializer(DSerialize* serializer);
private:
  static std::vector<DSerialize*  > __serializers;
};

}

#endif
