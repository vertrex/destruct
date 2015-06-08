#include "serverfunctionobject.hpp"

/**
 * ServerFunctionObject
 */
ServerFunctionObject::ServerFunctionObject(DStruct* dstruct, DValue const& args) : DCppObject<ServerFunctionObject>(dstruct, args)
{
}

ServerFunctionObject::ServerFunctionObject(ServerFunctionObject const& copy) : DCppObject<ServerFunctionObject>(copy)
{
}

ServerFunctionObject::~ServerFunctionObject()
{
}
