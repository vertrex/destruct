#!/usr/bin/python
import sys, os

sys.path.append('../../')

from _destruct import *

dstructs = DStructs()

class StreamArgument(DStruct):
  def __init__(self):
    DStruct.__init__(self, None, "StreamArgument")
    self.addAttribute(DAttribute("filePath" , DUnicodeString))
    self.addAttribute(DAttribute("input", DInt8))

def generateSerializer(name, mode, fileName):
  args = StreamArgument().newObject()
  args.input = mode
  args.filePath = os.getcwd() + "/" + fileName
  stream = dstructs.find("DStream").newObject(args)
  serializer = dstructs.find(name).newObject(stream)
  return serializer

def generateCoutSerializer(name):
   stream = dstructs.find("DStreamCout").newObject()
   serializer = dstructs.find(name).newObject(stream)
   return serializer

# Deserialize to Bin
def testObjectSerialization(dobject, fileName):
  ## Deserialize to Text
  serializer = generateSerializer("SerializeText", 0, fileName + ".text")
  serializer.DObject(dobject)

  serializer = generateCoutSerializer("SerializeText")
  serializer.DObject(dobject)

  serializer = generateSerializer("SerializeXML", 0, fileName + ".xml")
  serializer.DObject(dobject)

  serializer = generateCoutSerializer("SerializeXML")
  serializer.DObject(dobject)

  serializer = generateSerializer("SerializeBinary", 0, fileName + ".bin")
  serializer.DObject(dobject)

  serializer = generateSerializer("DeserializeBinary", 1, fileName + ".bin")
  return serializer.DObject()

def testDStructSerialization(dstruct, fileName):
  serializer = generateSerializer("SerializeText", 0, fileName + "Struct.text")
  serializer.DStruct(dstruct)

  serializer = generateSerializer("SerializeXML", 0, fileName + "Struct.xml")
  serializer.DStruct(dstruct)

  serializer = generateSerializer("SerializeBinary", 0, fileName + "Struct.bin")
  serializer.DStruct(dstruct)

  serializer = generateSerializer("DeserializeBinary", 1, fileName + "Struct.bin")
  return serializer.DStruct()

#
# Vector String Test
#
vectorString = dstructs.find("DVectorString").newObject()
for x in range(26):
  vectorString.push(chr(x + ord('A')) * (x + 1))

vector = testObjectSerialization(vectorString, "vector")
for x in vector:
  print x

#
# Complex Object Test
#
class ComplexObject(DStruct):
  def __init__(self):
    DStruct.__init__(self, None, "ComplexObject")
    self.addAttribute(DAttribute("name", DUnicodeString))
    self.addAttribute(DAttribute("number", DUInt64))
    self.addAttribute(DAttribute("object", DObject))

complex1 = ComplexObject().newObject()
complex2 = ComplexObject().newObject()
complex3 = ComplexObject().newObject()

complex1.name = "First object"
complex1.number = 1
complex1.object = complex2

complex2.name = "Second object"
complex2.number = 2
complex2.object = complex3

complex3.name = "Third object"
complex3.number = 3
complex3.object = vectorString

complexStruct = testDStructSerialization(ComplexObject(), "complex") #deserialize & register 
print dir(complexStruct.newObject())

complex = testObjectSerialization(complex1, "complex")
print complex.name, complex.number, complex.object
print complex.object.name, complex.object.number, complex.object.object
print complex.object.object.name, complex.object.object.number, complex.object.object.object
for x in complex.object.object.object:
  print x

#test streamstring (Must implem DBuffer in python and buffer size to know how much is bufferize and seek method too
ss = DStructs().find("DStreamString").newObject()
st  = DStructs().find("SerializeText").newObject(ss)
st.DObject(complex1)
s = st.stream
p = s.string()
print p
