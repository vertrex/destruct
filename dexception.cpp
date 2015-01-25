#include "dexception.hpp"

namespace Destruct
{

DException::DException(const char* errorMessage) : __errorMessage(errorMessage)
{
}

DException::DException(std::string errorMessage) : __errorMessage(errorMessage)
{
}

DException::DException(DUnicodeString errorMessage) : __errorMessage(errorMessage)
{
}

const DUnicodeString DException::error(void) const
{
  return (this->__errorMessage);
}

}
