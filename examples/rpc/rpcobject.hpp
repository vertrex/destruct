#ifndef __RPCOBJECT__
#define __RPCOBJECT__

#include "ddynamicobject.hpp"
#include "dvalue.hpp"

#include "protocol/dcppobject.hpp"
#include "protocol/dmutablestruct.hpp"

#include "networkstream.hpp"

//using namespace Destruct;

//sert pas vraiment a grd chose en faite autant faire un DSerializeRaw(&struct, stream) register(struct, name)

namespace Destruct {

class DSerialize;

class RPCObject : public DDynamicObject
{
public:
  RPCObject(NetworkStream stream, uint64_t id, DStruct* dstruct); //et on passe la struct plus simple 
  RPCObject(DStruct* dstruct, DValue const& args);
  RPCObject(RPCObject const & rhs);
  ~RPCObject();

  static DObject* newObject(DStruct* dstruct, DValue const& args);
  DObject* clone() const;
  //DValue getValue(size_t index) const;               //set data member
  //void setValue(size_t idx, DValue const &);         //set 
  //DValue call(size_t index, DValue const &);

  DValue getValue(std::string const& name) const;        //set data member
  void setValue(std::string const& name, DValue const &);
                                        
  DValue call(std::string const& name, DValue const &);
  DValue call(std::string const& name); 
  void wait(void);

private:
  uint64_t      __id;
  NetworkStream __stream;
  DSerialize*   __serializer;
  //DRPCSerializer __serializer;
};
}
#endif
