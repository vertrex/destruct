#ifndef DESTRUCT_DSERIALIZE_HPP_
#define DESTRUCT_DSERIALIZE_HPP_

#include <fstream>
#include <iostream> 
#include <vector>

#include "destruct.hpp"
#include "dstream.hpp"
#include "protocol/dcppobject.hpp"

namespace Destruct
{

class DStruct;
class DStream;

class DSerialize  //public DCppObject ! doit pouvoir se passer as soit meme qunad call serializeText / serialzieRaw etc... pour pouvoir appeller la method de base sans recreer un dserializer;  
{
public:
  virtual const DUnicodeString name(void) = 0;
  virtual bool serialize(DStream& output, DObject*  dobject)  = 0;
  virtual bool deserialize(DStream& output, DObject* dobject) = 0;
  virtual bool serialize(DStream& output, DFunctionObject* value, DType::Type_t argumentType, DType::Type_t returnType) = 0; //NEEDED FOR RPCSTREAM (serialize funciton arguement to server !
  
  virtual bool serialize(DStream& output, DStruct& dstruct) = 0;
  virtual DStruct* deserialize(DStream& output) = 0;
  virtual bool serialize(DStream& output, DValue value, DType::Type_t type) = 0;
  virtual DValue deserialize(DStream& input, DType::Type_t type) = 0; //inplky type is in descriptor/databuff
  virtual DValue deserialize(DStream& input, DType::Type_t returnType, DType::Type_t argumentType) = 0;

  virtual DSerialize* create(void) = 0;
  virtual ~DSerialize() {};
};

class DSerializers
{
public:
  EXPORT DSerializers();
  EXPORT ~DSerializers(); 
  EXPORT static DSerialize* to(const DUnicodeString type);
  EXPORT static DSerialize* to(size_t id);
  EXPORT static size_t      count(void);
  EXPORT static bool        registerSerializer(DSerialize* serializer);
private:
  static std::vector<DSerialize*  > __serializers;
};

}

#endif
