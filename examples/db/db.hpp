#ifndef __RPC__
#define __RPC__

#include <stdint.h>
#include "destruct.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"

//class DNode() : public DObject
//{
//  DUnicodeString __path;
//
//  Node* node();
//}

//class SaveFileSerializer// : public DBinarySerializer //public DSerializer
//{
  //SaveFileSerializer();
  //~SaveFileSerializer();
  //saveDStructs();
  //loadDStructs();
  //saveObjects();
  //loadObjects(); 
  ////checkSum();
  ////CompressStream();
  ////CypherStream(); /...
//};

/*
 
   SaveFile 

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

   so we can do a DObject / DStruct for that :) 

  class Session
  {
     DObject* modules;
     DObject* tags;
     DObject* dataType;
     DObject* bookmark;
     DObject* report;
  }
  then for modules in session : loadModule(...)
  then for tags in session : node.setTags() ...

  //session.modules[0].moduleName.path()

*/
class DB
{
public:
                        DB();
                        ~DB();
  void                  declare(void);
  Destruct::DObject*    fakeDB(void);
//  void                  populateSession(Destruct::DObject* dobject) const;
  void                  show(Destruct::DObject* dobject) const; 
  void                  load(Destruct::DUnicodeString const& filePath) const;
  void                  save(Destruct::DUnicodeString const& filePath);
private:
  Destruct::Destruct&   __destruct;
  //RealValue<DObject*> __session;
};

#endif
