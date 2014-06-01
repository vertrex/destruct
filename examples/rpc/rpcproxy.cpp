DRPCProxyObject(NetworkStream *stream, object) : DObject
{
}

DRPCProxyObject::call(void)
{
}

DRPCProxyObject::get(value)
{
  networkStream->message("get");
  value = networkStream->get(value);
}


DRPCProxyObject::set(value)
{
  networkStream->message("send");
  networkStream->
  networkStream->send(value);
}
