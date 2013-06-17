#ifndef DESTRUCT_TEST_HPP_
#define DESTRUCT_TEST_HPP_

#include <iostream>

#include "destruct.hpp"
#include "dmemberpointer.hpp"
#include "drealvalue.hpp"

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
 void      createPrefetchBaseClass(void);

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
private:        
 bool      __output;       
};


/*
 *   Prefetch use a baseclass who is derived on vista & xp this is perfect for an examples of DStruct power :)
 *   BaseClass mimic Standard class in dff modules
 */


class PrefetchBaseClass
{
public:
  typedef MemberPointer<PrefetchBaseClass, FinalValue> MemPtr; //famous template for auto handling of memberpointer that's rulez
  PrefetchBaseClass() 
  {
  };
  virtual        ~PrefetchBaseClass() 
  {
  };

  virtual DInt64   header() const = 0;
  virtual DInt32   headerSize() const = 0;
  virtual DInt32   fileSize() const = 0;
  virtual DInt32   firstFilePathBlock() const = 0 ;
  virtual DInt32   volumeInformationBlockOffset() const = 0;
  //virtual DObject* volumeInformationBlock; //Pointer 4 byte 
                
  static size_t ownAttributeCount()
  {
    return (5);
  }
 
  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = { 
                                        DAttribute("Header", DType::DInt64Type),
                                        DAttribute("Header size", DType::DInt32Type),
                                        DAttribute("File size", DType::DInt32Type),
                                        DAttribute("First file path block", DType::DInt32Type),
                                        DAttribute("Volume Information Block offset", DType::DInt32Type)
                                        //DAttribute("Volume Information Block", DType::DObjectType),
                                      };
    return (attributes);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

 static MemPtr*  memberBegin()
 {
   static MemPtr memberPointer[] = {
                                     MemPtr(&PrefetchBaseClass::__header),
                                     MemPtr(&PrefetchBaseClass::__headerSize),
                                     MemPtr(&PrefetchBaseClass::__fileSize),
                                     MemPtr(&PrefetchBaseClass::__firstFilePathBlock),
                                     MemPtr(&PrefetchBaseClass::__volumeInformationBlockOffset)
                                   };
   return (memberPointer);
 }  

 static MemPtr*  memberEnd()
 {
   return (memberBegin() + ownAttributeCount());
 } 
protected:
  PrefetchBaseClass(const PrefetchBaseClass& rhs) :  __header(rhs.__header), __headerSize(rhs.__headerSize), __fileSize(rhs.__fileSize), __firstFilePathBlock(rhs.__firstFilePathBlock), __volumeInformationBlockOffset(rhs.__volumeInformationBlockOffset)
  {
  }

  RealValue<DInt64>     __header;
  RealValue<DInt32>     __headerSize;
  RealValue<DInt32>     __fileSize;
  RealValue<DInt32>     __firstFilePathBlock;
  RealValue<DInt32>     __volumeInformationBlockOffset;
};

/*
 *  Class Volume Information Block
 */

class PrefetchXPClass : public PrefetchBaseClass
{
public:
  typedef MemberPointer<PrefetchXPClass, FinalValue> MemPtr;

  PrefetchXPClass(const DInt64 lastExecutionTime, const DInt32 numberOfExecution) : __lastExecutionTime(lastExecutionTime), __numberOfExecution(numberOfExecution) 
  {
  }

  PrefetchXPClass()
  {
  }

  DInt64 header() const
  {
    return (this->__header);
  }

  DInt32 headerSize() const
  {
    return (this->__headerSize);
  }

  DInt32 fileSize() const
  {
    return (this->__fileSize);
  }

  DInt32 firstFilePathBlock() const
  {
    return (this->__firstFilePathBlock);
  }

  DInt32 volumeInformationBlockOffset() const
  {
    return (this->__volumeInformationBlockOffset);
  }

  DInt64 lastExecutionTime() const
  {
    return (this->__lastExecutionTime);
  }

  void setLastExecutionTime(DInt64 value)
  {
    this->__lastExecutionTime = value;
  }

  static size_t ownAttributeCount(void)
  {
    return (2);
  }
 
  static DAttribute* ownAttributeBegin(void)
  {
    static DAttribute a[] = {
                              DAttribute("Last execution time", DType::DInt64Type),
                              DAttribute("Number of execution", DType::DInt32Type)
                            }; 
    return (a); 
  }

  static DAttribute* ownAttributeEnd()
  {
     return ownAttributeBegin() + ownAttributeCount();
  }

  static MemPtr* memberBegin()
  {
     static MemPtr memberPointer[] = {
                                       MemPtr(&PrefetchXPClass::__header),
                                       MemPtr(&PrefetchXPClass::__headerSize),
                                       MemPtr(&PrefetchXPClass::__fileSize),
                                       MemPtr(&PrefetchXPClass::__firstFilePathBlock),
                                       MemPtr(&PrefetchXPClass::__volumeInformationBlockOffset),
                                       MemPtr(&PrefetchXPClass::__lastExecutionTime),
                                       MemPtr(&PrefetchXPClass::__numberOfExecution)
                                     };
     return (memberPointer);  
  }

  static MemPtr* memberEnd()
  {
    return (memberBegin() + 7);
  }
private:
  RealValue<DInt64> __lastExecutionTime;
  RealValue<DInt32> __numberOfExecution;
};

}
#endif
