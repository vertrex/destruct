#include "stubstruct.hpp"
#include "dvalue.hpp"
#include "networkstream.hpp"

StubStruct::StubStruct(DStruct const * base, const DUnicodeString & name, CreateClientObjectFunction objectFunction, NetworkStream* networkStream) : DStruct(base, name, NULL), __createClientObject(objectFunction), __networkStream(networkStream)
{
}

StubStruct::~StubStruct()
{
}

DObject* StubStruct::newObject(DValue const& args) //for mutable non const so ++definitionFIx=false ?
{
 if (*this->__createClientObject == NULL)
   return (NULL);
  
 return (*this->__createClientObject)(this, args, this->__networkStream);
}
