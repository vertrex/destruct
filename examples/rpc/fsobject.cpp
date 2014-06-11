#include "fsobject.hpp"
#include "destruct.hpp"

File::File(DStruct* dstruct, Destruct::DValue const& args) : DCppObject<File>(dstruct, args)
{
  this->init();
  this->size = 0;
  this->name = "unknown";
}

DUnicodeString        File::path(Destruct::DValue const& args)
{
  return DUnicodeString("/home/destruct/");
}

Directory::Directory(DStruct* dstruct, Destruct::DValue const& args) : DCppObject<Directory>(dstruct, args)
{
  this->init();
  this->name = "unknown-directory";
  DStruct* vectorStruct = Destruct::Destruct::instance().find("DVectorObject");
  this->children = vectorStruct->newObject();
}

DUnicodeString        Directory::path(Destruct::DValue const& args)
{
  return DUnicodeString("/home/destruct/");
}
