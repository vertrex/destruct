#ifndef __STUBOBJECT_HPP__
#define __STUBOBJECT_HPP__

#include "destruct.hpp"
#include "dvalue.hpp"

#include "networkstream.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"

using namespace Destruct;

class StubStruct;

/**
 * Client Object proxy 
 */
class StubObject : public DObject
{
public:
  EXPORT StubObject(DValue const& stream, DValue const& serializer, DValue const& deserializer, uint64_t id, DStruct* dstruct);
  EXPORT StubObject(DStruct* dstruct, DValue const& args);
  EXPORT StubObject(StubObject const & rhs);

  EXPORT static DObject*       newObject(StubStruct* dstruct, DValue const& args, NetworkStream* networkStream);
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
  ~StubObject();
private:
  uint64_t                      __id;
  SerializeRPC*                 __serializer;
  DeserializeRPC*               __deserializer;
  NetworkStream*                __networkStream;
};

#endif
