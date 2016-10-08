#ifndef __CLIENT_OBJECT__
#define __CLIENT_OBJECT__

#include "destruct.hpp"
#include "dvalue.hpp"
#include "networkstream.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"

namespace Destruct {

class ClientStruct;

/**
 * Client Object proxy 
 */
class ClientObject : public DObject
{
public:
  EXPORT ClientObject(DValue const& stream, DValue const& serializer, DValue const& deserializer, uint64_t id, DStruct* dstruct);
  EXPORT ClientObject(DStruct* dstruct, DValue const& args);
  EXPORT ClientObject(ClientObject const & rhs);

  EXPORT static DObject*       newObject(ClientStruct* dstruct, DValue const& args, NetworkStream* networkStream);
  EXPORT DObject*              clone() const;

  EXPORT DValue                getValue(DUnicodeString const& name) const;
  EXPORT void                  setValue(DUnicodeString const& name, DValue const &);
                                        
  EXPORT DValue                call(DUnicodeString const& name, DValue const &);
  EXPORT DValue                call(DUnicodeString const& name);

  EXPORT DValue                getValue(size_t index) const;
  EXPORT void                  setValue(size_t idx, DValue const &);
  EXPORT DValue                call(size_t index, DValue const &);

  EXPORT BaseValue*            getBaseValue(size_t index);
  EXPORT BaseValue const*      getBaseValue(size_t index) const;

  RealValue<DObject*>           __networkStreamObject, __serializerObject, __deserializerObject;

  EXPORT uint64_t               id(void) const;
protected:
  ~ClientObject();
private:
  uint64_t                      __id;
  SerializeRPC*                 __serializer;
  DeserializeRPC*               __deserializer;
  NetworkStream*                __networkStream;
};

}
#endif
