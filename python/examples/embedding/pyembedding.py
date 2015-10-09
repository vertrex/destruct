import sys
sys.path.append("../../")
from _destruct import *

class PythonDStruct(DStruct):
  def __init__(self):
    DStruct.__init__(self, DNone, "PythonDStruct")
    self.addAttribute(DAttribute("size", DInt64))
    self.addAttribute(DAttribute("name", DUnicodeString))

def getPythonObject():
  struct = PythonDStruct()
  DStructs().registerDStruct(struct)
  dobject = struct.newObject()
  dobject.name = "Python object"
  dobject.size = 64
  return dobject

def showObject(dobject):
  print 'python show object of type ' + str(dobject)
  print 'dobject.name => ', dobject.name
  print 'dobject.size => ', dobject.size
