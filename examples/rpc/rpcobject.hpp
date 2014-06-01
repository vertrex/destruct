#ifndef __RPCOBJECT__
#define __RPCOBJECT__

#include "protocol/dcppobject.hpp"
#include "protocol/dmutableobject.hpp"

#include "networkstream.hpp"

using namespace Destruct;

class RPCObject : public DMutableObject
{
public:
  RPCObject(NetworkStream stream);
  ~RPCObject();

  //DObject* clone() const;
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
