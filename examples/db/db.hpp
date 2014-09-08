#ifndef __RPC__
#define __RPC__

#include <stdint.h>

#include "destruct.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "session.hpp"


class DB
{
public:
                        DB();
                        ~DB();
  void                  declare(void);
  Destruct::DObject*    populateSession(void);
  void                  show(Destruct::DObject* dobject) const; 
  void                  load(Destruct::DValue const& filePath);
  Session*              session(void);
private:
  Destruct::Destruct&   __destruct;
  Session*              __session;
};

#endif
