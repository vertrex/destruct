#ifndef DESTRUCT_TEST_HPP_
#define DESTRUCT_TEST_HPP_

#include <iostream>

#include "destruct.hpp"
#include "dmemberpointer.hpp"
#include "drealvalue.hpp"
#include "dcppobject.hpp"
#include "dmethodobject.hpp"

namespace Destruct
{

class DestructTest
{
public:
            DestructTest(bool output = false);
 Destruct*  structRegistry(void);

 void      createBaseClass(void);
 void      createNtfsClass(void);
 void      createNestedClass(void);

 void      createNtfsBootSector(void);
 void      deserializeNtfsBootSector(void);
 
 void      createModifiableClass(void);
 void      createFuncClass(void);

 void      createArchive(void);
 void      readArchive(void);

 void      run(void);
 void      showAttribute(DStruct* def);
 void      showObjectAttribute(DObject* object, int depth = 1);
 void      setObjectValue(DObject* object);
 DObject*  getObjectValue(void);

 DValue    toto(DObject* self, DValue& args);

 DObject*  createStringVector(void);
 DObject*  createIntVector(void);

private:        
 bool      __output;       
};

/*
 *  Class Volume Information Block
 */



}
#endif
