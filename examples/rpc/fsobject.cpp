#include "dstructs.hpp"
#include "fsobject.hpp"
/*
 * Directory
 */

using namespace Destruct;

File::File(DStruct* dstruct, DValue const& args) : DCppObject<File>(dstruct, args)
{
  this->init();
  this->size = 0;
  this->name = "unknown";
}

File::~File()
{
}

DUnicodeString  File::path(DValue const& args)
{
  return DUnicodeString("/home/destruct/");
}

/*
 *  Directory
 */

Directory::Directory(DStruct* dstruct, DValue const& args) : DCppObject<Directory>(dstruct, args)
{
  this->init();
  this->name = "unknown-directory";
  DStruct* vectorStruct = Destruct::DStructs::instance().find("DVectorObject");
  this->children = vectorStruct->newObject();
}

DUnicodeString        Directory::path(DValue const& args)
{
  return DUnicodeString("/home/destruct/");
}
