#ifndef __INHERIT__
#define __INHERIT__

#include <stdint.h>

#include "dstructs.hpp"
#include "protocol/dcppobject.hpp"

using namespace Destruct;

/*
NTFS.BPB
{
  uint16	bytesPerSector;
  uint8 	sectorsPerCluster;
  uint8	        reserved[7];
  uint8	        mediaDescriptor;
  uint8	        reserved2[18];
  uint64	totalSectors;
  uint64	MFTLogicalClusterNumber;
  uint64	MFTMirrorLogicalClusterNumber;
  int8  	clustersPerMFTRecord;
  uint8 	reserved3[3];
  int8  	clustersPerIndexRecord;  
  uint8 	reserved4[3];
  uint64	volumeSerialNumber;
  uint32	reserved5;
}		BPB;

NTFS.BootStrap
{
  uint8  bootstrap[426]
}

NTFS.BootSector
{
  uint8      jump[3]
  uint64     OEMID
  BPB        bpb
  BootStrap  bootStrap
  uint16     endOfSector
 
test: (inverser pour l instnat)
  endOfSector == 0xAA55
  bytesPerSector != 0 
  bytesPerSector() != 512)
  sectorsPerCluster != 0
  totalSectors != 0
  MFTLogicalClusterNumber <= totalSectors 
  MFTMirrorLogicalClusterNumber <= totalSectors
  clustersPerMFTRecord != 0
  clustersPerIndexRecord != 0
}

DObject*    //DVector<DUInt8> 
DUint64
DObject*    //BPB
DObject*    //BootStrap
DUInt16

DSLStruct : dstruct
{
  addStruct()
  addValidator(field, checkType)

  newObject();
}



DSLObject
{
  BootSectorStruct.addAttribute(jump, DObject)
  BootSectorStruct.addAttribute(OEMID, uint64)
  BootSectorStruct.addAttribute(bpb, dobject)
  BootSectorStruct.addAttribute(bootstrap, dobject)
  BootSectorStruct.addAttribute(endOfSector, uint16)

  BootSectorStruct.addValidator(endOfSector, "equal", 0xAA55);
  BootSectorStruct.addValidator(bytersPerSector, "not", 0)
  BootSectorStruct.addValidator(bytersPerSector, "not %", 512);
  BootSectoStruct.addValidator(totalSectors, "not", 0)
  BootSectorStruct.addValidator(MFTLogicalClusterNumber, "<", totalsector);
  BootSectorStruct.addValidator(MFTMirorLogicalClusterNumber, "<", totalsector);
  BootSectorStruct.addValidator(clusterPerMFTRecord, "!=", 0)
  BootSectoStruct.addValidator("cluserPerIndexRecord, "!=", 0)

  makeDSLObject("BootSectorStruct", dslstruct); 

  DSerializeRaw()
  {
    for each in dstruct.attribute
    {
      readAttribute
    }
    for each in validator
    {
      this->call(validator, attribute, value);
      or 
      this->call(validaotr, attribute, attribute)
    }
  }
}

*/

class DataType : public DCppObject<DataType>
{
public:
  DataType(DStruct* dstruct, DValue const& args);
  DValue testType(DValue const& stream);
      
  RealValue<DFunctionObject* >    _testType;

  static size_t ownAttributeCount()
  {
    return (1);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute attributes[] = 
    {
      DAttribute(DType::DObjectType, "testType", DType::DObjectType), 
    };

    return (attributes);
  }

  static DPointer<DataType>* memberBegin()
  {
    static DPointer<DataType> memberPointer[] =
    {
      DPointer<DataType>(&DataType::_testType, &DataType::testType),
    };

    return (memberPointer);
  } 

  static DAttribute*  ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<DataType>* memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
};

class FileType
{
public:
  FileType();
  ~FileType();

  static void declare(void);

  void showType(std::string const& filePath);
private:
  DStructs&           __dstructs;
  RealValue<DObject*> __dataType;
};

#endif
