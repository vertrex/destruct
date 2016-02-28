#ifndef DESTRUCT_TEST_HPP_
#define DESTRUCT_TEST_HPP_

#include <iostream>

#include "destruct.hpp"
#include "dstructs.hpp"
#include "dstruct.hpp"
#include "drealvalue.hpp"
#include "dmethodobject.hpp"

using namespace Destruct;


class DestructTest
{
public:
 EXPORT DestructTest(bool output = false);
 EXPORT ~DestructTest();

 EXPORT Destruct::DStructs*  structRegistry(void);
 EXPORT static std::vector<DStruct*> declare(void);
 
 EXPORT static void    createBaseClass(std::vector<DStruct*>&);
 EXPORT static void    createNtfsClass(std::vector<DStruct*>&);
 EXPORT static void    createNestedClass(std::vector<DStruct*>&);

 EXPORT void           createNtfsBootSector(void);
 EXPORT void           deserializeNtfsBootSector(void);
 
 EXPORT static void    createModifiableClass(std::vector<DStruct*>&);
 EXPORT static void    createFuncClass(std::vector<DStruct*>&);

 EXPORT void      createArchive(void);
 EXPORT void      readArchive(void);

 EXPORT void      run(void);
 EXPORT void      showAttribute(DStruct* def);
 EXPORT void      showObjectAttribute(DObject* object, int depth = 1);
 EXPORT void      setObjectValue(DObject* object);
 EXPORT DObject*  getObjectValue(void);

 EXPORT DValue    toto(DObject* self, DValue& args);

 EXPORT DObject*  createStringVector(void);
 EXPORT DObject*  createIntVector(void);
private:        
 bool      __output;       
};

/*
 *  Class Volume Information Block
 */
extern "C"
{
  EXPORT void DestructExport(void)
  {
    DestructTest::declare();
  }
}

#endif
