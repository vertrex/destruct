#ifndef DESTRUCT_DSTREAMBASE_HPP
#define DESTRUCT_DSTREAMBASE_HPP


class DStreamBase
{
public:
  DStreamBase() {};
  DStreamBase(const DStreamBase& copy) {};
  virtual ~DStreamBase() {};
  virtual DStreamBase& operator>>(std::string& val) = 0; 
  virtual DStreamBase& operator<<(std::string val) = 0;
  virtual DStreamBase& operator<<(char val) = 0;
  //virtual DStreamBase& operator<<(StandardEndLine func) = 0;
  virtual DStreamBase& read(char*  buff, uint32_t size) = 0;
  virtual DStreamBase& write(const char* buff, uint32_t size) = 0;
  virtual bool fail(void) = 0;
};



#endif
