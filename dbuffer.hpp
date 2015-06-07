#ifndef __DESTRUCT_DBUFFER__
#define __DESTRUCT_DBUFFER__

#include "destruct.hpp" //export
#include "dpolicy.hpp"
#include <stdint.h>
#include <string.h>

namespace Destruct
{

class RealBuffer : public RefcountPolicy<RealBuffer>
{
public:
  friend class RefcountPolicy<RealBuffer>;

  RealBuffer(); 
  RealBuffer(int32_t size_); 
  RealBuffer(uint8_t* data_, int32_t size_);
  RealBuffer(RealBuffer const& copy); 

  uint8_t*      data;
  uint32_t      size;
protected:
  virtual ~RealBuffer();
};

class EXPORT DBuffer //handler
{
public:
  DBuffer();//must not be used
  DBuffer(int32_t size);//allocate buff for reuse
  DBuffer(uint8_t* data , int32_t size);
  DBuffer(DBuffer const& copy);
  ~DBuffer();

  uint8_t*      data(void);
  int32_t       size(void);
private:
  RealBuffer*   __realBuffer;
};

}

#endif
