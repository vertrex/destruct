#!/usr/bin/python 
import sys

#sys.path.append('../')
sys.path.append('../../')
sys.path.append('../test/')

import time, timeit
from _dtest import *
from _destruct import *

COUNT = 10
COUNT = 10**6

t = Test()

def timeFunc(func, args):
   a = time.time()
   func(args)
   b = time.time()
   print str(func) + " : "+  str(b-a)
   return b-a

def total(times):
   x = 0
   for i in times:
     x += i
   print 'Total time ' + str(x)      

def fill(dobject):
   for i in range(0, COUNT):
      dobject.push(str(i))

def iterate(dobject):
   for i in dobject:
      s = i

class PySimpleIterator(DObject):
  def __init__(self):
     DObject.__init__(self, 'DIterator') 

class PySimpleDVectorString(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<String>")

class PyDIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIterator")
     self.i = 0  

  def first(self):
     self.i = 0
 
  def next(self):
     self.i += 1

  def isDone(self):
     #print 'pyIterator isDone'
     if self.i >= self.pyvector.size():
       return 1
     return DInt8(0) 

  def currentItem(self):
     #print 'currentItem' 
     if self.i < self.pyvector.size():
       val = self.pyvector.get(self.i)
       return val

  def setIterable(self, item):  
     self.pyvector = item
     pass
      
class PythonDVector(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<String>")
     self.l = []

  def get(self, index):
     return DUnicodeString('a')
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      #print len(self.l)
      return len(self.l)

  def iterator(self): #XXX si on la redefinie pas on peut psa utiliser literator c++ avec ce COntainer python car il faut qu il pointe sur ceux DObject c a corriger ds py_dobject.cpp eveidement c a lui de retourner un iterator qui pointe sur le pyobject et c mes thodes car par default il va retourner la methode du dobject c++  la parent pas la virtuel re donc voir ausis ds cpp object 
     iterator = PyDIterator()
     iterator.setIterable(self)
     return iterator
#

print "======================"
print "------ Python call c++ via class wrapper -----"

vector = PySimpleDVectorString()
a = timeFunc(fill, vector)
b = timeFunc(iterate, vector)
total((a, b,))
print "------ Python call python via ds ----"

pi = PythonDVector()
a = timeFunc(fill, pi)
b = timeFunc(iterate, pi)
total((a, b,))

class PyReverseIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIterator")
     self.i = 0  
#XXX comment j appelle les fonctions parent, deja ? :)
  def first(self):
     self.i = 0
 
  def next(self):
     #self.i -- ?
     self.i += 1

  #def isDone(self):
     ##print 'pyIterator isDone'
     #if self.i >= self.pyvector.size():
       #return 1
     #return DInt8(0) 
#
  #def currentItem(self):
     ##print 'currentItem' 
     #if self.i < self.pyvector.size():
       #val = self.pyvector.get(self.i)
       #return val
#
  #def setIterable(self, item):  
     #self.pyvector = item
     #pass
     # 
#class PythonDVector(DObject):
  #def __init__(self):
     #DObject.__init__(self, "DVector<String>")
     #self.l = []
#
  #def get(self, index):
     #return DUnicodeString('a')
     #return self.l[index] 
#
  #def push(self, val):
     #self.l.append(val)
#
  #def size(self):
      ##print len(self.l)
      #return len(self.l)

 
#print "----- Real pure python call -----"
#
#pypy = PyPy()
#
#print "------ Python / CPP object func ----"
#
#timeFunc(pi.returnObject, None)
#timeFunc(pi.callObject, pi)
