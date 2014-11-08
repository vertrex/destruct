#ifndef __CLIENT_OBJECT__
#define __CLIENT_OBJECT__

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
  ClientObject(NetworkStream& stream, uint64_t id, DStruct* dstruct, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects);
  ClientObject(DStruct* dstruct, DValue const& args);
  ClientObject(ClientObject const & rhs);

  static DObject* newObject(DStruct* dstruct, DValue const& args);
  DObject* clone() const;

  DValue getValue(std::string const& name) const;
  void setValue(std::string const& name, DValue const &);
                                        
  DValue call(std::string const& name, DValue const &);
  DValue call(std::string const& name);

  DValue getValue(size_t index) const;
  void setValue(size_t idx, DValue const &);
  DValue call(size_t index, DValue const &);

  BaseValue* getBaseValue(size_t index);
  BaseValue const* getBaseValue(size_t index) const;

protected:
  ~ClientObject();
private:
  uint64_t       __id;
  NetworkStream  __networkStream;
  DSerialize*    __serializer;
  DObject*       __object;
};
}
#endif
