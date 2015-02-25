#ifndef DESTRUCT_DEXCEPTION_HPP_
#define DESTRUCT_DEXCEPTION_HPP_

#include "destruct.hpp"
#include "dunicodestring.hpp"

namespace Destruct
{
/* 
 *This is the dictionary of DStruct class it use a singleton pattern but should me modified to be use in a session object
 */

class DException
{
public:
  EXPORT DException(const char* errorMessage);
  EXPORT DException(const std::string errorMessage);
  EXPORT DException(const DUnicodeString errorMessage);
  EXPORT const DUnicodeString error(void) const;
  //private:
  const DUnicodeString __errorMessage;
};

}

#endif
