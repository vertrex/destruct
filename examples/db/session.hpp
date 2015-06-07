#ifndef __SESSIONOBJECT__
#define __SESSIONOBJECT__

#include "dstructs.hpp"
#include "protocol/dcppobject.hpp"

/*
   //header : DFF0
   //version : 0x0001
   //ModuleHeader : "Modules"
   //all dstruct to save (should be in the program except generated at run time)
   //dobject & other to save 
   //TagHeader: "Tags"
   //tag&node list
   //DataType : "DataType"
   //Node & DataType list
   //BoomarkHeader:: "Bookmark"
   //ReportHeader :
   // report

les[0].moduleName.path()

*/
using namespace Destruct;

class Session : public DCppObject<Session>
{
public:
                                Session(Destruct::DStruct* dstruct, Destruct::DValue const& args);
  void                          save(Destruct::DValue const& filePath);
  RealValue<DObject*>           modules;
  RealValue<DFunctionObject* >  _save;
  /*
   *  DStruct declaration
   */
  static size_t ownAttributeCount()
  {
    return (2);
  }

  static DAttribute* ownAttributeBegin()
  {
    static DAttribute  attributes[] = 
    {
      DAttribute(DType::DNoneType, "save", DType::DUnicodeStringType),
      DAttribute(DType::DObjectType, "modules"),
      //bookmark
      //tags
      //report 
      //dataType ...
    };
    return (attributes);
  }

  static DPointer<Session>* memberBegin()
  {
    static DPointer<Session> memberPointer[] = 
    {
      DPointer<Session>(&Session::_save, &Session::save),
      DPointer<Session>(&Session::modules),
    };
    return (memberPointer);
  }

  static DAttribute* ownAttributeEnd()
  {
    return (ownAttributeBegin() + ownAttributeCount());
  }

  static DPointer<Session >*  memberEnd()
  {
    return (memberBegin() + ownAttributeCount());
  }
protected: 
  ~Session();
private:
  Destruct::DStructs&    __destruct;
};

#endif
