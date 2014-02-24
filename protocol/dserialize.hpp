#ifndef DESTRUCT_DSERIALIZE_HPP_
#define DESTRUCT_DSERIALIZE_HPP_

#include <fstream>
#include <iostream> 
#include <vector>

#include "dstream.hpp"

namespace Destruct
{

class DObject;
class DStruct;
class DStream;

class DSerialize
{
public:
  virtual const std::string  name(void) = 0;
  virtual bool serialize(DStream& output, DObject&  dobject)  = 0;
  virtual bool deserialize(DStream& output, DObject& dobject) = 0;
  
  virtual bool serialize(DStream& output, DStruct& dstruct) = 0;
  virtual DStruct* deserialize(DStream& output) = 0;  //Can't pass DStruct by ref& an object must be create void for deserialization that's will be strange
  virtual DSerialize* create(void) = 0;
  virtual ~DSerialize() {};
};

class DSerializeXML : public DSerialize
{
public :
  DSerializeXML()
  {
  }

  const std::string name(void);
  DSerializeXML*   create(void);
  bool serialize(DStream& output, DObject& dobject); 
  bool serialize(DStream& output, DObject& dobject, int depth);
  bool deserialize(DStream&input, DObject& dobject);

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

  bool serialize(DStream& output, DStruct& dstruct); 
  DStruct* deserialize(DStream& output); //can overload return type ...
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
