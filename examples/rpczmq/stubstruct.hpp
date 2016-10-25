#ifndef __STUBSTRUCT_HPP__
#define __STUBSTRUCT_HPP__

#include "destruct.hpp"
#include "dstruct.hpp"

using namespace Destruct;

class NetworkStream;

class StubStruct : public DStruct
{
public:
  typedef DObject* (*CreateClientObjectFunction)(StubStruct *, DValue const&, NetworkStream* networkStream);
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;

  EXPORT StubStruct(DStruct const* base, const DUnicodeString& name, CreateClientObjectFunction objectFunction, NetworkStream* networkStream);
  
  EXPORT ~StubStruct();

  EXPORT DObject* newObject(DValue const& args);
private:
  CreateClientObjectFunction  __createClientObject;
  NetworkStream*              __networkStream;
};

#endif
