#!/usr/bin/python -i 
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

def showStruct(dobject):
  struct = dobject.instanceOf()
  print struct.name(), " attributes : "
  for index in range(struct.attributeCount()):
    print "\t\t" + struct.attribute(index).name()

def showKeys(key):
 print key.keyName.name + " : "
 subKeysList = key.subkeys.list
 valuesList = key.values.list
 size = subKeysList.size()

 print "\t\tValues(" + str(valuesList.size()) + ") :"
 for value in valuesList:
   print value.name

 print "\t\tSubkeys(" + str(subKeysList.size()) + ") :"
 for subKey in subKeysList:
    showKeys(subKey)

def findKey(key, path):
  subKeysList = key.subkeys.list
  size = subKeysList.size()
  for subKey in subKeysList:
     if subKey.keyName.name == path:
       return subKey.values.list
       print 'found'
  print "not found"
 

if __name__ == "__main__":
 if len(sys.argv) < 2:
   print "./registry serverFilePath [server] [port]"
 else:
   registry = None
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpc.so")
   loader.file("../../../examples/modules/libregistry.so")
   arg = DStructs().find("ClientArgument").newObject()
   arg.address = "127.0.0.1"
   arg.port = 0xdff
   if len(sys.argv) == 4:
     registry = registryRPC.connect(sys.argv[2], int(sys.argv[3]))
     arg.address = sys.argv[2]
     arg.port = int(sys.argv[3])
   elif len(sys.argv) == 3:
     registry = registryRPC.connect(sys.argv[2], 0xdff)
     arg.address = sys.argv[2]
   client = DStructs().find("Client").newObject(arg)
   registry = client.findObject() #registry is h
   regf = registry.open(sys.argv[1])
  
   showKeys(regf.key)
   #subkeys =regf.key.subkeys.list
   #for key in subkeys:
        #print key.keyName.keyName
        
   #values = findKey(regf.key, "MountedDevices")
   #if values:
     #for value in values:
        #print value.name.keyName

