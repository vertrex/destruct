#include "serverobject.hpp"
#include "serializerpc.hpp"
#include "serverfunctionobject.hpp"

using namespace Destruct;

/* 
 *  ServerObject
 */
ServerObject::ServerObject(void* socket, void * context): __networkStream(NULL), __serializer(NULL), __deserializer(NULL), __objectManager(NULL)
{
  this->__objectManager = DStructs::instance().find("ObjectManager")->newObject();
  this->__networkStream = DStructs::instance().generate("NetworkStream");
  NetworkStream* ns = static_cast<NetworkStream*>(this->__networkStream);
  ns->__context = context;
  ns->__socket = socket;

  this->__serializer = DStructs::instance().generate("SerializeRPC", RealValue<DObject*>(this->__networkStream));
  this->__deserializer = DStructs::instance().generate("DeserializeRPC", RealValue<DObject*>(this->__networkStream));
}

ServerObject::~ServerObject()
{
  this->__objectManager->destroy();
  this->__serializer->destroy();
  this->__deserializer->destroy();
  this->__networkStream->destroy();
}

void    ServerObject::find(void)
{
  DUnicodeString name = this->__deserializer->call("DUnicodeString"); 
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct) //XXX must send exception to client !
    throw DException("Server::generate DStruct " + name + " not found"); 
  this->__networkStream->call("reply");
  this->__serializer->call("DStruct", RealValue<DStruct*>(dstruct));
  this->__networkStream->call("flushWrite");
}

void    ServerObject::generate(void)
{
  DUnicodeString name = this->__deserializer->call("DUnicodeString"); 
  std::cout << "Generate object " << name << std::endl;
  Destruct::DStructs& destruct = Destruct::DStructs::instance();
  DStruct* dstruct = destruct.find(name);
  if (!dstruct) ///XXX must send exception to client !
    throw DException("Server::generate DStruct " + name + " not found"); 
  DObject* object = dstruct->newObject(); 
  RealValue<DUInt64> objectId = this->__objectManager->call("registerObject", RealValue<DObject*>(object));

  this->__networkStream->call("reply");
  this->__serializer->call("DUInt64", RealValue<DUInt64>(objectId));
  this->__networkStream->call("flushWrite");
}

void    ServerObject::setValue(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DUnicodeString name = this->__deserializer->call("DUnicodeString");

  DObject* object = this->__objectManager->call("object", id);

  DValue value = this->__deserializer->call(object->instanceOf()->attribute(name).type().name());
  object->setValue(name, value);
  this->__networkStream->call("reply");
}

void    ServerObject::getValue(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DUnicodeString name = this->__deserializer->call("DUnicodeString");
  DObject* object = this->__objectManager->call("object", id);
  
  DValue value = object->getValue(name);

  DType type = object->instanceOf()->attribute(name).type();
  if (type.getType() == DType::DMethodType)
  {
    ServerFunctionObject* serverFunctionObject = static_cast<ServerFunctionObject*>(DStructs::instance().find("ServerFunctionObject")->newObject()); //DStruct get ?
    serverFunctionObject->argumentType = type.getArgumentType();
    serverFunctionObject->returnType = type.getReturnType();
    serverFunctionObject->functionObject = value.get<DFunctionObject*>();

    DValue id = this->__objectManager->call("registerObject", RealValue<DObject*>(serverFunctionObject));
    this->__networkStream->call("reply");
    this->__serializer->call("DUInt64", id);
    this->__networkStream->call("flushWrite");
    serverFunctionObject->destroy();
  }
  else
  {
    this->__networkStream->call("reply"); 
    this->__serializer->call(type.name(), value);
    this->__networkStream->call("flushWrite");
  }
}

//XXX not tested because there is no function in python !
void    ServerObject::call(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DObject* object = this->__objectManager->call("object", id);
  DUnicodeString name = this->__deserializer->call("DUnicodeString");
  DType type = object->instanceOf()->attribute(name).type();

  DValue args = this->__deserializer->call(type.argumentName());
  DValue value = object->call(name, args);

  this->__networkStream->call("reply");
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flushWrite");
}

//XXX not tested because tehre is no function in python
void    ServerObject::call0(void)
{
  DValue id = this->__deserializer->call("DUInt64");
  DObject* object = this->__objectManager->call("object", id);
  DUnicodeString name = this->__deserializer->call("DUnicodeString");

  DValue value = object->call(name); 
  
  DType type = object->instanceOf()->attribute(name).type();
  this->__networkStream->call("reply"); 
  this->__serializer->call(type.returnName(), value);
  this->__networkStream->call("flushWrite");
}

void    ServerObject::functionCall(void)
{
  DValue id= this->__deserializer->call("DUInt64");

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", id).get<DObject*>());
  DValue args = this->__deserializer->call(DType((DType::Type_t)(DUInt64)object->argumentType).name()); //XXX get name directly ? 
  DValue value = ((DFunctionObject*)object->functionObject)->call(args);

  this->__networkStream->call("reply");
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 
  this->__networkStream->call("flushWrite");
}

void    ServerObject::functionCall0(void)
{
  DValue id = this->__deserializer->call("DUInt64");

  ServerFunctionObject* object = static_cast<ServerFunctionObject*>(this->__objectManager->call("object", id).get<DObject*>());

  DValue value = ((DFunctionObject*)object->functionObject)->call();

  this->__networkStream->call("reply");
  this->__serializer->call(DType((DType::Type_t)(DUInt64)object->returnType).name(), value); 
  this->__networkStream->call("flushWrite");
}
 
void    ServerObject::unknown(DUnicodeString const& cmd)
{
  std::cout << "Receive unknown command : " << cmd << std::endl;

  //XXX
  this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>("Unknown command : " + cmd));
  //this->__networkStream->call("flush");
}

void    ServerObject::dispatch(void)
{
  DUnicodeString msg = this->__deserializer->call("DUnicodeString").get<DUnicodeString>();

  try
  {
    if (msg == "find")
      this->find();
    else if (msg == "generate")
      this->generate();
     //object call 
    else if(msg == "setValue")
      this->setValue();
    else if(msg == "getValue")
      this->getValue();
    else if(msg == "call")
      this->call();
    else if(msg == "call0")
      this->call0();
      // functionObject call
    else if(msg == "functionCall")
      this->functionCall();
    else if(msg == "functionCall0")
      this->functionCall0();
  }
  catch (DException const& exception)
  {
    //this->__networkStream->call("reply", RealValue<DInt8>(-1));
    //this->__serializer->call("DUnicodeString", RealValue<DUnicodeString>(exception.error()));
    this->__networkStream->call("replyError", RealValue<DUnicodeString>(exception.error()));
  }
}
