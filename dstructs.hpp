#ifndef DESTRUCT_DESTRUCT_HPP_
#define DESTRUCT_DESTRUCT_HPP_

#include "destruct.hpp"
#include "dstruct.hpp"

namespace Destruct
{
/* 
 *This is the dictionary of DStruct class it use a singleton pattern but should me modified to be use in a session object
 */
class DStruct;
class NameSpace;

class DStructs
{
public:
  EXPORT static DStructs& instance(void);

  EXPORT size_t        count(void);
  EXPORT DStruct*      find(DUnicodeString const & name);
  EXPORT DStruct*      find(const size_t name);  
  EXPORT void          registerDStruct(DStruct* dstruct);
  EXPORT void          registerDStruct(DUnicodeString const& name, DStruct* dstruct);
  EXPORT bool          unregister(DStruct* dstruct);
  EXPORT DObject*      generate(DUnicodeString const& name);
  EXPORT DObject*      generate(DUnicodeString const& name, DValue const& args);
                       ~DStructs();
private:
                       DStructs();
                       DStructs(const DStructs&);
  DStructs&            operator=(DStructs&);
  NameSpace*           __nameSpace;
};

class NameSpace
{
public:
  NameSpace(DUnicodeString const& name);
  ~NameSpace(void);
 
  const DUnicodeString  name(void) const;
  size_t                count(void) const;

  /**
   *  return __function
   */
  DStruct*              dstruct(const size_t index);
  DStruct*              dstruct(DUnicodeString const& name);
  NameSpace*            nameSpace(DUnicodeString const& name) const;

  /**
   * recursive function
   */
  NameSpace*            create(DUnicodeString const& nameSpaces);

  DStruct*              findDStruct(size_t index); //return __structures[index] recursively
  DStruct*              findDStruct(DUnicodeString const& name);//return __structures[name] recurrsively

  void                  addStructure(DStruct* dstruct);
private:
  const DUnicodeString      __name;
  std::vector<DStruct*>     __structures;
  std::vector<NameSpace*>   __nameSpaces;  
};

}

#endif
