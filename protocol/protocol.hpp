#ifndef DESTRUCT_DPROTOCOL_HPP
#define DESTRUCT_DPROTOCOL_HPP

namespace Destruct
{

class Destruct;

class Protocol
{
public:
   Protocol(Destruct* destruct);
private:
   void __registerStruct(Destruct* destruct);
};

}
#endif
