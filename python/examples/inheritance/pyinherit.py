#!/usr/bin/python 
import sys

sys.path.append('../')
sys.path.append('../../')

from _destruct import *

loader = DStructs().find("Import").newObject()
loader.file("/home/vertrex/destruct/examples/modules/libdestruct_inherit.so")

##for x in range(0, DStructs().count()):
  #print DStructs().find(x)

#load cpp iterator exemple and inherite from cpp class
#DStructs.find("DVectorString")

class PyBStruct(DStruct):
  def __init__(self):
    DStruct.__init__(self, DStructs().find("CppA"), "PyCppAB")
    #DStruct.__init__(self, DNone, "PyCppAB")
    self.addAttribute(DAttribute("text3", DUnicodeString))
    self.addAttribute(DAttribute("object3", DUnicodeString))
    self.addAttribute(DAttribute("changeA", DMethod, DNone, DNone))
    self.addAttribute(DAttribute("callA", DMethod, DNone, DNone))
DStructs().registerDStruct(PyBStruct())

class PyB(DObject):
  def __init__(self):
    DObject.__init__(self, "PyCppAB")
    
  def changeA(self):
    print "changeA(void) getValue text1:", self.text1
    self.text1 = "changed by changeA"

  def callA(self):
    self.callable1()

print PyB()

#pyCppAB = DStructs().find("PyCppAB").newObject()

#print pyCppAB
#
#print dir(pyCppAB)

pyCppAB = PyB()

print '.callable1() ', pyCppAB.callable1()
print '.object1 ', pyCppAB.object1
print '.object2 ', pyCppAB.object2
print '.object3 ', pyCppAB.object3
print '.text1 ', pyCppAB.text1
print '.text1 = Salut'
pyCppAB.text1 = "Salut"
print '.text1 ', pyCppAB.text1
print '.text2 ', pyCppAB.text2
print '.text3 ', pyCppAB.text3

print '.text2 = reset'
pyCppAB.text2 = 'reset'
print '.changeA', pyCppAB.changeA()
print '.text1', pyCppAB.text1
print '.callA', pyCppAB.callA()
print '.text2 ', pyCppAB.text2
