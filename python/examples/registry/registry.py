#!/usr/bin/python 
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *
from _registryrpc import *

def showStruct(dobject):
  struct = dobject.instanceOf()
  print struct.name(), " attributes : "
  for index in range(struct.attributeCount()):
    print "\t\t" + struct.attribute(index).name()

def showKeys(key):
 print key.keyName.keyName + " : "
 subKeysList = key.subkeys.list
 valuesList = key.values.list
 size = subKeysList.size()

 print "\t\tValues(" + str(valuesList.size()) + ") :"
 for value in valuesList:
   print value.name.keyName

 print "\t\tSubkeys(" + str(subKeysList.size()) + ") :"
 for subKey in subKeysList:
    showKeys(subKey)

def findKey(key, path):
  subKeysList = key.subkeys.list
  size = subKeysList.size()
  for subKey in subKeysList:
     if subKey.keyName.keyName == path:
       return subKey.values.list
       print 'found'
  print "not found"
 

if __name__ == "__main__":
 if len(sys.argv) < 2:
   print "./registry serverFilePath [server] [port]"
 else:
   registry = None
   registryRPC = RegistryRPC()
   if len(sys.argv) == 4:
     registry = registryRPC.connect(sys.argv[2], int(sys.argv[3]))
   elif len(sys.argv) == 3:
     registry = registryRPC.connect(sys.argv[2], 0xdff)
   else:
     registry = registryRPC.connect("127.0.0.1", 0xdff)
   regf = registry.open(sys.argv[1])
  
   #subkeys =regf.key.subkeys.list
   #for key in subkeys:
        #print key.keyName.keyName
        
   #values = findKey(regf.key, "MountedDevices")
   #if values:
     #for value in values:
        #print value.name.keyName

   showKeys(regf.key)
