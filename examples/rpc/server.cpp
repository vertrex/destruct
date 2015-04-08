#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


#include "server.hpp"
#include "serverobject.hpp"

using namespace Destruct;

Server::Server(uint32_t port) : DCppObject<Server>(NULL, RealValue<DUInt32>(port)), __connectionSocket(), __networkStream(NULL), __serializer(NULL)
{
  this->__bind(port);
}

Server::~Server()
{
  close(this->__connectionSocket);
  delete this->__networkStream;
  delete this->__serializer;
}

#ifdef WIN32
void			Server::__bind(int32_t port)
{
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
  itoa(port, sport, 10);
  if (getaddrinfo(NULL, sport, &hints, &result) != 0)
  {
    WSACleanup();
	throw DException("Server::__bind getaddrinfo failed.");
  }
  this->__listenSocket = INVALID_SOCKET;

  std::cout << "createsocket port " << sport << std::endl;
  this->__listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);  
  if (this->__listenSocket == INVALID_SOCKET) 
  {
	freeaddrinfo(result);
    WSACleanup();
	throw DException("Server::__bind Could not create socket");
  }
  std::cout << "socket created " << std::endl;
 if (bind(this->__listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
 {
   freeaddrinfo(result);
   closesocket(this->__listenSocket);
   WSACleanup();
   throw DException("Bind failed");
 }

}
#else
void            Server::__bind(int32_t port)
{
  this->__listenSocket = socket(AF_INET , SOCK_STREAM, 0);
  if (this->__listenSocket == -1)
    throw DException("Sever::__bind Can't create socket. ");

  int on = 1;
  if (setsockopt(this->__listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) == -1)
    throw DException("Server::__bind Can't set socket options");
  if (this->__listenSocket == -1)
    throw DException("Server::__bind Could not create socket");
     
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);
  memset(&server.sin_zero, 0, sizeof(server.sin_zero));

  if(bind(this->__listenSocket,(sockaddr *)&server , sizeof(server)) < 0)
    throw DException("Server::__bind bind failed. Error");
}
#endif

#ifdef WIN32
void			Server::__listen(void)
{
  std::cout << "Waiting for incoming connections ..." << std::endl;
  if (listen(this->__listenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
	printf( "Listen failed with error: %ld\n", WSAGetLastError() );
	closesocket(this->__listenSocket);
    WSACleanup();
	throw DException("Server::__listen failed");
  }
// Accept a client socket
  this->__connectionSocket = accept(this->__listenSocket, NULL, NULL);
  if (this->__connectionSocket == INVALID_SOCKET) 
  {
	closesocket(this->__listenSocket);
    WSACleanup();
	throw DException("Server::__listen accept failed");
  }
  std::cout << "Connection accepted" << std::endl;
  delete this->__networkStream;
  delete this->__serializer;
  this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__serializer = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
}
#else
void            Server::__listen(void) 
{
  int c;
  struct sockaddr_in client;
 
  listen(this->__listenSocket , 3);
  std::cout << "Waiting for incoming connections ..." << std::endl;
  c = sizeof(sockaddr_in);
  this->__connectionSocket = accept(this->__listenSocket, (sockaddr *)&client, (socklen_t*)&c);//XXX windows
  if (this->__connectionSocket < 0)
    throw DException("Server::__listen accept failed");
  std::cout << "Connection accepted" << std::endl;

  delete this->__networkStream;
  delete this->__serializer;
  this->__networkStream = new NetworkStream(NULL, RealValue<DInt32>(this->__connectionSocket));
  this->__serializer = new DSerializeRPC(*this->__networkStream, this->__objectManager, this->__functionObjectManager);
}
#endif

void    Server::findDStruct(void)
{
  DUnicodeString name;
  this->__networkStream->read(name);

  std::cout << "Send DStruct " << name << std::endl;
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct)
   throw DException("Server::findDStruct DStruct not found");

  this->__serializer->serialize(*this->__networkStream, *dstruct);
  this->__networkStream->flush();
}
 
void Server::unknown(const DUnicodeString& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;
  this->__networkStream->write("Unknown command : " + cmd);
  this->__networkStream->flush();
}

void            Server::daemonize(void)
{
  while (1)
  {
    try
    {
	  std::cout << "Daemonize this->Server()" << std::endl;
      this->serve();
    }
    catch (DException const& exception)
    {
      std::cout << "Server error " << exception.error() << std::endl << "Restarting server ... " << std::endl;
    }
    catch (...)
    {
      std::cout << "Server error : unknown " << std::endl << " Restarting server ... " << std::endl;
    }
  }
}

void            Server::serve(void)
{
  std::cout << "Sever::serve listen" << std::endl;
  this->__listen();
  std::cout << "Create serverObject " << std::endl;
  ServerObject serverObject(*this->__networkStream, this->__serializer, this->__objectManager, this->__functionObjectManager);
  this->initRoot(); //again? XXX
  this->showRoot();

  while (true)
  {
    //std::cout << "Wait for message..." << std::endl;
    DUnicodeString msg;
    this->__networkStream->read(msg);

    if (msg == "show") 
      this->showRoot();
    else if (msg == "findDStruct")
      this->findDStruct();
    else if(msg == "setValue")
      serverObject.setValue();
    else if(msg == "getValue")
      serverObject.getValue();
    else if(msg == "call")
      serverObject.call();
    else if(msg == "call0")
      serverObject.call0();
    else if(msg == "functionCall")
      serverObject.functionCall();
    else if(msg == "functionCall0")
      serverObject.functionCall0();
    else
      this->unknown(msg);
  }
}

ObjectManager<DObject*>& Server::objectManager(void) 
{
  return (this->__objectManager);
}

ObjectManager<ServerFunctionObject*>& Server::functionObjectManager(void) 
{
  return (this->__functionObjectManager);
}

void            Server::showRoot(void)
{
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DObject* stream = destruct.generate("DStreamCout");
  DObject* serializer = destruct.generate("SerializeText", RealValue<DObject*>(stream));

  serializer->call("DObject", RealValue<DObject*>(this->__objectManager.object(0)));
  serializer->destroy();
  stream->destroy();
}
