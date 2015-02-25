#ifndef DSTRUCT_SIMPLE_OBJECT_HPP_
#define DSTRUCT_SIMPLE_OBJECT_HPP_

#include "destruct.hpp"
#include "ddynamicobject.hpp"

namespace Destruct
{

class DSimpleObject : public DDynamicObject
{
public:
  EXPORT explicit DSimpleObject(DStruct *, DValue const& args);
  EXPORT DSimpleObject(DSimpleObject const &);

  EXPORT static DObject*   newObject(DStruct *, DValue const& args);
  EXPORT virtual DObject*  clone() const;
protected:
  virtual ~DSimpleObject();
};

}
#endif
