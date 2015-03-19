#include "recursivetraceobject.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"

namespace Destruct
{

/**
 *  RecursiveTraceObject
 */
RecursiveTraceObject::RecursiveTraceObject(DStruct* dstruct, DValue const& args) : TraceObject(dstruct, args)
{
}

RecursiveTraceObject::RecursiveTraceObject(TraceObject const& debugObject) : TraceObject(debugObject)
{
}

RecursiveTraceObject::RecursiveTraceObject(DObject* dobject) : TraceObject(dobject)
{
}

DObject* RecursiveTraceObject::newObject(DStruct* dstruct, DValue const& args)
{
  return (new RecursiveTraceObject(dstruct, args)); 
}


DValue    RecursiveTraceObject::getValue(size_t index) const
{
  DValue result = TraceObject::getValue(index);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(index).type().getType();

  if (attributeType == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveTraceObject(result.get<DObject*>())));
  else if (attributeType == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new RecursiveTraceFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

DValue    RecursiveTraceObject::getValue(DUnicodeString const& name) const
{
  DValue result = TraceObject::getValue(name);
  DType::Type_t  attributeType = this->object()->instanceOf()->attribute(name).type().getType();

  if (attributeType == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveTraceObject(result.get<DObject*>())));
  else if (attributeType == DType::DMethodType)
    return (RealValue<DFunctionObject*>(new RecursiveTraceFunctionObject(result.get<DFunctionObject*>())));

  return (result);
}

DValue    RecursiveTraceObject::call(size_t index, DValue const &args)
{
  DValue result = TraceObject::call(index, args);
  if (this->object()->instanceOf()->attribute(index).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveTraceObject(result.get<DObject*>())));

  return (result);
}

DValue    RecursiveTraceObject::call(DUnicodeString const& name, DValue const &args)
{
  DValue result = TraceObject::call(name, args);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveTraceObject(result.get<DObject*>())));

  return (result);
}

DValue    RecursiveTraceObject::call(DUnicodeString const& name)
{
  DValue result = TraceObject::call(name);
  if (this->object()->instanceOf()->attribute(name).type().getReturnType() == DType::DObjectType)
    return (RealValue<DObject*>(new RecursiveTraceObject(result.get<DObject*>())));

  return (result);
}

/**
 *  RecursiveTraceFunctionObject
 */
RecursiveTraceFunctionObject::RecursiveTraceFunctionObject(DFunctionObject* functionObject) : TraceFunctionObject(functionObject), __functionObject(NULL)
{
}

RecursiveTraceFunctionObject::~RecursiveTraceFunctionObject()
{
}

DValue    RecursiveTraceFunctionObject::call(void) const
{
  return (RealValue<DObject*>(new TraceObject(TraceFunctionObject::call().get<DObject*>())));
}

DValue    RecursiveTraceFunctionObject::call(DValue const& args) const
{
  return (RealValue<DObject*>(new TraceObject(TraceFunctionObject::call().get<DObject*>())));
}

}
