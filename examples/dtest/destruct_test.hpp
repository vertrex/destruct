#ifndef DESTRUCT_TEST_HPP_
#define DESTRUCT_TEST_HPP_

#include <iostream>

#include "destruct.hpp"
#include "dstruct.hpp"
#include "drealvalue.hpp"
#include "dmethodobject.hpp"

using namespace Destruct;


class DestructTest
{
public:
 DestructTest(bool output = false);
 ~DestructTest();

 Destruct::Destruct*  structRegistry(void);
 static std::vector<DStruct*> declare(void);
 
 static void    createBaseClass(std::vector<DStruct*>&);
 static void    createNtfsClass(std::vector<DStruct*>&);
 static void    createNestedClass(std::vector<DStruct*>&);

 void           createNtfsBootSector(void);
 void           deserializeNtfsBootSector(void);
 
 static void    createModifiableClass(std::vector<DStruct*>&);
 static void    createFuncClass(std::vector<DStruct*>&);

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
extern "C"
{
  void declare(void)
  {
    DestructTest::declare();
  }
}

#endif
