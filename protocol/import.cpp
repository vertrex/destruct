#include "dstruct.hpp"
#include "import.hpp"

#include "dobject.hpp"
#include "dexception.hpp"

namespace Destruct
{

Import::Import(DStruct* dstruct, DValue const& args) : DCppObject<Import>(dstruct, args), __destruct(Destruct::DStructs::instance())
{
  this->init();
}

Import::Import(const Import& copy) : DCppObject<Import>(copy), __destruct(Destruct::DStructs::instance())
{
  this->init();
}

Import::~Import(void) //XXXX when is called ? and must call unload method rather ? because object can be destroyed lot of time !
                      //or object must be singleton if we keep this list !!!!!!!!!
{
}

DUInt8  Import::file(DValue const& args)
{
  DUnicodeString filePath = args.get<DUnicodeString>();
  std::cout << "Loading file : " << filePath << std::endl;

#ifndef WIN32
  void* library = dlopen(filePath.c_str(), RTLD_LAZY);
  dlerror();
  if (!library)
  {
    std::cout << "Can't load libray " << filePath << std::endl;
    return (false);
  }
  std::cout << "adding lib" << std::endl;
  this->__libraries.push_back(library);
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
  dlerror();
  if (!declare)
  {
    std::cout << "No method declare found in " << filePath << std::endl;
    //dlclose(library); if close remove from __libraries
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

DUInt8  Import::directory(DValue const& directoryPath)
{
  return (0);
}

void    Import::unload(void) //Must take the name or path of the lib to unload
{
#ifndef WIN32
  std::vector<void*>::iterator library = this->__libraries.begin();

  for (; library != this->__libraries.end(); ++library)
  {
    dlclose(*library);
  }
  //library->clear();
  //Must unregister registred DStruct ? 
#endif
}

void    Import::__registerDStructs(std::vector<Destruct::DStruct*>& dstructs)
{
  std::cout << "Registering module " << " structure:" << std::endl;

  std::vector<Destruct::DStruct*>::iterator dstruct = dstructs.begin();
  for (; dstruct != dstructs.end(); ++dstruct)
  {
    std::cout << "  [+] " << (*dstruct)->name() << std::endl;
    this->__destruct.registerDStruct(*dstruct);
  }
}

void    Import::__showDestruct() const
{
  int32_t count = (int32_t)this->__destruct.count();
  for (int32_t index = 0; index < count; ++index)
  {
    std::cout << this->__destruct.find(index)->name() << std::endl;
  }
}

}
