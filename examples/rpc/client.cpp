
#ifndef WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <netinet/tcp.h>
#endif


#include "dstructs.hpp"

#include "client.hpp"
#include "clientobject.hpp"
#include "serializerpc.hpp"
#include "networkstream.hpp"

#include "dsimpleobject.hpp"
#include "server.hpp"
#include "objectmanager.hpp"
#include "serverfunctionobject.hpp"


/**
 *  Client & Server Destruct Declaration
 */
extern "C"
{
  void DestructExport()
  {
    Client::declare();
  }
}

void    Client::declare(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  destruct.registerDStruct(makeNewDCpp<NetworkStream>("NetworkStream"));
  destruct.registerDStruct(makeNewDCpp<SerializeRPC>("SerializeRPC"));
  destruct.registerDStruct(makeNewDCpp<DeserializeRPC>("DeserializeRPC"));

  destruct.registerDStruct(makeNewDCppSingleton<ObjectManager >("ObjectManager"));
  destruct.registerDStruct(makeNewDCpp<ServerFunctionObject>("ServerFunctionObject"));


  DStruct* dstruct = makeNewDCpp<Server>("Server"); 
  destruct.registerDStruct(dstruct);

  dstruct = makeNewDCpp<Client>("Client");
  destruct.registerDStruct(dstruct);

  dstruct = new DStruct(NULL, "ClientArgument", DSimpleObject::newObject);
  dstruct->addAttribute(DAttribute(DType::DUInt32Type, "port"));
  dstruct->addAttribute(DAttribute(DType::DUnicodeStringType, "address"));
  destruct.registerDStruct(dstruct); 
}

/**
 *  Client
 */
Client::Client(DUnicodeString const& addr, uint32_t port) : DCppObject<Client>(NULL, RealValue<DObject*>(DNone)), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();
  this->__connect(addr, port);
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject(RealValue<DInt32>(this->__connectionSocket));
  this->__serialize = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
  this->__deserialize = DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::Client(DStruct* dstruct, DValue const& value) : DCppObject<Client>(dstruct, value), __networkStream(NULL), __serialize(NULL), __deserialize(NULL)
{
  this->init();
  DObject* args = value;

  DUnicodeString addr = args->getValue("address");
  uint32_t port = args->getValue("port").get<DUInt32>();
  this->__connect(addr, port);
  this->__networkStream = DStructs::instance().find("NetworkStream")->newObject(RealValue<DInt32>(this->__connectionSocket));
  this->__serialize = DStructs::instance().find("SerializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
  this->__deserialize = DStructs::instance().find("DeserializeRPC")->newObject(RealValue<DObject*>(this->__networkStream));
}

Client::~Client()
{
  std::cout << "~Client" << std::endl;
  this->__networkStream->destroy();
  this->__serialize->destroy();
  this->__deserialize->destroy();
  this->__close();
}

void    Client::__connect(DUnicodeString const& addr, uint32_t port)
{
#ifdef WIN32
  struct addrinfo *result = NULL;
  struct addrinfo hints;
  WSADATA wsaData;

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    throw DException("WSAStartup failed");

  ZeroMemory(&hints, sizeof (hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

// Resolve the local address and port to be used by the server
  char sport[6];
  ZeroMemory(&sport, sizeof(char)*6);
  _itoa_s(port, sport, 10);
  if (getaddrinfo(NULL, sport, &hints, &result) != 0)
  {
    WSACleanup();
	throw DException("Server::__bind getaddrinfo failed.");
  }
  this->__connectionSocket = (DInt32)INVALID_SOCKET;

  std::cout << "createsocket port " << sport << std::endl;
  this->__connectionSocket = (DInt32)socket(result->ai_family, result->ai_socktype, result->ai_protocol);  
#else

  this->__connectionSocket = (DInt32)socket(AF_INET , SOCK_STREAM, 0);
#endif
  if (this->__connectionSocket == -1)
    throw DException("Client::__connect Could not create socket");

  int on = 1;
  if (setsockopt(this->__connectionSocket, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(on)) == -1)
  throw DException("Server::__bind Can't set socket options");
 
    sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  memset(&server.sin_zero, 0, sizeof(server.sin_zero));

  if (connect(this->__connectionSocket, (sockaddr *)&server , sizeof(server)) < 0)
    throw DException("Client::__connect connect failed. Error");
}

void    Client::__close(void)
{
#ifdef WIN32
  closesocket(this->__connectionSocket);
#else
  close(this->__connectionSocket);
#endif
 
}

DObject*   Client::start(void)
{
  throw DException("Client::start Not implemented.");
}

DValue     Client::findObject(void) //getRoot XXX ?
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();

  DStruct* registryS = destruct.find("Registry");

  ClientObject* root = new ClientObject(RealValue<DObject*>(this->__networkStream), RealValue<DObject*>(this->__serialize), RealValue<DObject*>(this->__deserialize), 0, registryS); 

  return (RealValue<DObject*>(root));
}

Destruct::DStruct* Client::remoteFind(const DUnicodeString name)
{
  this->__serialize->call("DUnicodeString", RealValue<DUnicodeString>("findDStruct"));
  this->__serialize->call("DUnicodeString", RealValue<DUnicodeString>(name));
  this->__networkStream->call("flush");
 
  DStruct* dstruct = this->__deserialize->call("DStruct");
  if (dstruct)
  {
    Destruct::DStructs& destruct = Destruct::DStructs::instance();
    if (destruct.find(dstruct->name()) == NULL) //XXX don't need to find again
      destruct.registerDStruct(dstruct);
    else
      std::cout << dstruct->name() << " already registered" << std::endl;
    //this->print(dstruct); 
  } 
  else
    std::cout << "Struct " << name << " is NULL can't show content " << std::endl;
  return (dstruct);
}

bool    Client::print(DStruct* dstruct) const
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* stream = destruct.generate("DStreamCout");

  if (!dstruct)
    return (false);
 
  DObject* serializer = destruct.generate("SerializeText", RealValue<DObject*>(stream));
  serializer->call("DStruct", RealValue<DStruct*>(dstruct));

  serializer->destroy();
  stream->destroy();
  return (true);
}

bool    Client::print(DObject* dobject) const
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* stream = destruct.generate("DStreamCout");
  if (!dobject)
    return (false);

  DObject* serializer = destruct.generate("SerializeText", RealValue<DObject*>(stream));
  serializer->call("DObject", RealValue<DObject*>(dobject));
  
  serializer->destroy();
  stream->destroy();
  return (true);
}

int32_t Client::connectionSocket(void) const
{
  return (this->__connectionSocket);
}

DObject*     Client::networkStream(void) const
{
  return (this->__networkStream);
}

DObject*     Client::serializeRPC(void) const
{
  return (this->__serialize);
}

DObject*     Client::deserializeRPC(void) const
{
  return (this->__deserialize);
}
