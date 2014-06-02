#ifndef __RPCOBJECT__
#define __RPCOBJECT__

#include "ddynamicobject.hpp"

#include "protocol/dcppobject.hpp"
#include "protocol/dmutablestruct.hpp"

#include "networkstream.hpp"

using namespace Destruct;

//sert pas vraiment a grd chose en faite autant faire un DSerializeRaw(&struct, stream) register(struct, name)

class RPCStruct : public DStruct
{
//get On de deserialize struct 
// factorize new object too ? 
public:
  typedef DObject* (*CreateMutableObjectFunction)(RPCStruct*, DValue const&); //DStream ? 
  typedef std::vector<DAttribute> DAttributeContainer;
  typedef DAttributeContainer::const_iterator DAttributeIterator;

  template<typename Iterator> 
  RPCStruct(NetworkStream stream, const DUnicodeString& name, CreateMutableObjectFunction newObj, Iterator attributeBegin, Iterator attributeEnd) : DStruct(NULL, name, (DObject* (*) (DStruct*, DValue const&))newObj, attributeBegin, attributeEnd), __createObject(newObj)
  {
  }
  RPCStruct(NetworkStream stream, const DUnicodeString& name, CreateMutableObjectFunction objectFunction);
  DObject* newObject(DValue const& args);
private:
  const CreateMutableObjectFunction    __createObject;
};

//MakeNewRPC(stream, "StructName"); ...

class RPCObject : public DDynamicObject
{
public:
  RPCObject(NetworkStream stream, std::string const& name); //et on passe la struct plus simple 
  RPCObject(RPCStruct* dstruct, DValue const& args);
  RPCObject(RPCObject const & rhs);
  ~RPCObject();

  static DObject* newObject(RPCStruct* dstruct, DValue const& args);
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
  NetworkStream __stream;
//RawSerializer serializer;
};

#endif
