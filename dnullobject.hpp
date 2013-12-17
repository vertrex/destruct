#ifndef _DESTRUCT_DNULLOBJECT_HPP_
#define _DESTRUCT_DNULLOBJECT_HPP_

#include "dstruct.hpp"
#include "dobject.hpp"

namespace Destruct
{

class DNullObject : public DObject
{
public:
  static DNullObject*   instance(); //on a deja un instance of :) donc les utiliser pour faire les singletons il faut juste une methode DInstance singleton ! au lieu de new object ! 
  DObject*  clone() const;
  DValue    getValue(size_t index) const;
  void      setValue(size_t index, DValue const&);
  DValue    call(size_t index, DValue const&);
  DValue    getValue(std::string const& name) const;
  void      setValue(std::string const& name, DValue const& value);
  DValue    call(std::string const& name, DValue const& value);

protected:
  BaseValue*       getBaseValue(size_t index);
  BaseValue const* getBaseValue(size_t index) const;

  void             destroy(); //singleton must avoid to call ~DNullObject who use a recuring template pattern
private:
  static DStruct*       __dstructInstance();

// d ailleurs peutre faire un DNull : DStruct dnull herite de dstruct et la methode newObject renvoie tjrs le meme object ce qui fait un vrai singleton sans singleton !!!! XXX utiliser ca <- si possible c un meilleur designe on populate les object qu on veux
// donc on peut faire un dynamiqudestruct qui genera des objet modifiable car leur instance seront modiifiable il faut donc pas modifier l objet mais sont instance , meme s il peut heriter d une instance fixe 


  DNullObject();
  ~DNullObject();
}; 

static DNullObject* const DNone = DNullObject::instance();

}

#endif
