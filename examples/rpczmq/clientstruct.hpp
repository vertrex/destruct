#ifndef DESTRUCT_CLIENT_HPP_
#define DESTRUCT_CLIENTSTRUCT_HPP_

#include "destruct.hpp"
#include "dstruct.hpp"

namespace Destruct
{

class NetworkStream;

class ClientStruct : public DStruct
{
public:
  typedef DObject* (*CreateClientObjectFunction)(ClientStruct *, DValue const&, NetworkStream* networkStream);
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;

  EXPORT ClientStruct(DStruct const* base, const DUnicodeString& name, CreateClientObjectFunction objectFunction, NetworkStream* networkStream);
  
  EXPORT ~ClientStruct();

  EXPORT DObject* newObject(DValue const& args);
private:
  CreateClientObjectFunction  __createClientObject;
  NetworkStream*              __networkStream;
};

}

#endif
