#ifndef DESTRUCT_DESTRUCT_HPP_
#define DESTRUCT_DESTRUCT_HPP_

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
  typedef std::vector<DStruct* >  Container;
  typedef Container::iterator     Iterator;

  static DStructs& instance(void);

  size_t        count(void);
  DStruct*      find(DUnicodeString const & name);
  DStruct*      find(const size_t name);  
  void          registerDStruct(DStruct* dstruct);
  void          registerDStruct(DUnicodeString const& name, DStruct* dstruct);
  bool          unregister(DStruct* dstruct);
  DObject*      generate(DUnicodeString const& name);
  DObject*      generate(DUnicodeString const& name, DValue const& args);
private:
  DStructs();
  ~DStructs();
  DStructs(const DStructs&);
  DStructs&                 operator=(DStructs&);

  NameSpace*                 __nameSpace; //NameSpace("")
};

class NameSpace
{
public:
  NameSpace(DUnicodeString const& name);
  ~NameSpace(void);
 
 
  const DUnicodeString  name(void) const;
  size_t             count(void) const;

  /**
   *  return __function
   */
  DStruct*           dstruct(const size_t index);
  DStruct*           dstruct(DUnicodeString const& name);
  NameSpace*         nameSpace(DUnicodeString const& name) const;

  /**
   * recursive function
   */

  NameSpace*         create(DUnicodeString const& nameSpaces);

  DStruct*           findDStruct(size_t index); //return __structures[index] recursively
  DStruct*           findDStruct(DUnicodeString const& name);//return __structures[name] recurrsively

  void               addStructure(DStruct* dstruct);
private:
  const DUnicodeString      __name;
  std::vector<DStruct*>     __structures;
  std::vector<NameSpace* >  __nameSpaces;  
};

}

#endif
