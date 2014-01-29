#ifndef _DESTRUCT_DOBJECT_HPP_
#define _DESTRUCT_DOBJECT_HPP_

#include <memory>
#include "dstruct.hpp"
#include "dpolicy.hpp"

namespace Destruct
{
class DValue;
class BaseValue;

class DObject : public RefcountPolicy<DObject> //public UUIDPolicy<DObject > possible ? 
{
public:
  friend class RefcountPolicy<DObject>;
 
  DObject(DStruct * dstructDef); //XXX pour la deserialization DObject.deserialize9) mais si non en static

  DStruct * instanceOf() const;
  virtual DObject* clone() const = 0;
  virtual DValue getValue(size_t index) const = 0;               //set data member
  virtual void setValue(size_t idx, DValue const &) = 0;         //set 
  virtual DValue call(size_t index, DValue const &)  = 0; //XXX ds destruct plutot que ds DObject pout etre set qu une seul fois ! ?XXX voir cppobject etc... mais si non a chaque objet faut set le callee spe peut etre plutot faire un dstruct->find(callee)->call() pour eviter de copier les functions a chaque fois 
//  virtual DValue call(size_t) = 0; ? optim ? evite de passer DNone a chaque fois quand on c que c void ...
//  virtual void call(void) = 0; ? optim ? 


  virtual DValue getValue(std::string const& name) const;        //set data member
  virtual void setValue(std::string const& name, DValue const &); //
  virtual DValue call(std::string const& name, DValue const &); // ? donc c un dcallableobject ?
//this is un helper pour call des functions c tout a ne pas confondre avec le "call" da value qui a rien a voir la dedans

// UUID() -> object must have a uniq ID ! ou alors wrapper au dessus mais ca sera utile a un oment ou un autre (si non un hash comme en python) 
//  operator ->(std::string) ? ->toto = get->value = ?

protected:
  DObject(const DObject& rhs) : __dstructDef(rhs.__dstructDef) 
  {
  }
  virtual BaseValue* getBaseValue(size_t index) = 0;
  virtual BaseValue const* getBaseValue(size_t index) const = 0;
  static  BaseValue* getBaseValue(DObject*, size_t index);
  static  BaseValue const* getBaseValue(DObject const*, size_t idx);
  virtual ~DObject()
  {
      //delete __dstructDef; -> sert a rien c pas car l objet est detruit qu o nva pas en recree a partir de la structure, ...
      // mais si y a un refcount on pourrait le call ici et a chaque newobject ?  utile ?
  }
private:
  DStruct * const __dstructDef;
};


}

#endif
