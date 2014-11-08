#ifndef __SERVER_FUNCTION_OBJECT_HPP__
#define __SERVER_FUNCTION_OBJECT_HPP__

#include "dobject.hpp"
#include "dfunction.hpp"

using namespace Destruct;

class ServerFunctionObject
{
public:
  ServerFunctionObject(DFunctionObject* functionObject, DType::Type_t argumentType, DType::Type_t returnType);
  DType::Type_t argumentType(void) const;
  DType::Type_t returnType(void) const;
  DFunctionObject* functionObject(void) const;
private:
  DFunctionObject* __functionObject;
  DType::Type_t __argumentType;
  DType::Type_t __returnType;
};

#endif
