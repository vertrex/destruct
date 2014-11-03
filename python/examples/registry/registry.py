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

registryRPC = RegistryRPC()
registry = registryRPC.connect()
regf = registry.open("/home/vertrex/dump/registry/system")

showKeys(regf.key)
