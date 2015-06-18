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
  //this->data = Destruct::DStructs::instance().find("RegistryValueData")->newObject(RealValue<DObject*>(this));
}


ValueKey::~ValueKey(void)
{
}

DObject*       ValueKey::deserializeRaw(DValue const& arg)
{
  //Destruct::DStructs& destruct = Destruct::DStructs::instance();
  //DObject* serializer = arg.get<DObject*>();

  //destruct.generate(this->dataTypeName[this->dataType]);
  //return (RealValue<DObject*>(this));
  return (this);
}

DUnicodeString  ValueKey::valueTypeName(void)
{
 return (ValueKey::registryType(this->dataType));
}

/**
 * RegistryValueData
 */
//RegistryNone::Registry
//{}

RegistryValueData::RegistryValueData(DStruct* dstruct, DValue const& args) : DCppObject<RegistryValueData>(dstruct, args)
{
  this->init();
  this->parent = args.get<DObject*>();
  //DUInt16 typeName = ((DObject*)this->parent)->getValue("dataType").get<DUInt16>();
  
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  //if (typeName == 1)
  //this->dataObject = destruct.find("RegistryString")->newObject(RealValue<DObject*>(this));
}

RegistryValueData::~RegistryValueData(void)
{
}

DObject*        RegistryValueData::data(void)
{
  return (this->dataObject); 
}
