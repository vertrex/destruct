#include "debugobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

DebugObject::DebugObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args.get<DObject*>())
{
  std::string const& name = __dobject->instanceOf()->name();
  std::cout << name <<  "::" << name << "()" << std::endl;
}

DebugObject::DebugObject(DebugObject const& copy) : DObject(copy), __dobject(copy.__dobject)
{
}

DebugObject::DebugObject(DObject* object) : DObject(*object), __dobject(object)
{
}

DebugObject::~DebugObject()
{
  std::cout << this->instanceOf()->name() << "::~" << this->instanceOf()->name() << " count => " << this->__dobject->refCount() << std::endl;
  this->__dobject->destroy();
}

DObject*         DebugObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new DebugObject(dstruct, args)); 
}

DObject*         DebugObject::clone() const
{
  std::cout << this->__dobject->instanceOf()->name() << "->clone()" << std::endl;
  return (this->__dobject->clone());
}

DValue           DebugObject::getValue(size_t index) const
{
  DValue result = this->__dobject->getValue(index);
  DStruct* dstruct = this->__dobject->instanceOf();
  DAttribute attribute = dstruct->attribute(index);

  std::cout <<dstruct->name() << "::getValue(" << attribute.name() << ") => " << result.asUnicodeString() << std::endl;

  if (attribute.type().getType() == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new DebugFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

void             DebugObject::setValue(size_t index, DValue const& args)
{
  std::cout << this->__dobject->instanceOf()->name() << "::setValue(" << index << "," << args.asUnicodeString() << std::endl;

  this->__dobject->setValue(index, args);
}

DValue           DebugObject::call(size_t index, DValue const& args)
{
  DValue result = this->__dobject->call(index, args);

  std::cout << this->__dobject->instanceOf()->name() << "::call(" << index << "," << args.asUnicodeString() << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue           DebugObject::getValue(std::string const& name) const
{
  DValue result = this->__dobject->getValue(name);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

void             DebugObject::setValue(std::string const& name, DValue const& args)
{
  std::cout << this->__dobject->instanceOf()->name() << "::" << name << " => " << args.asUnicodeString() << std::endl;

  this->__dobject->setValue(name, args);
}


DValue           DebugObject::call(std::string const& name, DValue const& args)
{
  DValue result = this->__dobject->call(name, args);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << "(" << args.asUnicodeString() << ") => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue           DebugObject::call(std::string const& name)
{
  DValue result = this->__dobject->call(name);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << "()" << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

BaseValue*       DebugObject::getBaseValue(size_t index) 
{
  return (NULL);
}

BaseValue const* DebugObject::getBaseValue(size_t index) const 
{
  return (NULL);
}

DObject*         DebugObject::object(void) const
{
  return (this->__dobject);
}

/**
 *  DebugFunctionObject
 */
DebugFunctionObject::DebugFunctionObject(DFunctionObject* functionObject) : __functionObject(functionObject)
{
}

DebugFunctionObject::~DebugFunctionObject()
{
  delete __functionObject;
}

DValue          DebugFunctionObject::call(void) const
{
  DValue result = this->__functionObject->call(); 
  std::cout << "DFunctionObject::call(void) => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue          DebugFunctionObject::call(DValue const& args) const
{
  DValue result = this->__functionObject->call(args); 
  std::cout << "DFunctionObject::call(" << args.asUnicodeString() << ") => " << result.asUnicodeString() << std::endl;

  return (result);
}

}
