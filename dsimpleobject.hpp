#ifndef DSTRUCT_SIMPLE_OBJECT_HPP_
#define DSTRUCT_SIMPLE_OBJECT_HPP_

#include "ddynamicobject.hpp"

#include "dstruct.hpp"

namespace Destruct
{

class DSimpleObject : public DDynamicObject
{
public:
  explicit DSimpleObject(DStruct *);
  DSimpleObject(DSimpleObject const &);

  static DObject*   newObject(DStruct *);
  virtual DObject*  clone() const;
};

}
#endif
