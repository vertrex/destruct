#ifndef __REFCOUNT__
#define __REFCOUNT__

#include <stdint.h>

#include "dstructs.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "drealvalue.hpp"

using namespace Destruct;


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

  DValue                testDUInt64(void) 
  {
    return (RealValue<DUInt64>(1));
  } 
 
private:
  Destruct::DStructs&   __destruct;
};

#endif
