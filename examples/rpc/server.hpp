#ifndef __SERVER__
#define __SERVER__

#include <stdint.h>

#include "networkstream.hpp"
#include "dstruct.hpp"
#include "protocol/dcppobject.hpp"
#include "fsobject.hpp"

using namespace Destruct;

class Server
{
public:
                Server(); 
  int32_t       _receive(void* buff, int32_t lenflags);
  int32_t       _send(void* buff, int32_t size) const;
  void          serve(void);
  void          initFS(void);
  void          showFS(void);
  NetworkStream stream(void); //return NetworkStream

  void          getValue(NetworkStream stream, DObject* object);
  void          setValue(NetworkStream stream, DObject* object);
  void          call(NetworkStream stream, DObject* object);
  void          call0(NetworkStream stream, DObject* object);
  void          unknown(NetworkStream stream);
private:
  DObject*      root;
  int           __listenSocket;
  int           __connectionSocket;
  void          __bind(void);
  void          __listen(void);
  
};

#endif
