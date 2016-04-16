#include "serverfunctionobject.hpp"

/**
 * ServerFunctionObject
 */
ServerFunctionObject::ServerFunctionObject(DStruct* dstruct, DValue const& args) : DCppObject<ServerFunctionObject>(dstruct, args)
{
}

ServerFunctionObject::ServerFunctionObject(ServerFunctionObject const& rhs) : DCppObject<ServerFunctionObject>(rhs)
{
  this->copy(this, rhs);
}

ServerFunctionObject::~ServerFunctionObject()
{
}
