#include "serverfunctionobject.hpp"

/**
 * ServerFunctionObject
 */
ServerFunctionObject::ServerFunctionObject(DFunctionObject* functionObject, DType::Type_t argumentType,
 DType::Type_t returnType) : __functionObject(functionObject), __argumentType(argumentType), __returnType(returnType)
{

}

DType::Type_t ServerFunctionObject::argumentType(void) const
{
  return (this->__argumentType);
}

DType::Type_t ServerFunctionObject::returnType(void) const
{
  return (this->__returnType);
}

DFunctionObject* ServerFunctionObject::functionObject(void) const
{
  return (this->__functionObject);
}
