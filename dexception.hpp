#ifndef DESTRUCT_DEXCEPTION_HPP_
#define DESTRUCT_DEXCEPTION_HPP_

#include "dunicodestring.hpp"

namespace Destruct
{
/* 
 *This is the dictionary of DStruct class it use a singleton pattern but should me modified to be use in a session object
 */

class DException
{
public:
  DException(const char* errorMessage);
  DException(const std::string errorMessage);
  DException(const DUnicodeString errorMessage);
  const DUnicodeString error(void) const;
  //private:
  const DUnicodeString __errorMessage;
};

}

#endif
