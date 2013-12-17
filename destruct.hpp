#ifndef DESTRUCT_DESTRUCT_HPP_
#define DESTRUCT_DESTRUCT_HPP_

#include <list>
#include <algorithm>
#include <iterator>
#include "dunicodestring.hpp"
#include "dstruct.hpp"


namespace Destruct
{
/* 
 *This is the dictionary of DStruct class it use a singleton pattern but should me modified to be use in a session object
 */

class DStruct;

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
  bool          unregister(DStruct* dstruct);
  DObject*      generate(DUnicodeString const& name);
private:
                Destruct();
                ~Destruct();
                Destruct(const Destruct&);
  Destruct&     operator=(Destruct&);
  Container     __structures;
};

}

#endif
