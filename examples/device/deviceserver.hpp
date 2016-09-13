#ifndef __deviceserver__
#define __deviceserver__

#include <stdint.h>
#include <map>

#include "server.hpp"


class DeviceServer : public Server
{
public:
  EXPORT                DeviceServer(int32_t port);
  EXPORT virtual void   initRoot(void);
};

#endif
