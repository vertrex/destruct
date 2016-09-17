#!/usr/bin/python -i 
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

if __name__ == "__main__":
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpczmq.so")
   loader.file("../../../examples/modules/libdestruct_device.so")
   print 'ok loaded'
   arg = DStructs().find("ClientArgument").newObject()
   arg.address = "127.0.0.1"
   #arg.address = "10.42.1.195"
   arg.port = 0xdff
   client = DStructs().find("Client").newObject(arg)
   print "generate devicelist" 
   serverLoader = client.generate("Import")
   print "call server load"
   serverLoader.file("../modules/libdestruct_device.so")
   deviceList = client.generate("DeviceList")
   devs = deviceList.list()
   for dev in devs:
     print dev.blockDevice, dev.serialNumber, dev.model, dev.size
