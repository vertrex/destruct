#include "recursivedebugobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

/**
 *  RecursiveDebugObject
 */
RecursiveDebugObject::RecursiveDebugObject(DStruct* dstruct, DValue const& args) : DebugObject(dstruct, args)
{
}

RecursiveDebugObject::RecursiveDebugObject(DebugObject const& debugObject) : DebugObject(debugObject)
{
}

RecursiveDebugObject::RecursiveDebugObject(DObject* dobject) : DebugObject(dobject)
{
}

DObject* RecursiveDebugObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new RecursiveDebugObject(dstruct, args)); 
}


DValue    RecursiveDebugObject::getValue(size_t index) const
{
  DValue result = DebugObject::getValue(index);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(index).type().getType();

  if (attributeType == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveDebugObject(result.get<DObject*>())));
  else if (attributeType == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new RecursiveDebugFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

DValue    RecursiveDebugObject::getValue(std::string const& name) const
{
  DValue result = DebugObject::getValue(name);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(name).type().getType();

  if (attributeType == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveDebugObject(result.get<DObject*>())));
  else if (attributeType == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new RecursiveDebugFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

DValue    RecursiveDebugObject::call(size_t index, DValue const &args)
{
  DValue result = DebugObject::call(index, args);
  if (this->object()->instanceOf()->attribute(index).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveDebugObject(result.get<DObject*>())));

  return (result);
}

DValue    RecursiveDebugObject::call(std::string const& name, DValue const &args)
{
  DValue result = DebugObject::call(name, args);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveDebugObject(result.get<DObject*>())));

  return (result);
}

DValue    RecursiveDebugObject::call(std::string const& name)
{
  DValue result = DebugObject::call(name);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveDebugObject(result.get<DObject*>())));

  return (result);
}

/**
 *  RecursiveDebugFunctionObject
 */
RecursiveDebugFunctionObject::RecursiveDebugFunctionObject(DFunctionObject* functionObject) : DebugFunctionObject(functionObject)
{
}

RecursiveDebugFunctionObject::~RecursiveDebugFunctionObject()
{
}

DValue    RecursiveDebugFunctionObject::call(void) const
{
  return (RealValue<DObject*>(new DebugObject(DebugFunctionObject::call().get<DObject*>())));
}

DValue    RecursiveDebugFunctionObject::call(DValue const& args) const
{
  return (RealValue<DObject*>(new DebugObject(DebugFunctionObject::call().get<DObject*>())));
}

}
