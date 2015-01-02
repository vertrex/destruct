#include "compose.hpp"

/**
 * Compose
 */
Compose::Compose(DComposedStruct* composedStruct, DValue const& args) : DObject(composedStruct, args)
{
  Destruct::Destruct& destruct = Destruct::Destruct::instance();

  std::vector<DStruct const*> dstructs = composedStruct->inherit();
  std::vector<DStruct const*>::iterator dstruct = dstructs.begin();
  
  std::string msg = "<";
  for (; dstruct != dstructs.end(); ++dstruct)
  {
    if (dstruct != dstructs.begin())
      msg += ", ";
    msg += (*dstruct)->name();
    //XXX if composable object si non on devrait pas passer this ca peut s attendre a autre chose !

    DObject* object = ((DStruct*)*dstruct)->newObject(RealValue<DObject*>(this));
  
//if inherited 
//object->setValue("compose", RealValue<DObject*>(this));

    this->__objects.push_back(object);//set later ? 
    //if set later can get a list of object then to object->setValue("inheritance", this);
  }
  msg += ">";

  std::cout << "Compose" << msg << "()" << std::endl;
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
  return (new Compose((DComposedStruct*)dstruct, args));
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


