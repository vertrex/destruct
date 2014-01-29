#ifndef DESTRUCT_DEXCEPTION_HPP_
#define DESTRUCT_DEXCEPTION_HPP_

#include <iostream>

namespace Destruct
{
/* 
 *This is the dictionary of DStruct class it use a singleton pattern but should me modified to be use in a session object
 */

class DException
{
public:
  DException(const std::string errorMessage);
  const std::string error(void) const;
private:
  const std::string __errorMessage;
};

}

#endif
