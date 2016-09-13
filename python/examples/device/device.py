#!/usr/bin/python -i 
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

if __name__ == "__main__":
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpc.so")
   loader.file("../../../examples/modules/libdestruct_device.so")
   print 'ok loaded'
   arg = DStructs().find("ClientArgument").newObject()
   #arg.address = "127.0.0.1"
   arg.address = "10.42.1.1"
   arg.port = 0xdff
   client = DStructs().find("Client").newObject(arg)
   device  = client.createRoot("DStream")
   #print device.size()
