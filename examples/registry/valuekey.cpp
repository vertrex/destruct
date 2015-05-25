#include "valuekey.hpp"

using namespace Destruct;

/**
 * ValueKey
 */
ValueKey::ValueKey(DStruct* dstruct, DValue const& args) : DCppObject<ValueKey>(dstruct, args), __size(0)
{
  this->init();
  this->name = Destruct::DStructs::instance().find("NameLength")->newObject(RealValue<DObject*>(this));
  ((DObject*)this->name)->setValue("attributeKeyName", RealValue<DUnicodeString>("nameLength"));
  //((DObject*)this->name)->addRef();
  
  this->data = Destruct::DStructs::instance().find("RegistryValueData")->newObject(RealValue<DObject*>(this));
  //((DObject*)this->data)->addRef();
}


ValueKey::~ValueKey(void)
{
}

DValue    ValueKey::deserializeRaw(DValue const& arg)
{
  //Destruct::DStructs& destruct = Destruct::DStructs::instance();

  //destruct.generate(this->dataTypeName[this->dataType]);
  return (RealValue<DObject*>(this));
}

DValue  ValueKey::valueTypeName(void)
{
  return (RealValue<DUnicodeString>(ValueKey::registryType(this->dataType)));
}

/**
 * RegistryValueData
 */
RegistryValueData::RegistryValueData(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValueData>(dstruct, args)
{

  this->init();
}

RegistryValueData::~RegistryValueData(void)
{
}


DValue    RegistryValueData::deserializeRaw(DValue const& arg)
{
  return (RealValue<DObject*>(this));
}
