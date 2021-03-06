#include "dwrapper.hpp"
#include "dvalue.hpp"
#include "drealvalue.hpp"
#include "dtype.hpp"
#include "dmethodobject.hpp"
#include "dsimpleobject.hpp"
#include "dstructs.hpp"

DWrapper::DWrapper()
{
  this->__object = Destruct::DNone;
  //this->__value = NULL;

  Destruct::DStruct*  module = new Destruct::DStruct(NULL, "DModule", Destruct::DSimpleObject::newObject);
  module->addAttribute(Destruct::DAttribute(Destruct::DType::DNoneType, "start", Destruct::DType::DObjectType));
  Destruct::DStructs::instance().registerDStruct(module);
}

DWrapper::~DWrapper()
{
}


Destruct::DObject* DWrapper::getObject(void)
{
  return (this->__object);
}

void DWrapper::setObject(Destruct::DObject* object)
{
  this->__object = object;
}

Destruct::DValue DWrapper::getValue(void)
{
 return this->__value;
}


void DWrapper::setValue(Destruct::DValue value)
{
   this->__value.replace(value);
}

void DWrapper::serialize(Destruct::DObject* object)
{
        //Destruct::DSerializers::to("XML")->serialize(Destruct::cout, *object);
        //Destruct::DSerializers::to("Text")->serialize(Destruct::cout, *object);
}


//XXX swig thread ! le gil est lacher par swig et repris en dpython
void DWrapper::run(Destruct::DObject* module, Destruct::DObject* arguments)
{
  module->call("start", Destruct::RealValue<Destruct::DObject*>(arguments));
}

void DWrapper::runValueObject(Destruct::DValue module, Destruct::DValue argument)
{
  Destruct::DObject* moduleDObject = module;
  Destruct::DObject* argumentDObject = argument;
 
  moduleDObject->call("start", Destruct::RealValue<Destruct::DObject*>(argumentDObject));
}

void DWrapper::runValueMethod(Destruct::DValue module, Destruct::DValue argument)
{
 Destruct::DFunctionObject* moduleDObject = module;
 Destruct::DObject* argumentDObject = argument;

 Destruct::DValue value = Destruct::RealValue<Destruct::DFunctionObject*>(moduleDObject);

 moduleDObject->call(Destruct::RealValue<Destruct::DObject*>(argumentDObject));
}


/* possiblite d'ecrire les ocnfig argument en c++ voir wrapper.py !
   manque la conversion vers variant 
   et les list et map 
   apres le reste peut etre wrapper les single, description etc...
*/ 
