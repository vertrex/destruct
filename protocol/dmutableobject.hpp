#include "../ddynamicobject.hpp"
#include "../dsimpleobject.hpp"
#include "../drealvalue.hpp"

#include <string>
//#include "../dstruct.hpp"
class DMutableStruct;

namespace Destruct
{

class DMutableObject : public DDynamicObject
{
public:
  DMutableObject(const DUnicodeString& name); 
  DMutableObject(DMutableStruct* dstructDef); 
  DMutableObject(DMutableObject const &);

  using DObject::getValue;
  using DObject::setValue;

  virtual DValue           getValue(size_t idx) const;
  virtual void             setValue(size_t idx, DValue const &);
  virtual DValue           call(size_t idx, DValue const&); //this is juste an helper function

  void setValueAttribute(std::string const& name, DValue const& v, DType::Type_t type);

  //template<class realtype> //XXX style must have non templated version
  //void setValueAttribute(std::string const& name, RealValue<realtype> v, DType::Type_t type)
  ////void setValueAttribute(std::string const& name, DValue const &v = RealValue<realtype>(arg), DType::Type_t type);
  //{
    //DAttribute attribute(name, type);
    //
    //this->instanceOf()->addAttribute(attribute);
    //this->__values.push_back(attribute.type().newValue()); //set new value ou directement v possible ?
    //this->__values.back()->set(v);
  //}

  static DObject* newObject(DMutableStruct* myClass);
  virtual DObject*  clone() const;
};

}
