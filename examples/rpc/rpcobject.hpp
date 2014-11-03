#ifndef __RPCOBJECT__
#define __RPCOBJECT__

#include "ddynamicobject.hpp"
#include "dvalue.hpp"

#include "protocol/dcppobject.hpp"
#include "protocol/dmutablestruct.hpp"

#include "networkstream.hpp"
#include "server.hpp"

namespace Destruct {

class DSerialize;

/**
 * Client Object proxy 
 */
class RPCObject : public DObject
{
public:
  RPCObject(NetworkStream& stream, uint64_t id, DStruct* dstruct, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects);
  RPCObject(DStruct* dstruct, DValue const& args);
  RPCObject(RPCObject const & rhs);

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
  ~RPCObject();
private:
  uint64_t       __id;
  NetworkStream  __networkStream;
  DSerialize*    __serializer;
  DObject*       __object;
};

/**
 * Client FunctionObject proxy
 */
class RPCFunctionObject : public DFunctionObject
{
public:
  RPCFunctionObject(NetworkStream& networkStream, uint64_t id, ObjectManager<DObject*>& objects, ObjectManager<ServerFunctionObject*>& functionObjects, DType::Type_t argumentType, DType::Type_t returnType);
  ~RPCFunctionObject();

  DValue call(void) const;
  DValue call(DValue const& args) const;
private:
  uint64_t       __id;
  NetworkStream&  __networkStream;
  DSerialize*    __serializer;
  DType::Type_t         __argumentType;
  DType::Type_t         __returnType;
};

}
#endif
