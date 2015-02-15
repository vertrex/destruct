#!/usr/bin/python 
import sys

sys.path.append('../')
sys.path.append('test/')

import unittest
from _destruct import DStruct, DStruct, DAttribute, DObject, DType, DInt8, DInt16, DInt32, DInt64, DUInt8, DUInt16, DUInt32, DUInt64, DUnicodeString
from _dtest import Test
import base64


class PyTest(unittest.TestCase):
  def test_CStruct(self):
     print """Search for C++ created DStruct"""
     d = DStructs()
     self.assertIsInstance(d.find(1), DStruct)
     #self.assertIsInstance(d.find("PrefetchXP"), None) #PrefetchXP is not registered

  def test_modify(self):
     print """Use a not yet initialized (Fix = 0) CPP create DStruct and add new attribute to it"""
     ms = DStructs().find("Modify")
     self.assertIsInstance(ms, DStruct)
     self.assertEqual(ms.attributeCount(), 3)

     self.assertEqual(ms.findAttribute('noset'), None)
     self.assertEqual(ms.attribute(3), None)
     self.assertIsInstance(ms.attribute(0), DAttribute)
                  
     for index in range(0, ms.attributeCount()):
        attr = ms.attribute(index)
        dtype = ms.attribute(index).dtype().getType()
     newAttr = DAttribute("pytype", DInt64)
     self.assertEqual(newAttr.dtype().getType(), DInt64)
     self.assertEqual(newAttr.name(), "pytype")
     newAttr = DAttribute("pytype2", DInt32)
     self.assertEqual(newAttr.dtype().getType(), DInt32)
     self.assertEqual(newAttr.name(), "pytype2")
     self.assertEqual(ms.attributeCount(), 3)
     self.assertEqual(ms.addAttribute(newAttr), None)
     self.assertEqual(ms.attributeCount(), 4)

  def test_dtype(self):
     print """Test DType and type specialization"""
     #Test numeric type
     self.assertEqual(DType(1).getType(), DInt16)
     self.assertEqual(DType(1).getType()(42), 42)
     self.assertEqual(DInt64(), long())
     self.assertEqual(DInt64(2) + 60, 62)
     self.assertEqual(DInt64(42).getType(), 3)
     self.assertIsInstance(DInt64(), long)
     #Test string type
     self.assertEqual(DUnicodeString(), "")
     self.assertEqual(DUnicodeString('a') * 2, 'aa')
     #Test DObject (pointer) type
     n = DStruct(None, "TestObject")
     n.addAttribute(DAttribute("parent", DObject))
     n.addAttribute(DAttribute("toto", DInt8))
     
     pobj = n.newObject()
     pobj.toto = 1
     self.assertEqual(pobj.toto, 1)       
     pobj.parent = None
     self.assertEqual(pobj.parent, None)
     pobj.parent = pobj
     self.assertIsInstance(pobj.parent, DObject)
     pobj.parent = None 
     self.assertEqual(pobj.parent, None)

  def test_CPPcasting(self):
     print """Special case for numeric object who is casted once in c++ but not in python !"""
     casterStruct = DStruct(None, "Caster")
     casterStruct.addAttribute(DAttribute("signed_byte", DInt8))
     caster = casterStruct.newObject()     

     cast = DInt8(256+42)
     #we 're still in python ! 
     self.assertEqual(cast, 256 + 42)

     #caster.signed_byte is set as a C++ real_value ! as internally we create a C++ real value object and don't keep tracks of PyObject
     caster.signed_byte = cast
     self.assertEqual(caster.signed_byte, 42)
     caster.signed_byte = 255
     self.assertEqual(caster.signed_byte, -1)

  def test_createDStruct(self):
     print """Create a pure python DStruct"""
     pyps = DStruct(None, "PyNtfsBase")
     pyps.addAttribute(DAttribute("mbr", DInt64))
     pyps.addAttribute(DAttribute("boot", DInt32))
     pyps.addAttribute(DAttribute("info", DUnicodeString))
     pyps.addAttribute(DAttribute("parent", DObject))
     #show_attribute(pyps)
     destruct = DStructs()
     destruct.registerDStruct(pyps)
     PyNtfsBaseStruct = destruct.find("PyNtfsBase")
     self.assertIsInstance(PyNtfsBaseStruct, DStruct)
     self.assertEqual(PyNtfsBaseStruct.attributeCount(), 4)

  def test_generateObject(self):
     print """Create a pure python object from a python DStruct"""
     dstruct = DStructs().find("PyNtfsBase")
     self.assertEqual(dstruct.name(), "PyNtfsBase")
     obj = dstruct.newObject()
     self.assertIsInstance(obj, DObject)
     self.assertEqual(obj.instanceOf().name(), "PyNtfsBase")

     obj.setValue("mbr", 666)
     value = obj.getValue("mbr")
     self.assertEqual(value, 666)

     obj.setValue("boot", DInt32(998))
     value = obj.getValue("boot")
     self.assertEqual(value, 998)

     obj.setValue("info", "It's a kind of magic object") 
     obj.getValue("info")
     self.assertEqual(obj.getValue("info"), "It's a kind of magic object")
    
     value = obj.getValue("parent")
     self.assertEqual(value, None)

     self.assertEqual(obj.mbr , 666)
     self.assertEqual(obj.boot, 998)
     self.assertEqual(obj.info, "It's a kind of magic object")
     self.assertEqual(obj.parent, None)

     n = DStruct(None, "Node")
     n.addAttribute(DAttribute("size", DInt64))
     n.addAttribute(DAttribute("parent", DObject))
 
     parentObj = n.newObject()
     parentObj.setValue("size", 11232)
      
     obj.setValue("parent", parentObj)
   
     self.assertEqual(obj.getValue("parent"), parentObj)
     self.assertEqual(obj.parent, parentObj)
     self.assertIsInstance(obj.getValue("parent"), DObject)
     self.assertIsInstance(obj.parent, DObject)
     return obj

  def test_generateCPynested(self):
     print """Create a nested object from C++ definition. Test for garbage collection"""
     dstruct = DStructs().find("Nested")
     nested = dstruct.newObject()
     nested.setValue("NestedStart", 123)
     nested.setValue("NestedEnd", "End of object")
     obj = self.test_generateObject()
     nested.setValue("ObjectNested", obj)
     self.assertEqual(nested.ObjectNested.parent.size, 11232)

  def test_toset(self):
     print """Create a python object from a Python DStruct, and set Value of object in c++"""
     print "dstruct = DStruct(None, \"ToSet\")"
     dstruct = DStruct(None, "ToSet")
     print "dstruct.addAttribute(DAttribute(\"num\", DInt64))"
     dstruct.addAttribute(DAttribute("num", DInt64))
     dstruct.addAttribute(DAttribute("text", DUnicodeString))
     dstruct.addAttribute(DAttribute("parent", DObject))
     print "DStructs().registerDStruct(dstruct)"
     DStructs().registerDStruct(dstruct)
     sobj = dstruct.newObject()
     print "Test().setObjectValue(sobj)"
     Test().setObjectValue(sobj)

     print "self.assertEqual(sobj.num, 424242)"
     self.assertEqual(sobj.num, 424242)
     self.assertEqual(sobj.text, "My text.")
     self.assertEqual(sobj.parent.num, 414141)
     self.assertEqual(sobj.parent.text, "My parent object")

  def test_getObjectValue(self):
     print """Get a c++ dobject and check value and call C++ function from python"""
     obj = Test().getObjectValue()
     print obj.instanceOf().name()
     self.assertEqual(obj.instanceOf().name(), "CallMe")
     self.assertEqual(obj.func, "call me baby !")

  def callback(self):
     print """define a python callback"""
     return "You've called python !"

  #XXX OBSOLETE no callback anymore object can have member function
  #def test_Callback(self):
     #print """test calling python callaback on python object from python and c++"""
     #ds = DStruct(None, "CallMePy")
     #ds.addAttribute(DAttribute("pyfunc", DUnicodeString))
     #df = ds.newObject()
     #df.replaceValue("pyfunc", self.callback)
     #self.assertEqual(df.pyfunc, "You've called python !")
     #Test().showObjectAttribute(df)

  def Obsolete_PyClass(self): #XXX surement obsolete !
  #def test_PyClass(self):
     print """Create Python nested object/class and use callback to get object path and decode data"""
     ns = DStruct(None, "Node")
     ns.addAttribute(DAttribute("name", DUnicodeString))
     ns.addAttribute(DAttribute("absolute", DUnicodeString))
     ns.addAttribute(DAttribute("parent", DObject))
     DStructs().registerDStruct(ns)

     fns = DStruct(ns, "FatNode") #DStruct("Node", "FatNode") not implemtend yet  
     fns.addAttribute(DAttribute("offset", DInt64))
     fns.addAttribute(DAttribute("data", DUnicodeString))
     fns.addAttribute(DAttribute("dataDecode", DUnicodeString))
     DStructs().registerDStruct(fns)

     class RootNode(DObject):
       def __init__(self):
          DObject.__init__(self, "Node")
          self.name = ""
          self.absolute = ""

     class FatNode(DObject):
       def __init__(self, parent, name):
          DObject.__init__(self, "FatNode")
          self.name = name
          self.offset = 2
          self.parent = parent
          self.data = "c2FsdXQK"
          #self.replaceValue("absolute", self._absolute)
          self.absolute = self._absolute
          self.dataDecode = self.decode

       def decode(self):
          try:
            return base64.b64decode(self.data)[0:-1]
          except:
            return self.data

       def _absolute(self):
          return self.parent.absolute + "/" + self.name
 
       def compute(self):
          return self.offset * 2

     root = RootNode()
     documents = FatNode(root, "documents") 
     pdf = FatNode(documents, "pdf")
     parent = FatNode(pdf, "toto.pdf")

     for i in range(0, 3):
       parent = FatNode(parent, "name" + str(i))
        
     self.assertEqual(parent.absolute, '/documents/pdf/toto.pdf/name0/name1/name2')
     self.assertEqual(parent.dataDecode, "salut")
        
if __name__ == "__main__":
  """Run C++ test class this initialize some struct and basic things"""
  t = Test()
  t.run()

  """Run python test class"""
  unittest.main()
