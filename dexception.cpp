#include "dexception.hpp"

namespace Destruct
{

DException::DException(std::string errorMessage) : __errorMessage(errorMessage)
{
}

const std::string DException::error(void)
{
  return (this->__errorMessage);
}

}
