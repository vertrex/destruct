#!/usr/bin/python 
import sys

#sys.path.append('../')
sys.path.append('../../')

import time, timeit
from _destruct import *

#COUNT = 10
COUNT = 10**6

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

def fillInt(dobject):
   for i in range(0, COUNT):
      dobject.push(DInt32(i))


def iterate(dobject):
   for i in dobject:
      #print i, type(i)
      s = i

class PySimpleIterator(DObject):
  def __init__(self):
     DObject.__init__(self, 'DIteratorString') 

class PySimpleDVectorString(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<String>")

class PyDIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIteratorString")
     self.i = 0  

  def first(self):
     self.i = 0
 
  def next(self):
     self.i += 1

  def isDone(self):
     #print 'pyIterator isDone'
     if self.i >= self.pyvector.size():
       return DInt8(1)
     return DInt8(0) 

  def currentItem(self):
     #print 'currentItem' 
     if self.i < self.pyvector.size():
       val = self.pyvector.get(self.i)
       return val

  def container(self, item):  
     self.pyvector = item
     return self.pyvector
      
class PythonDVector(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<String>")
     self.l = []

  def get(self, index):
     #return DUnicodeString('a')
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      #print len(self.l)
      return len(self.l)

  def iterator(self): #XXX si on la redefinie pas on peut psa utiliser literator c++ avec ce COntainer python car il faut qu il pointe sur ceux DObject c a corriger ds py_dobject.cpp eveidement c a lui de retourner un iterator qui pointe sur le pyobject et c mes thodes car par default il va retourner la methode du dobject c++  la parent pas la virtuel re donc voir ausis ds cpp object 
     iterator = PyDIterator()
     iterator.container(self)
     return iterator
#
class PyReverseIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIteratorString")

  def first(self):
     self.index = self.container().size() - 1 #XXX else segfault donc check ds la template 
     #print 'first end'

  def next(self):
     #print 'next'
     self.index = self.index - 1 #XXX bien l attribut et c ok !

  def isDone(self):
     #print 'isDone', self.index, self.container().size()
     if self.index == 0:
       return 1
     return 0

  #def currentItem(self): XXX ca serait bien de pouvoir le faire faut pas detruire la fonction parent quand une nouvelle est aloeur par un fils alors ? pour gerer l heritage ? 
     #return DObject.currentItem(self, self.size() - index) #:)

  def iterator(self):
     #print 'iterator self return self'
     return self 


print "======================"

print "------ Python create c++ object via Destruct --"
cs = Destruct().find('DVector<String>')
c = cs.newObject()
a = timeFunc(fill, c)
b = timeFunc(iterate, c)
total((a, b,))

print "------ Python call c++ via empty inherited class-----"
vector = PySimpleDVectorString()
a = timeFunc(fill, vector)
b = timeFunc(iterate, vector)
total((a, b,))

print "------ Python call python herited class ----"
pi = PythonDVector()
a = timeFunc(fill, pi)
b = timeFunc(iterate, pi)
total((a, b,))

print "------ Int Test  Python create c++ object via Destruct --"
cs = Destruct().find('DVector<Int32>')
c = cs.newObject()
a = timeFunc(fillInt, c)
b = timeFunc(iterate, c)
total((a, b,))

print 'Reverse iterator'
vector = PySimpleDVectorString()
for i in range(0, 10):
  vector.push(str(i)) 

iterator = PyReverseIterator()
iterator.container(vector)
print 'iterate reverse'
for i in iterator:
  print i

print len(vector)
for x in range(0, len(vector)): #implem len
  print vector[x]


