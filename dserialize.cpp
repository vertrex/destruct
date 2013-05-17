#include "dserialize.hpp"
#include "dstruct.hpp"
#include "dobject.hpp"

#include <iostream>

namespace Destruct
{

/*
 *   XML serialization
 */ 

const std::string DSerializeXML::name(void)
{
  return ("XML");
}

DSerializeXML*   DSerializeXML::create(void)
{
  return (new DSerializeXML);
}

bool DSerializeXML::serialize(std::ostream& output, DObject& dobject)
{
  return (this->serialize(output, dobject, 1));
}
 
bool DSerializeXML::serialize(std::ostream& output, DObject& dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf();
  XMLTag tag = XMLTag(output, dstruct->name(), "\n", depth++, true);

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subObject = dobject.getValue(x).get<DObject* >();

      if (subObject != NULL)
      {
        XMLTag xmlObj  = XMLTag(output, i->name(), "\n", depth, true);
        this->serialize(output, *subObject, depth + 1);
        subObject->destroy();
      }
    }
    else
      XMLTag(output, i->name(), dobject.getValue(x).asUnicodeString(), depth);
  }   
  
  return (true);
}

bool DSerializeXML::unserialize(std::ostream& input, DObject& dobject)
{
  return (false);
}

/* 
 *  Text serialization
 */

const std::string DSerializeText::name(void)
{
  return ("Text");
}

DSerializeText*   DSerializeText::create(void)
{
  return (new DSerializeText);
}

bool DSerializeText::serialize(std::ostream& output, DObject&  dobject)
{
  return (this->serialize(output, dobject, 0));
}
 
bool DSerializeText::serialize(std::ostream& output, DObject& dobject, int depth)
{
  int x = 0;
  DStruct const* dstruct = dobject.instanceOf(); 
  output << std::string(2*depth, ' ') << dstruct->name() << std::endl;
  output << std::string(2*depth++, ' ') <<  "{" << std::endl;

  for (DStruct::DAttributeIterator i = dstruct->attributeBegin(); i != dstruct->attributeEnd(); ++i, ++x)
  {
    if (i->type().getType() == DType::DObjectType)
    {
      DObject* subDObject = dobject.getValue(x).get<DObject*>();

      output << std::string(2*depth, ' ')  << i->name() << " : " <<  std::endl;
      if (subDObject != NULL)
      {
        this->serialize(output, *subDObject, depth + 1);
        subDObject->destroy();
      }
    }
    else
      output << std::string(2*depth, ' ')  << i->name() << " : " << dobject.getValue(x).asUnicodeString() << std::endl;
  }
 
  output << std::string(2*depth - 2, ' ') << "}" << std::endl; 
  return (true);
}

bool DSerializeText::unserialize(std::ostream& input, DObject& dobject)
{
  return (false);
}

/* 
 *  Serializers factory 
 */ 

std::vector<DSerialize* > DSerializers::__serializers;
static DSerializers dserializers = DSerializers();

DSerializers::DSerializers()
{
  this->registerSerializer(new DSerializeXML());
  this->registerSerializer(new DSerializeText());
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

DSerialize* DSerializers::to(const std::string type)
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
