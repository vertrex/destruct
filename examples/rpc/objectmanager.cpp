#include "drealvalue.hpp"
#include "objectmanager.hpp"

/**
 * Object Manager DObject specialization
 */
template<>
Destruct::DObject* ObjectManager<Destruct::DObject* >::object(uint64_t id) const
{
  std::map<uint64_t, Destruct::DObject* >::const_iterator object = this->__objectsID.find(id);
  if (object != this->__objectsID.end())
    return (object->second);
  return Destruct::RealValue<Destruct::DObject*>(Destruct::DNone); 
}
