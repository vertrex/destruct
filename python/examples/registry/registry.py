#!/usr/bin/python -i
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
 valueSize = valuesList.size() 

 if valueSize:
   print "\t\tValues(" + str(valueSize) + ") :"
   for index in range(0, valueSize):
     value = valuesList.get(index)
     print value.name.keyName

 if size:
   print "\t\tSubkeys(" + str(size) + ") :"
   for index in range(0, size):
     subKey = subKeysList.get(index)
     showKeys(subKey)

def findKey(key, path):
  subKeysList = key.subkeys.list
  valuesList = key.values.list
  size = subKeysList.size()
  valueSize = valuesList.size()
  for index in range(0, size):
     subKey = subKeysList.get(index)
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

   values = findKey(regf.key, "MountedDevices")
   if values:
     for index in range(0, values.size()):
        print values.get(index).name.keyName
   showKeys(regf.key)
