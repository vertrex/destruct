#include "dexception.hpp"

namespace Destruct
{

DException::DException(std::string errorMessage) : __errorMessage(errorMessage)
{
}

const std::string DException::error(void) const
{
  return (this->__errorMessage);
}

}
