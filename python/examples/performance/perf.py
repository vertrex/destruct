#!/usr/bin/python 
import sys
import time

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

def stats(performance):
  start = time.time() 
  ncall = 100000
  i = 0 
  while i < ncall:
    res = performance.callStr("test")
    i += 1
  end = time.time()
  timesec = end - start
  print "Call DUnicodeString callStr('test') ", ncall, "times in", timesec
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
