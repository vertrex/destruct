#include "compose.hpp"

/**
 * Compose
 */
Compose::Compose(DStruct* dstruct, DValue const& args) : DObject(dstruct, args)
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();
  //std::string msg = "<";

  DObject* structsName = args.get<DObject*>();
  DUInt64  count = structsName->call("size").get<DUInt64>();

  for (DUInt64 i = 0; i < count; ++i)
  {
    DUnicodeString structName = structsName->call("get", RealValue<DUInt64>(i)).get<DUnicodeString>();

    DObject* object = destruct.generate(structName, RealValue<DObject*>(this));
    this->__objects.push_back(object);

    //msg += structName;
    //msg += ",";
  }
  //msg += ">";

  //std::cout << "Compose " << msg << std::endl;
}

//Compose::Compose(Compose const& copy) : DObject(copy), __dobject(copy.__dobject)
//{
//}

//Compose::Compose(DObject* object) : DObject(*object), __dobject(object)
//{
//}

Compose::~Compose()
{
  std::cout << "Compose::~Compose()" << std::endl;

  std::vector<DObject*>::iterator it = this->__objects.begin();
  for (; it != this->__objects.end(); ++it)
  {
     (*it)->destroy();
  }
}

DObject*         Compose::newObject(DStruct* dstruct, DValue const& args)
{
  return (new Compose(dstruct, args));
}

DObject*         Compose::clone() const
{
  throw DException("Compose::clone() not implemented");
}

DValue           Compose::getValue(size_t index) const
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->getValue(attribute.name()));
}

void             Compose::setValue(size_t index, DValue const& args)
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->setValue(attribute.name(), args));
}

DValue           Compose::call(size_t index, DValue const& args)
{
  DAttribute attribute = this->instanceOf()->attribute(index);
  return (this->call(attribute.name(), args));
}

DValue           Compose::getValue(std::string const& name) const
{
  std::vector<DObject*>::const_iterator object = this->__objects.begin();
  for (; object != this->__objects.end(); ++object)
  {
    DStruct* dstruct = (*object)->instanceOf();
    int32_t  attribute = dstruct->findAttribute(name);
    if (attribute != -1)
      return ((*object)->getValue(name)); 
  }

  std::string error = "getValue " + name + " not found";
  throw DException(error);
}

void             Compose::setValue(std::string const& name, DValue const& args)
{
  std::vector<DObject*>::const_iterator object = this->__objects.begin();
  for (; object != this->__objects.end(); ++object)
  {
    DStruct* dstruct = (*object)->instanceOf();
    int32_t  attribute = dstruct->findAttribute(name);
    if (attribute != -1)
      return ((*object)->setValue(name, args)); 
  }

  std::string error = "setValue(" + name + ", " + args.asUnicodeString() + ") not found";
  throw DException(error);
}

DValue           Compose::call(std::string const& name, DValue const& args)
{
  std::vector<DObject*>::const_iterator object = this->__objects.begin();
  for (; object != this->__objects.end(); ++object)
  {
    DStruct* dstruct = (*object)->instanceOf();
    int32_t  attribute = dstruct->findAttribute(name);
    if (attribute != -1)
      return ((*object)->call(name), args); 
  }

  std::string error = "getValue " + name + " not found";
  throw DException(error);
}

DValue           Compose::call(std::string const& name)
{
  std::vector<DObject*>::const_iterator object = this->__objects.begin();
  for (; object != this->__objects.end(); ++object)
  {
    DStruct* dstruct = (*object)->instanceOf();
    int32_t  attribute = dstruct->findAttribute(name);
    if (attribute != -1)
      return ((*object)->call(name)); 
  }

  std::string error = "call " + name + " not found";
  throw DException(error);
}

BaseValue*       Compose::getBaseValue(size_t index) 
{
  return (NULL);
}

BaseValue const* Compose::getBaseValue(size_t index) const 
{
  return (NULL);
}


