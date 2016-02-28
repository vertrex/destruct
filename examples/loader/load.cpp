#include "dstructs.hpp"
#include "dtype.hpp"
#include "dexception.hpp"
#include "dobject.hpp"
#include "drealvalue.hpp"
#include "load.hpp"

int main(int argc, char **argv)
{
  std::cout << "loading directory " << "destruct/examples" << std::endl;

  Destruct::DType::init();

  try
  {
    //XXX leaking path name :) must recurse a dir in argv
#ifndef WIN32
    //loader.loadFile(filePath);
    Destruct::DObject* import = Destruct::DStructs::instance().find("Import")->newObject();

    Destruct::RealValue<Destruct::DUnicodeString> filePath("../modules/libregistry.so");
    import->call("file", filePath);
    filePath = "../modules/libdthreading.so";
    import->call("file", filePath);
    filePath = "../modules/libdestruct_inherit.so";
    import->call("file", filePath);
    filePath = "../modules/libdestruct_rpc.so";
    import->call("file", filePath);
#else
    //XXX load other lib 
    std::string filePath = "lib\\dthreading.dll";
    import->call("file", filePath);
#endif

    size_t count = Destruct::DStructs::instance().count();
    std::cout << "Destruct database contain " << count << " structures" << std::endl;
    for (size_t index = 0; index < count; ++index)
      std::cout << Destruct::DStructs::instance().find(index)->name() << std::endl;

    import->destroy();
  }
  catch (Destruct::DException const& exception)
  {
    std::cout << exception.error() << std::endl;
  }

  Destruct::DType::clean();
}
