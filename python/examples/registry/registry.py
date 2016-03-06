#!/usr/bin/python 
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
 #print key.keyName.name + " : "
 subKeysList = key.subkeys.list
 valuesList = key.values.list
 size = subKeysList.size()

 #print "\t\tValues(" + str(valuesList.size()) + ") :"
 for value in valuesList:
   #print value.name
   data = value.data()
   if data is not None:
     x = data.data()

 #print "\t\tSubkeys(" + str(subKeysList.size()) + ") :"
 for subKey in subKeysList:
    showKeys(subKey)

def findKey(key, path):
  subKeysList = key.subkeys.list
  size = subKeysList.size()
  for subKey in subKeysList:
     if subKey.name == path:
       return subKey.values.list
       print 'found'
  print "not found"
 
def usage():
   print "./registry -l localFilePath"
   print "./registry -c serverFilePath [server] [port]"

if __name__ == "__main__":
 if len(sys.argv) < 3:
   usage()
 else:
   registry = None
   loader = DStructs().find("Import").newObject()
   loader.file("../../../examples/modules/libdestruct_rpc.so")
   loader.file("../../../examples/modules/libregistry.so")
   print 'ok loaded'
   if sys.argv[1] == '-l':
      registry = DStructs().find("Registry") .newObject()
   elif sys.argv[1] == '-c':
     arg = DStructs().find("ClientArgument").newObject()
     arg.address = "127.0.0.1"
     arg.port = 0xdff
     if len(sys.argv) == 5:
       arg.address = sys.argv[3]
       arg.port = int(sys.argv[4])
     elif len(sys.argv) == 4:
       arg.address = sys.argv[3]
     client = DStructs().find("Client").newObject(arg)
     registry = client.findObject()
   else:
      usage()
   print registry
   regf = registry.open(sys.argv[2])
   print 'showKeys'
   key = regf
   showKeys(key)
   print 'end'
   #subkeys =regf.key.subkeys.list
   #for key in subkeys:
        #print key.keyName.keyName
        
   #values = findKey(regf.key, "MountedDevices")
   #if values:
     #for value in values:
        #print value.name.keyName

