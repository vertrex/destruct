#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include <string.h>

#include "networkstream.hpp"

namespace Destruct 
{

NetworkStream::NetworkStream(DStruct* dstruct, DValue const& args) : DStream(dstruct), __readBuffer(new uint8_t[4096])
{
  this->__socket = args.get<DInt32>();
}

NetworkStream::NetworkStream(NetworkStream const& copy) : DStream(copy), __socket(copy.__socket), __readBuffer(new uint8_t[4096]), __buff(copy.__buff)
{
        //std::cout << "NetworkStream(NetworkStream const& copy) " << std::endl;
}

NetworkStream::~NetworkStream()
{
        //std::cout << "Deleting NetworkStream" << std::endl;
  this->__close();
}

int32_t NetworkStream::write(std::string const& str) const
{
 StringBuff sb(str);

 //std::cout << "write buff of size " << sb.size() << std::endl;
 //uint32_t readed = send(this->__socket, (void*)sb.data(), sb.size(), 0);
 ////uint32_t readed = this->write((void*)&size, sizeof(size));
 ////readed = this->write((void*)str.c_str(), str.size() );
 //
 //return readed;

  uint64_t size = str.size();
  uint32_t readed = this->write((void*)&size, sizeof(size));
  if (readed != sizeof(size))
    throw std::string("Can't write string size");

  readed = this->write((void*)str.c_str(), size);
  if (readed != size)
    throw std::string("Can't write string");
  return readed;
}

int32_t NetworkStream::write(uint64_t id) const
{
  return (send(this->__socket, (void*)&id, sizeof(id), 0));
}

int32_t NetworkStream::write(void* buff, int32_t size) const 
{
  return (send(this->__socket, buff, size, 0));
}

int32_t NetworkStream::read(std::string & readValue)
{    
        ////uint64_t size = 0;
        ////std::cout << "NetworkStream::read(std::string& val) " << std::endl;
        
        //if (this->__buff.toRead() == false) 
        //{
                ////std::cout << "Receiving data " << std::endl;
          //ssize_t received = recv(this->__socket, this->__readBuffer, 4096, 0);
          ////if buffer.dataSize(); //not chomped
          ////else read ? 
          //if (received >= 4096 || received == -1 || received == 0)
          //{
               //// std::cout << "NetworkStreak::read buffer size exceed" << received << std::endl;
              ////  return received;
          //}
          //else
          //this->__buff.add((char*)this->__readBuffer, received);
          //readValue = this->__buff.get(); //std::string((char*)this->__readBuffer + sizeof(uint64_t) , received);
        //}
        ////StringBuff 
        //else
        //{
                ////std::cout << "  Get data from cache " << std::endl;
        //readValue = this->__buff.get(); //std::string((char*)this->__readBuffer + sizeof(uint64_t) , received);
        //}
        ////size= *(uint64_t*)this->__readBuffer;
        ////readValue = std::string((char*)this->__readBuffer + sizeof(uint64_t) , received);
        ////std::cout << " finally   " << readValue <<  std::endl;
        //return readValue.size();

        uint64_t size = 0;
        if (recv(this->__socket, &size, sizeof(size), 0) != sizeof(size))
                throw std::string("NetworkStream::read can't get size");
        uint8_t*  value = new uint8_t[size + 1];
        recv(this->__socket, value, size, 0); //test return value
        value[size] = 0;
        readValue = std::string((char*)value, size);
        delete value;

        return readValue.size();

}

int32_t NetworkStream::read(uint64_t* id)
{
        //std::cout << "NetworkStream::read(uint64_t id)" << std::endl;

  if (this->__buff.toRead() == false)
    return (recv(this->__socket, (void*)id, sizeof(id), 0));
  //else
  //std::cout << "MUST COPY MEMORY " << std::endl;

  return (sizeof(uint64_t));
  // memcpy(id, 
}

int32_t NetworkStream::read(void* buff, int32_t size)
{
        //std::cout << "NetworkStream::read(buff, " << size << ")" << std::endl;

  if (this->__buff.toRead() == false) 
    return(recv(this->__socket, buff, size, 0));
  //else
  //std::cout << "MUST COPY MEMORY " << std::endl;
  return (size);
}

void NetworkStream::__close(void)
{
}

DStream& NetworkStream::operator<<(DStreamString& input)
{

  this->write(input.str());
  input.clear();
  return (*this);
}

DStream& NetworkStream::operator>>(DStreamString& output)
{
//  std::cout << "NetworkStream::Reading >> " << std::endl;

  //if (this->__buff.toRead())
    //std::cout << "MUST COPY ATA " << std::endl;
        //uint64_t size = 0;
        //if (this->read(&size, sizeof(size)) != sizeof(size))
                //throw std::string("NetworkStream::read can't get size");
        //uint8_t*  value = new uint8_t[size + 1];
        //this->read(value, size);
        //value[size] = 0;
        //output.write((char*)value, size); 
        //delete value;

  std::string tmp;
  this->read(tmp);
  //std::cout << " >> networkstream write to output streamstring " 
            //<< tmp.size() << "" 
            //<< tmp <<  std::endl;
  output.write(tmp.c_str(), tmp.size());
  return (*this);
}

}
