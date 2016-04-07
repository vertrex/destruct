#ifndef __DB__
#define __DB__

#include <stdint.h>

#include "dstructs.hpp"
#include "dobject.hpp"
#include "dunicodestring.hpp"
#include "session.hpp"

using namespace Destruct;

class DB
{
public:
                        DB();
                        ~DB();
  void                  declare(void);
  DObject*    populateSession(void);
  void                  show(DObject* dobject) const; 
  void                  load(DValue const& filePath);
  Session*              session(void);
private:
  DStructs&             __destruct;
  Session*              __session;
};

#endif
