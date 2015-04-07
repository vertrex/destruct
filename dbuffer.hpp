#ifndef __DESTRUCT_DBUFFER__
#define __DESTRUCT_DBUFFER__

#include <stdint.h>

class DBuffer
{
public:
  DBuffer();
  DBuffer(uint8_t* data , int32_t size); 
  DBuffer(DBuffer const& copy);

  uint8_t*      data(void);
  int32_t       size(void);
private:
  uint8_t*      __data;
  int32_t       __size;
};

#endif
