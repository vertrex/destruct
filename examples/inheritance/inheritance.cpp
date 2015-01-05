#include "inherit.hpp"

int     main(int argc, char** argv)
{
  Inheritance ih;
 
  try 
  {
    ih.test();
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
