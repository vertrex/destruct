#include "dtype.hpp"
#include "dexception.hpp"

#include "datatype.hpp"

int     main(int argc, char** argv)
{
  try 
  {
    FileType fileType;

    fileType.showType("file.bin");

   //for each file
  // get stream
   // DataType()->getType(stream)  
      
  }
  catch (DException const& exception)
  {
    std::cout << "Error : " << std::endl << exception.error() << std::endl; 
  }
  catch (const std::string& error)
  {
     std::cout << "Error : " << std::endl << error << std::endl;
  }

  return (0);
}
