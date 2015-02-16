#!/usr/bin/python 
import sys

sys.path.append('../')
sys.path.append('../../')
sys.path.append('../loader')

from _destruct import *
from _loader import *

loader = Loader()
loader.loadFile("/home/vertrex/destruct/examples/inheritance/libdestruct_inherit.so")

##for x in range(0, DStructs().count()):
  #print DStructs().find(x)

#load cpp iterator exemple and inherite from cpp class
#DStructs.find("DVectorString")

class PyB(DStruct):
  def __init__(self):
    DStruct.__init__(self, DStructs().find("CppA"), "PyCppAB")
    #DStruct.__init__(self, DNone, "PyCppAB")
    self.addAttribute(DAttribute("text3", DUnicodeString))
    #DStructs().registerDStruct(self)

print PyB()
DStructs().registerDStruct(PyB())

pyCppAB = DStructs().find("PyCppAB").newObject()

print pyCppAB

print dir(pyCppAB)

print pyCppAB.callable1()
print pyCppAB.object1
print pyCppAB.object2
print pyCppAB.text1
pyCppAB.text1 = "Salut"
print pyCppAB.text1
print pyCppAB.text2
print pyCppAB.text3

