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

args = StreamArgument().newObject()

# Deserialize to Bin
def testObjectSerialization(dobject, fileName):
  ## Deserialize to Text
  args.filePath = os.getcwd() + "/" + fileName + ".text"
  args.input = 0
  print "Serializing to text content of file " + args.filePath
  streamText = dstructs.find("DStream").newObject(args)
  serializeText = dstructs.find("SerializeText").newObject(streamText)
  serializeText.DObject(dobject)

  args.filePath = os.getcwd() + "/" + fileName +".bin"
  print "Deserializing to binary content of file " + args.filePath
  streamBin = dstructs.find("DStream").newObject(args)
  serializeBinary = dstructs.find("SerializeBinary").newObject(streamBin)
  serializeBinary.DObject(dobject)
  serializeBinary = None
  streamBin = None

  args.filePath = os.getcwd() + "/" + fileName + ".bin"
  print "Deserializing content of file " + args.filePath
  args.input = 1
  streamIn = dstructs.find("DStream").newObject(args)
  deserializeBinary = dstructs.find("DeserializeBinary").newObject(streamIn)
  dobject = deserializeBinary.DObject()
  return dobject
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
dstructs.registerDStruct(ComplexObject())

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

complex = testObjectSerialization(complex1, "complex")
print complex.name, complex.number, complex.object
print complex.object.name, complex.object.number, complex.object.object
print complex.object.object.name, complex.object.object.number, complex.object.object.object
for x in complex.object.object.object:
  print x

#XXX Test Destruct deserialization
