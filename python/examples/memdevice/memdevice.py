#!/usr/bin/python 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

#if __name__ == "__main__":
loader = DStructs().find("Import").newObject()
loader.file("../../../examples/modules/libdestruct_rpczmq.so")
#loader.file("../../../examples/modules/libdestruct_memdevice.so")
print 'ok loaded'
arg = DStructs().find("ClientArgument").newObject()
#arg.address = "127.0.0.1"
arg.address = "10.42.1.195"
#arg.address = "10.42.1.1"
#arg.address = "10.42.1.2"
arg.port = 0xdff
arg.publicKeyPath = "cert/destruct_cert.txt"
print "connecting" 
client = DStructs().find("Client").newObject(arg)
serverLoader = client.generate("Import")
serverLoader.file("destruct_memdevice.dll")
memDriver = client.generate("MemoryDeviceDriver")

print "loading driver"
memDriver.load()
s = client.generate("MemoryDeviceStream")

b = ""
size = 0
MO = 1024*1024
GO = 1024 * MO
#sizeToRead = GO * 5
sizeToRead = 100*MO
buffSize = 10*MO #8192 #100000 #4096*25 #0#1*MO
print "Reading ", sizeToRead, " with buff of size ", buffSize 
timeStart = time.time()

read = s.read
while size < sizeToRead:
  b = read(buffSize)
  size += buffSize
totalTime = time.time() - timeStart
print 'speed ' , str(int(((sizeToRead) / totalTime)/ MO)) + ' MO' #in byte /sec
