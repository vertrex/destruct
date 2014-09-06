#ifndef __SERVER__
#define __SERVER__

#include <stdint.h>
#include <map>

#include "networkstream.hpp"
#include "ddynamicobject.hpp"
#include "protocol/dcppobject.hpp"
#include "fsobject.hpp"
#include "protocol/dserialize.hpp"
#include "rpcserver.hpp"

class Server
{
public:
                        Server(); 
  void                  initFS(void);
  void                  serve(void);
  void                  showRoot(void);
private:
  ObjectManager         __objectManager;
  int                   __listenSocket;
  int32_t               __connectionSocket;
  void                  __bind(void);
  void                  __listen(void);
};

#endif
