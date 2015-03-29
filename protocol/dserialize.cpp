#include "dserialize.hpp"

#include "dstruct.hpp"
#include "dstructs.hpp"
#include "dobject.hpp"
#include "dsimpleobject.hpp"
#include "dnullobject.hpp"
#include "dmutableobject.hpp"
#include "drealvalue.hpp"
#include "dserializetext.hpp"
#include "dserializeraw.hpp"
#include "dserializebinary.hpp"
#include "dserializexml.hpp"

#include <iostream>

namespace Destruct
{

/* 
 *  Serializers factory 
 */ 
std::vector<DSerialize* > DSerializers::__serializers;
static DSerializers dserializers = DSerializers();

DSerializers::DSerializers()
{
  //type
  //Encoders ? != serializer ? 
  this->registerSerializer(new DSerializeXML(NULL, RealValue<DObject*>(DNone)));
  this->registerSerializer(new DSerializeText());
  this->registerSerializer(new DSerializeBinary());
  this->registerSerializer(new DSerializeRaw());
}

DSerializers::~DSerializers()
{
  for (std::vector<DSerialize* >::iterator i = __serializers.begin(); i != __serializers.end(); ++i)
  {
    //delete (*i);
  }
}

size_t DSerializers::count()
{
  return (__serializers.size());
}

DSerialize* DSerializers::to(size_t id)
{
  if (id >= __serializers.size())
    return (NULL); 
  return (__serializers[id]);
}

DSerialize* DSerializers::to(const DUnicodeString type)
{
  std::vector<DSerialize* >::iterator i;

  for (i = __serializers.begin() ; i != __serializers.end(); ++i)
  {
    if (type == (*i)->name())
      return ((*i)->create()); 
  }
  return (NULL); 
}

bool DSerializers::registerSerializer(DSerialize* serializer)
{
  __serializers.push_back(serializer);
  return (true);
}

}
