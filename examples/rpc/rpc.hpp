#ifndef __RPC__
#define __RPC__

#include <stdint.h>

class RPC
{
public:
        RPC();
        ~RPC();
  void  serve(void);
  void  connect(std::string const& addr, uint32_t port);
};

#endif
