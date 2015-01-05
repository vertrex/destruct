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

class Destruct
{
public:
  typedef std::vector<DStruct* >  Container;
  typedef Container::iterator     Iterator;

  static Destruct& instance(void);

  size_t        count(void);
  DStruct*      find(DUnicodeString const & name);
  DStruct*      find(const size_t name);  
  void          registerDStruct(DStruct* dstruct);
  void          registerDStruct(std::string const& name, DStruct* dstruct);
  bool          unregister(DStruct* dstruct);
  DObject*      generate(DUnicodeString const& name);
  DObject*      generate(DUnicodeString const& name, DValue const& args);
private:
  Destruct();
  ~Destruct();
  Destruct(const Destruct&);
  Destruct&                 operator=(Destruct&);

  NameSpace*                 __nameSpace; //NameSpace("")
};

class NameSpace
{
public:
  NameSpace(std::string const& name);
  ~NameSpace(void);
 
 
  const std::string  name(void) const;
  size_t             count(void) const;

  /**
   *  return __function
   */
  DStruct*           dstruct(const size_t index);
  DStruct*           dstruct(std::string const& name);
  NameSpace*         nameSpace(std::string const& name) const;

  /**
   * recursive function
   */

  NameSpace*         create(std::string const& nameSpaces);

  DStruct*           findDStruct(size_t index); //return __structures[index] recursively
  DStruct*           findDStruct(DUnicodeString const& name);//return __structures[name] recurrsively

  void               addStructure(DStruct* dstruct);
private:
  const std::string         __name;
  std::vector<DStruct*>     __structures;
  std::vector<NameSpace* >  __nameSpaces;
  
};

}

#endif
