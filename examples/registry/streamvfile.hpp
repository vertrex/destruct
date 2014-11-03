#ifndef __streamvfile_hpp__
#define __streamvfile_hpp__

#include "registry_common.hpp"
#include "vfile.hpp"

using namespace Destruct;

class StreamFile : public DStream
{
public:
                    StreamFile(DStruct* dstruct, DValue const &args);
                    StreamFile(const StreamFile& copy);
                    ~StreamFile();
  DStream&          read(char*  buff, uint32_t size);
  DStream&          write(const char* buff, uint32_t size);
  DStream&          seek(int64_t pos);
  int64_t           tell(void);
private:
  int               __fd;
};

#endif
