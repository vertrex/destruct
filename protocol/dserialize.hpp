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
/*
class DStruct::write()
{
  for this->attribute:
     serialize.writeAttribute(value);


}

class DObject::write()
{
  for this->dstruct->value != func or func without arg 
  {
    serializer.writeValue(value);
  }

  if serialize.type == textserializer

  if serliaze.type == raw 
}

class Raw()
{
  read int
  read long
  read buff
  read list  
}

DStream()
{
  seek
  read
  []
  open/close 
}

DBuffer : DStream
{
  DBUffer(size)
  read(, offset)
  serialize()
        for x in buff[x]
           serializer.writevalue(x)
}

DMap 
{
  serialize:
        for key, value:
         serializer.write(key)
         serializer.write(value)
}




class NTFS()
{

  DValue serialize:

        return lsit de dvalue
                [valuem object., (funcobject, argument), funcobject]

        for i in ntfs.serialize
             xml.serializevalue(value)
             
             if (dfunctiobobject, args)
              value =  dfunction(args)
              xml.serializevalue(value)

}

class XML(Stream)
{
  writeStructBase()


  for read in dstruct in stream:
     read object
        ..

  for object in object:
        write struct if not yet written:
              write(object)


  XMLSerialize(object, recurse);
  XMLDSerialize(&object, recusrse);
  XMLDSerialize(dstruct)
  XMLSerialize(dstruct)
  XMLSerialize(value)
  XMLDEserializ(value)

  writeValue()
  writeAttribute()
}
*/

class DSerialize 
{
public:
  virtual const std::string  name(void) = 0;
  virtual bool serialize(DStream& output, DObject&  dobject)  = 0;
  virtual bool deserialize(DStream& output, DObject& dobject) = 0;
  
  virtual bool serialize(DStream& output, DStruct& dstruct) = 0;
  virtual DStruct* deserialize(DStream& output) = 0;  //Can't pass DStruct by ref& an object must be create void for deserialization that's will be strange
  virtual bool serialize(DStream& output, DValue value, DType type) = 0;
  virtual DValue deserialize(DStream& input, DType type) = 0; //inplky type is in descriptor/databuff


  virtual DSerialize* create(void) = 0;
  virtual ~DSerialize() {};
/*
  DValue  readValue();
  DObject readObject();
  DInt32  readInt32();


  object.write(sterializer)
  serializer.writeValue(value)
  serializer.writeValue(value2)
  serializer.writeValue(object.toto)
  
  deserializer.readValue(a)
  deserializer.readValue(b)

//ok mais vraiment generique ?? 
// faire un exemple : raw / xml / json
// et pour le .dbin c peut etre special car y a la descritpion a ecrire (mais pour xml, json aussi pourquoi pa ? 
*/
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
  bool serialize(DStream& output, DValue value, DType type);
  DValue deserialize(DStream& input, DType type);

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
      //DAttribute(DType::DInt64Type,"read", DType::DObjectType), 
      //DAttribute(DType::DInt64Type,"write",  DType::DObjectType),
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
  bool serialize(DStream& output, DValue value, DType type);
  DValue deserialize(DStream& input, DType type);

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

  bool serialize(DStream& output, DValue value, DType type);
  DValue deserialize(DStream& input, DType type);
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


  bool serialize(DStream& output, DValue value, DType type);
  DValue deserialize(DStream& input, DType type);
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
