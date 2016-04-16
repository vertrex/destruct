#ifndef __REFCOUNT__
#define __REFCOUNT__

#include <stdint.h>

#include "dstructs.hpp"
#include "dstruct.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "drealvalue.hpp"
#include "protocol/dcppobject.hpp"


using namespace Destruct;

class DCppRefCount : public DCppObject<DCppRefCount>
{
public:
  DCppRefCount(DStruct* dstruct, DValue args) : DCppObject<DCppRefCount>(dstruct, args)
  {
    this->init();
  }

  DCppRefCount(DCppRefCount const& rhs) : DCppObject<DCppRefCount>(rhs)
  {
    this->copy(this, rhs);
  }

  RealValue<DFunctionObject*> _returnObject, _returnObjectAsDValue, _setDObject, _setDObjectEq;
  RealValue<DObject*>         valObject;

  void          setDObject(DValue const& args)
  {
    DObject* o = args.get<DObject*>();
    this->valObject.set(RealValue<DObject*>(o));
  }

  void          setDObjectEq(DValue const& args)
  {
    DObject* o = args.get<DObject*>();
    this->valObject = o;
  }

  DObject*      returnObject(void)
  {
    DObject* object = DStructs::instance().generate("DCppRefCount");
    return (object);
  }

  DValue        returnObjectAsDValue(void)
  {
    DObject* object = DStructs::instance().generate("DCppRefCount");
    return (RealValue<DObject*>(object));
  } 

  static size_t ownAttributeCount()
  {
    return (5);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DObjectType, "returnObject", DType::DNoneType),
      DAttribute(DType::DObjectType, "returnObjectAsDValue", DType::DNoneType),
      DAttribute(DType::DNoneType, "setDObject", DType::DObjectType),
      DAttribute(DType::DNoneType, "setDObjectEq", DType::DObjectType),
      DAttribute(DType::DObjectType, "valObject"),
    };
    return (attributes);
  }

  static DPointer<DCppRefCount>* memberBegin()
  {
    static DPointer<DCppRefCount> memberPointer[] = 
    {
      DPointer<DCppRefCount>(&DCppRefCount::_returnObject, &DCppRefCount::returnObject),
      DPointer<DCppRefCount>(&DCppRefCount::_returnObjectAsDValue, &DCppRefCount::returnObjectAsDValue),
      DPointer<DCppRefCount>(&DCppRefCount::_setDObject, &DCppRefCount::setDObject),
      DPointer<DCppRefCount>(&DCppRefCount::_setDObjectEq, &DCppRefCount::setDObjectEq),
      DPointer<DCppRefCount>(&DCppRefCount::valObject),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DCppRefCount >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
protected:
  ~DCppRefCount()
  {
  }
};

class Refcount 
{
public:
                        Refcount();
                        ~Refcount();
  void                  declare(void);
  void                  test(void);

  void                  show(DObject* object) const;
/*
  RealValue<DObject*>  testRV(void);
  DValue testDV(void);
  DObject* testObj(void);

*/
/*
  void testIRV(RV);
  void test(dv)
  void testobj();
*/


  RealValue<DObject*>   testRVRV(RealValue<DObject*> rv);
  DValue                testDVCRefDV(DValue const& object);
  DValue                testDVRefDV(DValue & object);
  DValue                testDVDV(DValue value);

  DObject*              testObjObj(DObject* object);
  RealValue<DObject*>   testRVObj(DObject* object);
  DObject*              testObjRV(RealValue<DObject*> object);

  void                  voidDVCRef(DValue const& object)
  {};
  DValue                testDUInt64(void) 
  {
    return (RealValue<DUInt64>(1));
  } 
 
private:
  Destruct::DStructs&   __destruct;
};

#endif
