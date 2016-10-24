#!/usr/bin/python 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

def callStr(count, performance):
  i = 0
  while i < count:
    res = performance.callStr("test")
    i += 1

def funcCallStr(count, performance):
   i = 0
   callStr = performance.callStr
   while i < count:
      res = callStr("test")
      i += 1

def stats(performance):
  for f in [callStr, funcCallStr]:
    start = time.time() 
    ncall = 10000
    f(ncall, performance)
    end = time.time()
    timesec = end - start
    print "Called " + str(f), ncall, "times in", timesec
    print ncall/timesec, " call per seconds"


def usage():
   print "./performance -l"
   print "./performance -c [server] [port]"

if __name__ == "__main__":
 if len(sys.argv) < 2:
   usage()
 else:
   registry = None
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpczmq.so")
   loader.file("../../../examples/modules/libdestruct_performance.so")
   print 'ok loaded'
   if sys.argv[1] == '-l':
      performance = DStructs().find("Performance") .newObject()
   elif sys.argv[1] == '-c':
     arg = DStructs().find("ClientArgument").newObject()
     arg.address = "127.0.0.1"
     arg.port = 0xdff
     auth  = DStructs().find("RPCAuth").newObject()
     auth.cert = "clicert/rpczmq_client_cert.txt" 
     auth.certStore = "clicert/"
     arg.auth = auth
     if len(sys.argv) == 4:
       arg.address = sys.argv[2]
       arg.port = int(sys.argv[3])
     elif len(sys.argv) == 3:
       arg.address = sys.argv[2]
     client = DStructs().find("Client").newObject(arg)
     serverLoader = client.generate("Import")
     if serverLoader.file("../modules/libdestruct_performance.so") == 0:
       serverLoader.file("destruct_performance.dll")
     performance = client.generate("Performance")
   else:
      usage()

   stats(performance)
