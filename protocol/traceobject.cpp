#include "traceobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

TraceObject::TraceObject(DStruct* dstruct, DValue const& args) : DObject(args.get<DObject*>()->instanceOf(), args), __dobject(args)
{
  DUnicodeString const& name = __dobject->instanceOf()->name();
  std::cout << name <<  "::" << name << "()" << std::endl;
  this->__dobject->addRef();
}

TraceObject::TraceObject(TraceObject const& copy) : DObject(copy), __dobject(copy.__dobject)
{
}

TraceObject::TraceObject(DObject* object) : DObject(*object), __dobject(object)
{
}

TraceObject::~TraceObject()
{
  std::cout << this->instanceOf()->name() << "::~" << this->instanceOf()->name() << " count => " << this->__dobject->refCount() << std::endl;
  this->__dobject->destroy();
}

DObject*         TraceObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new TraceObject(dstruct, args)); 
}

DObject*         TraceObject::clone() const
{
  std::cout << this->__dobject->instanceOf()->name() << "->clone()" << std::endl;
  return (this->__dobject->clone());
}

DValue           TraceObject::getValue(size_t index) const
{
  DValue result = this->__dobject->getValue(index);
  DStruct* dstruct = this->__dobject->instanceOf();
  DAttribute attribute = dstruct->attribute(index);

  std::cout <<dstruct->name() << "::getValue(" << attribute.name() << ") => " << result.asUnicodeString() << std::endl;

  if (attribute.type().getType() == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new TraceFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

void             TraceObject::setValue(size_t index, DValue const& args)
{
  std::cout << this->__dobject->instanceOf()->name() << "::setValue(" << index << "," << args.asUnicodeString() << std::endl;

  this->__dobject->setValue(index, args);
}

DValue           TraceObject::call(size_t index, DValue const& args)
{
  DValue result = this->__dobject->call(index, args);

  std::cout << this->__dobject->instanceOf()->name() << "::call(" << index << "," << args.asUnicodeString() << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue           TraceObject::getValue(DUnicodeString const& name) const
{
  DValue result = this->__dobject->getValue(name);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

void             TraceObject::setValue(DUnicodeString const& name, DValue const& args)
{
  std::cout << this->__dobject->instanceOf()->name() << "::" << name << " => " << args.asUnicodeString() << std::endl;

  this->__dobject->setValue(name, args);
}


DValue           TraceObject::call(DUnicodeString const& name, DValue const& args)
{
  DValue result = this->__dobject->call(name, args);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << "(" << args.asUnicodeString() << ") => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue           TraceObject::call(DUnicodeString const& name)
{
  DValue result = this->__dobject->call(name);

  std::cout << this->__dobject->instanceOf()->name() << "::" << name << "()" << " => " << result.asUnicodeString() << std::endl;

  return (result);
}

BaseValue*       TraceObject::getBaseValue(size_t index) 
{
  return (NULL);
}

BaseValue const* TraceObject::getBaseValue(size_t index) const 
{
  return (NULL);
}

DObject*         TraceObject::object(void) const
{
  return (this->__dobject);
}

/**
 *  TraceFunctionObject
 */
TraceFunctionObject::TraceFunctionObject(DFunctionObject* functionObject) : __functionObject(functionObject)
{
  this->__functionObject->addRef();
}

TraceFunctionObject::~TraceFunctionObject()
{
//delete this->__functionObject; //XXX
  this->__functionObject->destroy();
}

DValue          TraceFunctionObject::call(void) const
{
  DValue result = this->__functionObject->call(); 
  std::cout << "DFunctionObject::call(void) => " << result.asUnicodeString() << std::endl;

  return (result);
}

DValue          TraceFunctionObject::call(DValue const& args) const
{
  DValue result = this->__functionObject->call(args); 
  std::cout << "DFunctionObject::call(" << args.asUnicodeString() << ") => " << result.asUnicodeString() << std::endl;

  return (result);
}

}
