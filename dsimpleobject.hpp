#ifndef DSTRUCT_SIMPLE_OBJECT_HPP_
#define DSTRUCT_SIMPLE_OBJECT_HPP_

#include "ddynamicobject.hpp"

#include "dstruct.hpp"

namespace Destruct
{

class DSimpleObject : public DDynamicObject
{
public:
  explicit DSimpleObject(DStruct *, DValue const& args);
  DSimpleObject(DSimpleObject const &);

  static DObject*   newObject(DStruct *, DValue const& args);
  virtual DObject*  clone() const;
};

}
#endif
