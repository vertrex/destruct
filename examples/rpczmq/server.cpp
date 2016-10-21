#include <iostream>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#endif

#include "server.hpp"
#include "serverobject.hpp"

#include <zmq.h>
#include <czmq.h>

using namespace Destruct;

Server::Server(uint32_t port) : DCppObject<Server>(NULL, RealValue<DUInt32>(port)), __objectManager(NULL)
{
  this->__bind(port);
  this->__objectManager = DStructs::instance().generate("ObjectManager");
}

Server::Server(DStruct* dstruct, DValue const& args) : DCppObject<Server>(dstruct, args), __objectManager(NULL) 
{
  this->init();
  this->__bind(args.get<DUInt32>());
  this->__objectManager = DStructs::instance().generate("ObjectManager");
}

Server::~Server()
{
  zmq_close(this->__socket);
  zmq_ctx_destroy(this->__context);
  //zcert_destroy;

  this->__objectManager->call("clear"); //called in destroy 
  this->__objectManager->destroy();
}

void    Server::setRoot(RealValue<DObject*> root)
{
  this->__objectManager->call("registerObject", root);
}
/**
 *   Use public key if server is corrupted no problem
 */
void    Server::__setAuth(DUnicodeString const& certificate)
{
   if (zsys_has_curve() == false)
	 throw DException("Curve encryption not supported");
   zcert_t* client_cert = zcert_load("cert\\rpczmq_cert.txt");
   if (client_cert == NULL)
     throw DException("Can't load our certificate");
  zcert_apply(client_cert, this->__socket);
  zcert_t* server_pub_cert = zcert_load("cert\\rpczmq_client_cert.txt");
  if (server_pub_cert == NULL)
	throw DException("Can't load client certificate");
  const char* server_pub_key = zcert_public_txt(server_pub_cert);
  zsocket_set_curve_serverkey(this->__socket, server_pub_key);
}

void    Server::__bind(int32_t port)
{
  this->__context = zctx_new();
  this->__socket = zsocket_new((zctx_t*)this->__context, ZMQ_REP);

  this->__setAuth("cert/rpczmq_cert.txt");

  std::stringstream address;
  address << "tcp://*:" << port;

  zmq_bind(this->__socket, address.str().c_str());
}

void    Server::daemonize(void)
{
  while (1)
  {
    try
    {
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
       #include <assert.h>
int           get_monitor_event (void *monitor, int *value, char **address)
{
   //  First frame in message contains event number and value
   zmq_msg_t msg;
   zmq_msg_init (&msg);
   if (zmq_msg_recv (&msg, monitor, ZMQ_DONTWAIT) == -1)
     return -1;              //  Interruped, presumably
   assert (zmq_msg_more (&msg));

    uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
    uint16_t event = *(uint16_t *) (data);
    if (value)
      *value = *(uint32_t *) (data + 2);

     //  Second frame in message contains event address
    zmq_msg_init (&msg);
    if (zmq_msg_recv (&msg, monitor, 0) == -1)
      return -1;              //  Interruped, presumably
   assert (!zmq_msg_more (&msg));

    if (address) 
    {
      uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
      size_t size = zmq_msg_size (&msg);
      *address = (char *) malloc (size + 1);
      memcpy (*address, data, size);
      *address [size] = 0;
    }
   return event;
}


void    Server::serve(void)
{
  ServerObject serverObject(this->__socket, this->__context); //one for zmq connection ?
  std::cout << "Destruct server started" << std::endl;
  //zmq_socket_monitor (this->__socket, "inproc://monitor.rep", ZMQ_EVENT_ALL);
  //void *serv_mon= zmq_socket (this->__context, ZMQ_PAIR);
  //zmq_connect (serv_mon, "inproc://monitor.rep");
  while (true)
  {
    //int event = get_monitor_event(serv_mon, NULL, NULL);
    //if (event == ZMQ_EVENT_CONNECTED)
      //std::cout << "Connection from " << std::endl;
    //else if (event == ZMQ_EVENT_CLOSED)
     //std::cout << "Connection closed from " << std::endl;
    //else if (event == ZMQ_EVENT_DISCONNECTED)
    //{
     //std::cout << "Disconnection from " << std::endl;
     //exit(0);
    //}

    serverObject.dispatch();
  }
}

DObject* Server::objectManager(void) 
{
  return (this->__objectManager);
}

