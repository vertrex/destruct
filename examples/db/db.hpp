#ifndef __RPC__
#define __RPC__

#include <stdint.h>

#include "destruct.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "session.hpp"


class DB
{
public:
                        DB();
                        ~DB();
  void                  declare(void);
  Destruct::DObject*    populateSession(void);
  void                  show(Destruct::DObject* dobject) const; 
  void                  load(Destruct::DValue const& filePath);
  Session*              session(void);

  void                  testRefcount(void);

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
  Destruct::Destruct&   __destruct;
  Session*              __session;
};

#endif
