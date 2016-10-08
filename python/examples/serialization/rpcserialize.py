#!/usr/bin/python -i 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *


class ComplexObject(DStruct):
  def __init__(self):
    DStruct.__init__(self, None, "ComplexObject")
    self.addAttribute(DAttribute("name", DUnicodeString))
    self.addAttribute(DAttribute("number", DUInt64))

def usage():
   print "./rpcserialize -c [server] [port]"

if __name__ == "__main__":
 if len(sys.argv) < 2:
   usage()
 else:
   registry = None
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpczmq.so")
   #loader.file("../../../examples/modules/libdestruct_performance.so")
   print 'ok loaded'
   server = None
   if sys.argv[1] == '-l':
      performance = DStructs().find("Performance") .newObject()
   elif sys.argv[1] == '-c':
     arg = DStructs().find("ClientArgument").newObject()
     arg.address = "127.0.0.1"
     arg.port = 0xdff
     if len(sys.argv) == 4:
       arg.address = sys.argv[2]
       arg.port = int(sys.argv[3])
     elif len(sys.argv) == 3:
       arg.address = sys.argv[2]
     server  = DStructs().find("Client").newObject(arg)
     #serverLoader = client.generate("Import")
     #if serverLoader.file("../modules/libdestruct_performance.so") == 0:
       #serverLoader.file("destruct_performance.dll")
     #serializer = client.generate("SerializeText", stream)
   else:
      usage()

    
   stream = server.find("DStreamCout").newObject() ##XXX none
   #serialize = server.find("SerializeText").newObject(stream)
   #serverVector = server.generate("DVectorObject")
#
   #complex1 = ComplexObject().newObject()
   #complex1.name = "First object"
   #complex1.number = 1
#
   #serverVector.push(complex1)
   
   #serializer.DObject(object)
