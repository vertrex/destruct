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
	Destruct::DObject* import = Destruct::DStructs::instance().find("Import")->newObject();
#ifndef WIN32
    //loader.loadFile(filePath);

    Destruct::RealValue<Destruct::DUnicodeString> filePath("../modules/libregistry.so");
    import->call("file", filePath);
    filePath = "../modules/libdthreading.so";
    import->call("file", filePath);
    filePath = "../modules/libdestruct_inherit.so";
    import->call("file", filePath);
    filePath = "../modules/libdestruct_rpc.so";
    import->call("file", filePath);
#else    
    Destruct::RealValue<Destruct::DUnicodeString> filePath("../modules/registry.dll");
    import->call("file", filePath);
    filePath = "../modules/dthreading.dll";
    import->call("file", filePath);
    filePath = "../modules/destruct_inherit.dll";
    import->call("file", filePath);
    filePath = "../modules/destruct_rpc.dll";
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
