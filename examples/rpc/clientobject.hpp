#ifndef __CLIENT_OBJECT__
#define __CLIENT_OBJECT__

#include "destruct.hpp"
#include "dvalue.hpp"
#include "networkstream.hpp"
#include "serverfunctionobject.hpp"
#include "objectmanager.hpp"

namespace Destruct {

class DSerialize;

/**
 * Client Object proxy 
 */
class ClientObject : public DObject
{
public:
  EXPORT ClientObject(NetworkStream& stream, DSerialize* serializer, uint64_t id, DStruct* dstruct);
  EXPORT ClientObject(DStruct* dstruct, DValue const& args);
  EXPORT ClientObject(ClientObject const & rhs);

  EXPORT static DObject*       newObject(DStruct* dstruct, DValue const& args);
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
protected:
  EXPORT ~ClientObject();
private:
  uint64_t              __id;
  NetworkStream&        __networkStream;
  DSerialize*           __serializer;
};
}
#endif
