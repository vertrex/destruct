#!/usr/bin/python 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

#if __name__ == "__main__":
loader = DStructs().find("Import").newObject()
loader.file("../../../examples/modules/libdestruct_rpczmq.so")
loader.file("../../../examples/modules/libdestruct_device.so")
print 'ok loaded'
arg = DStructs().find("ClientArgument").newObject()
#arg.address = "127.0.0.1"
#arg.address = "127.0.0.1"
#arg.address = "10.42.1.197"
arg.address = "10.42.1.195"
#arg.address = "10.42.1.1"
arg.port = 0xdff
client = DStructs().find("Client").newObject(arg)
print "generate devicelist" 
serverLoader = client.generate("Import")
print "call server load"
if serverLoader.file("../modules/libdestruct_device.so") == 0:
  serverLoader.file("destruct_device.dll")
deviceList = client.generate("DeviceList")
devs = deviceList.list()
for dev in devs:
  print dev.blockDevice, dev.serialNumber, dev.model, dev.size

s = devs[0].open()
b = ""
size = 0
MO = 1024*1024
GO = 1024 * MO
sizeToRead = GO
timeStart = time.time()
while size < sizeToRead:
  b = s.read(10*MO)
  size += 10*MO 
totalTime = time.time() - timeStart
print 'speed ' , str(int( ((sizeToRead) / totalTime)/ MO)) + ' MO' #in byte /sec

  #print size
#print len(b)
