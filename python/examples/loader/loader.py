#!/usr/bin/python -i 
import sys
import os

sys.path.append("../")
sys.path.append("../../")

from _destruct import *
#from _loader import *

class FindKeyStruct(DStruct):
  def __init__(self):
    DStruct.__init__(self, None, "FindKey")
    DStructs().registerDStruct(self)
    self.addAttribute(DAttribute("keyName", DMethod, DUInt64, DUInt64))


class FindKey(DObject):
  def __init__(self):
    DObject.__init__(self, "FindKey")

  def keyName(self, index):
    for x in xrange(index * 2500, (index + 1) * 2500):
      print "key index : " + str(x)
    return x

def registryShow():
  FindKeyStruct()
  finder = FindKey()
  registry = DStructs().find("Registry").newObject()
  regf = registry.open("/home/vertrex/dump/registry/system")
  regfDebug = DStructs().find("TraceObject").newObject(regf)

  for key in regf.key.subkeys.list:
    print key.time
    print key.keyName.keyName


def workPool():
  workerPool = DStructs().find("WorkerPool").newObject(DUInt8(4))
  taskStruct = DStructs().find("Task")
  for x in range(0, 4): 
    task = taskStruct.newObject()
    task.function = finder.keyName
    task.argument = DUInt64(x) 
    workerPool.addTask(task)

  workerPool.join()

def showStructs():
  for x in range(0, DStructs().count()):
    print DStructs().find(x)


def serve():
  print "create server" 
  server = DStructs().find("Server").newObject(DUInt32(3583))
  registry = DStructs().find("Registry").newObject()

  #print "create registry object tree"
  #regf = registry.open("/home/vertrex/dump/registry/system")
  #init etc..
  print dir(server)
  server.addRoot(registry)
  server.daemonize()
  
def connect():
  print "connecting"
  arg = DStructs().find("ClientArgument").newObject()
  arg.port = 3583
  arg.address = "127.0.0.1"
  arg.publicKeyPath = "cert/destruct_cert.txt"
  client = DStructs().find("Client").newObject(arg)
  print client

  print "client.findObject"
  registry = client.findObject() #registry ? #root 0

  print dir(registry)
  print "opening registry"
  regf = registry.open("/home/vertrex/dump/registry/system")
  return regf


def loadPath(pathname):
  loader = DStructs().find("Import").newObject()
  for name in pathname:
    if os.name == 'nt':
      absolute = baseDir + "/Release/" + name + ".dll"
      absolute = os.path.normpath(absolute)
    else:
      absolute = baseDir + "/lib" + name + ".so"
    loader.file(absolute)

def testLoading(pathname):
  loadPath(pathname)

  sa = DStructs().find("SimpleA").newObject() 
  print dir(sa)
  print sa.object1
  if sa.object1:
    print 'sa.object1 exists'
  else:
    print 'sa.object1 is none'

  sa.object1 = DStructs().find("DVectorString").newObject()
  print sa.object1
  if sa.object1:
    print 'sa.object1 exists'
  else:
    print 'sa.object1 is none'


  vs = DStructs().find("DVectorString").newObject()
  #server = DStructs().find("Server").newObject(DUInt32(3583))
  #server.addRoot(vs)

if os.name == 'nt': 
  baseDir = os.path.expanduser("~") + "/destruct-build/examples/modules/" 
else:
  baseDir = os.path.expanduser("~") + "/destruct/examples/modules/" 

if __name__ == "__main__":
  print "found baseDir" + baseDir

  pathname = ["destruct_test",
	      "destruct_threading",
	      "destruct_inherit",
              "destruct_rpc",
              "destruct_registry",
	     ]

  testLoading(pathname) 


