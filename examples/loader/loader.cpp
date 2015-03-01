

#include "dstruct.hpp"
#include "loader.hpp"

#include "dobject.hpp"
#include "dexception.hpp"

Loader::Loader(void) : __destruct(Destruct::DStructs::instance())
{
}

bool    Loader::loadFile(const std::string& filePath)
{
  std::cout << "Loading file : " << filePath << std::endl;

  //found x struct :
#ifndef WIN32
  void* library = dlopen(filePath.c_str(), RTLD_LAZY);
  if (!library)
  {
    std::cout << "Can't load libray " << filePath << std::endl;
    return (false);
  }
#else
  HMODULE library = LoadLibrary(filePath.c_str());
  if (library == NULL) 
  {
      std::cout << "Can't load libray " << filePath << std::endl;
    return (false);
  }
#endif

  std::cout << "Loading symbol from libray " << filePath << std::endl;
#ifndef WIN32
  void* declare = dlsym(library, "declare");//must better return a list of struct that we register so we now about them and can unload them if the module is unloaded and closed
  if (!declare)
  {
    std::cout << "No method declare found in " << filePath << std::endl;
    return (false);
  }
#else
  FARPROC declare = GetProcAddress(library, "declare");
  if (declare == NULL) 
  {
    std::cout << "No method declare found in " << filePath << std::endl;
    return (false);
  }
#endif
  
  //typedef std::vector<Destruct::DStruct*> (*declareFunc)(void);
  typedef void (*declareFunc)(void);

  std::cout << "call decalre" << std::endl;
  declareFunc func = (declareFunc)declare;

 // std::vector<Destruct::DStruct*> dstructs = (*func)();
  (*func)();

  //this->registerDStructs(dstructs);

  std::cout << "showing struct" << std::endl;
  this->__showDestruct();

//return list of dstruct to delete before closing library // but if a dobject is created it will crash if not refcount
//  dlclose(library);
//FreeLibrary(library); 
  return (true);
}

void    Loader::registerDStructs(std::vector<Destruct::DStruct*>& dstructs)
{
  std::cout << "Registering module " << " structure:" << std::endl;

  std::vector<Destruct::DStruct*>::iterator dstruct = dstructs.begin();
  for (; dstruct != dstructs.end(); ++dstruct)
  {
    std::cout << "  [+] " << (*dstruct)->name() << std::endl;
    this->__destruct.registerDStruct(*dstruct);
  }
}

void    Loader::__showDestruct() const
{
  int32_t count = (int32_t)this->__destruct.count();
  for (int32_t index = 0; index < count; ++index)
  {
    std::cout << this->__destruct.find(index)->name() << std::endl;
  }
}

bool    Loader::loadDirectory(const std::string& directoryPath)
{

  return (false);
}
