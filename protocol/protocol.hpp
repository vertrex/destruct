#ifndef DESTRUCT_DPROTOCOL_HPP
#define DESTRUCT_DPROTOCOL_HPP

namespace Destruct
{
class Destruct;

//template <typename CppClass>
//inline 
//void  register(DUnicodeString const& name)
//{
  //Destruct::instance().registerDStruct(new DStruct(NULL, name, DCppObject<CppClass>::newObject, CppClass::ownAttributeBegin(), CppClass::ownAttributeEnd()));
//}

class Protocol
{
public:
   Protocol(Destruct* destruct);
private:
   void __registerStruct(Destruct* destruct);
};

}
#endif
