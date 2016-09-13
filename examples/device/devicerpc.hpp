#ifndef __devicerpc_hpp__
#define __devicerpc_hpp__

#include "device_common.hpp"

class DeviceRPC
{
public:
 DeviceRPC();
 ~DeviceRPC();

 void                     serve(uint32_t port);
 static const std::string usage(void);
 static void              show(Destruct::DObject* object);
};


#endif
