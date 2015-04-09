#ifndef __DESTRUCT_DBUFFER__
#define __DESTRUCT_DBUFFER__

#include "destruct.hpp" //export
#include <stdint.h>

class EXPORT DBuffer
{
public:
  DBuffer();
  DBuffer(uint8_t* data , int32_t size); 
  DBuffer(DBuffer const& copy);
  ~DBuffer();

  uint8_t*      data(void);
  int32_t       size(void);
private:
  uint8_t*      __data;
  int32_t       __size;
};

#endif
