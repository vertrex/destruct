#ifndef __RPC_SERVER__
#define __RPC_SERVER__

#include "networkstream.hpp"
#include "objectmanager.hpp"
#include "protocol/dserialize.hpp"

using namespace Destruct;

class ServerFunctionObject;

class ServerObject
{
public:
                                ServerObject(Destruct::NetworkStream networkStream, ObjectManager<DObject*>& objectManager, ObjectManager<ServerFunctionObject*>& functionObjectManager);
  void                          findDStruct(void);
  void                          getValue(void);
  void                          setValue(void);
  void                          call(void);
  void                          call0(void);
  void                          functionCall(void);
  void                          functionCall0(void);
  void                          unknown(const std::string& cmd);
  Destruct::NetworkStream&      networkStream(void);
private:
  Destruct::NetworkStream       __networkStream;
  DSerialize*                   __serializer;
  ObjectManager<DObject*>&      __objectManager;
  ObjectManager<ServerFunctionObject*>& __functionObjectManager;
};
/*
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
*/
#endif
