#include <iostream>
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#include "client.hpp"
#include "rpcobject.hpp" 

Client::Client(std::string const& addr, uint32_t port)
{
  this->__connect(addr, port);
}

Client::~Client()
{
  this->__close();
}

NetworkStream   Client::stream(void)
{
  return (NetworkStream(this->__connectionSocket));
}
void    Client::__connect(std::string const& addr, uint32_t port)
{
  struct sockaddr_in server;

  this->__connectionSocket = socket(AF_INET , SOCK_STREAM , 0);
  if (this->__connectionSocket == -1)
    throw std::string("Could not create socket");
  std::cout << "socket created" << std::endl;
     
  server.sin_addr.s_addr = inet_addr(addr.c_str());
  server.sin_family = AF_INET;
  server.sin_port = htons(port);
 
  if (connect(this->__connectionSocket, (struct sockaddr *)&server , sizeof(server)) < 0)
    throw std::string("connect failed. Error");
}



void    Client::__close(void)
{
  //close(this->__connectionSocket);
}

void    Client::start(void)
{
  char message[1000] , server_reply[2000];
  //keep communicating with server

  //get Root Object
  RPCObject* remote = new RPCObject(this->stream(), "Root");
  DUnicodeString remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "root->name : " << remoteName << std::endl;

  remote->setValue("name", RealValue<DUnicodeString>("rename-by-remote"));

  remoteName = remote->getValue("name").get<DUnicodeString>();
  std::cout << "root->name after rename: " << remoteName << std::endl;

  this->stream().write("show");

  remoteName = remote->call("path").get<DUnicodeString>();
  std::cout << "remote call path : " << remoteName << std::endl;

//call path
    //Serializers::to("Text")->(remote); //show remote object locally

  //while(1)
  //{
    /*  
    std::cout << "Enter message : " << std::endl;
    scanf("%s" , message);
    if(this->_send(message , strlen(message)) < 0)
      throw std::string("Send failed");
        
    int32_t received = this->_receive(server_reply , 2000);

    if (received < 0)
      throw std::string("recv failed");
    if (received >= 2000)
      *(server_reply + 2000) = 0;
    else
      *(server_reply + received) = 0; 

    std::cout << "Server reply :" << std::endl;
    std::cout << server_reply << std::endl; */
    //}
}
