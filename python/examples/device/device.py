#!/usr/bin/python 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

loader = DStructs().find("Import").newObject()
loader.file("../../../examples/modules/libdestruct_rpczmq.so")
#print 'ok loaded'
arg = DStructs().find("ClientArgument").newObject()
auth  = DStructs().find("RPCAuth").newObject()
auth.cert = "clicert/rpczmq_client_cert.txt" 
auth.certStore = "clicert/"
#arg.address = "127.0.0.1"
#arg.address = "127.0.0.1"
#arg.address = "10.42.1.197"
arg.address = "10.42.1.195"
#arg.address = "10.42.1.1"
#arg.address = "10.42.1.2"
arg.port = 0xdff
arg.auth = auth
print "connecting" 
client = DStructs().find("Client").newObject(arg)
print "generate devicelist" 
serverLoader = client.generate("Import")
if serverLoader.file("../modules/libdestruct_device.so") == 0:
  serverLoader.file("destruct_device.dll")
deviceList = client.generate("DeviceList")
devs = deviceList.list()
for dev in devs:
  print dev.blockDevice, "-", dev.serialNumber,  "-", dev.model, dev.size

s = devs[0].open()
b = ""
size = 0
MO = 1024*1024
GO = 1024 * MO
#sizeToRead = GO * 5
sizeToRead = 1000*MO
buffSize = 8192 #100000 #4096*25 #0#1*MO
print "Reading ", sizeToRead, " with buff of size ", buffSize 
timeStart = time.time()

read = s.read
while size < sizeToRead:
  b = read(buffSize)
  size += buffSize
totalTime = time.time() - timeStart
print 'speed ' , str(int(((sizeToRead) / totalTime)/ MO)) + ' MO' #in byte /sec

  #print size
#print len(b)
