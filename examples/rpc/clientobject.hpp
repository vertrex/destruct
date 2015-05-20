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
class EXPORT ClientObject : public DObject
{
public:
  ClientObject(DObject* stream, DObject* serializer, DObject* deserializer, uint64_t id, DStruct* dstruct);
  ClientObject(DStruct* dstruct, DValue const& args);
  ClientObject(ClientObject const & rhs);

  static DObject*       newObject(DStruct* dstruct, DValue const& args);
  DObject*              clone() const;

  DValue                getValue(DUnicodeString const& name) const;
  void                  setValue(DUnicodeString const& name, DValue const &);
                                        
  DValue                call(DUnicodeString const& name, DValue const &);
  DValue                call(DUnicodeString const& name);

  DValue                getValue(size_t index) const;
  void                  setValue(size_t idx, DValue const &);
  DValue                call(size_t index, DValue const &);

  BaseValue*            getBaseValue(size_t index);
  BaseValue const*      getBaseValue(size_t index) const;
protected:
  ~ClientObject();
private:
  uint64_t              __id;
  DObject*              __networkStream;
  DObject*              __serializer;
  DObject*              __deserializer;
};
}
#endif
