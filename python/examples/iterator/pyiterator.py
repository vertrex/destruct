#!/usr/bin/python 
import sys

#sys.path.append('../')
sys.path.append('../../')

import time, timeit
from _destruct import *

COUNT = 10
COUNT = 10**6
#COUNT = 10**3

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
   l = []
   print 'will iteratte on ', len(dobject)
   for i in dobject:
      #print i, type(i)
      #print i, type(i)
      s = i
      l.append(s)
   print 'have iterate on ' + str(len(l))

class PySimpleIterator(DObject):
  def __init__(self):
     DObject.__init__(self, 'DIterator') 

class PySimpleDVectorInt(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<DInt64>")


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
     if self.i >= self.container.size():
       return 1
     return 0 

  def currentItem(self):
     #print 'currentItem' 
     if self.i < self.container.size():
       val = self.container.get(self.i)
       return val

  def container(self, item):  
     self.container = item
     return self.container
      
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
      return len(self.l)

  #def container(self):
     #return self
  #def iterator(self):
     #iterator = PyDIterator()
     #iterator.container = self
     #return iterator
#
class PyReverseIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIterator")
  #def first(self):
     #self.index = self.container.size() - 1 #XXX else segfault donc check ds la template 
     #print 'first end'

  #def next(self):
     ##print 'next'
     #self.index = self.index - 1 #XXX bien l attribut et c ok !
#
  #def isDone(self):
     #print 'reverse is done'
     ##print 'isDone', self.index, self.container().size()
     #if self.index == 0:
       #return 1
     #return 0

  def currentItem(self): #XXX ca serait bien de pouvoir le faire faut pas detruire la fonction parent quand une nouvelle est aloeur par un fils alors ? pour gerer l heritage ? 
     print 'Reverse currentItem'
     print self.container
     print self.container.size() - self.index
     return self.container.get(DUInt64(0))


#XXX faudrait le call en C++ ! 

class PyPureIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "PyPureIterator")
     self.i = 0  
     self.container = None

  def first(self):
     self.i = 0
 
  def next(self):
     self.i += 1

  def isDone(self):
     if self.i >= self.container.size():
       return DInt8(1)
     return DInt8(0) 

  def currentItem(self):
     if self.i < self.container.size():
       val = self.pyvector.get(self.i)
       return val


class PythonPureIterable(DObject):
  def __init__(self):
     DObject.__init__(self, "DContainer") ##Dcequ on veut en faitef
     self.l = ['a', 'b', 'c']

  def get(self, index):
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      return len(self.l)

  def iterator(self): 
     iterator = PyPureIterator()
     iterator.container = self
     return iterator


def dvectorstring():
  print "------ Python create c++ object via Destruct --"
  cs = Destruct().find('DVector<String>')
  c = cs.newObject()
  a = timeFunc(fill, c)
  b = timeFunc(iterate, c)
  total((a, b,))

def simplevectorstring():
  print "------ Python call c++ via empty inherited class-----"
  vector = PySimpleDVectorString()
  a = timeFunc(fill, vector)
  b = timeFunc(iterate, vector)
  total((a, b,))

def pythonvector():
  print "------ Python call python herited class ----"
  pi = PythonDVector()
  a = timeFunc(fill, pi)
  b = timeFunc(iterate, pi)
  total((a, b,))

def simplevectorint():
  print "------ DINT64 Test  Python create c++ object via Destruct --"
  cs = Destruct().find('DVector<Int32>')
  c = cs.newObject()
  a = timeFunc(fillInt, c)
  b = timeFunc(iterate, c)
  total((a, b,))

def reverseiterate(): 
  print 'Push 0,10 in simpleDvectorSString'
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 

  print 'create reverse iterator'
  iterator = PyReverseIterator()
  print 'iterator set container'
  iterator.container = vector
  #print 'iterate reverse'
  for i in iterator:
    print i

   #pass
  print 'for in len'
  for x in range(0, len(vector)): #implem len
    print vector[x]


def setvector():
  #XXX vector = Vector
  print vector[0], type(vector[0])
  print 'vector set[0]'
  vector[0] = '100 strings'
  print vector[0]

  print 'vector set 0 10 str'
  for x in range(0, 10):
    print x
    vector[x] = 'test ' +str(x)

  print 'print vector content'
  for i in vector:
    print i

def pushint():
 vi = PySimpleDVectorInt()
 for x in range(0, 10):
   vi.push(x)
 for x in range(0, 10):
    vi[x] = x * 10
 for i in vi:
    print i

 print 'put int in string doit planter mais doit etre catchable'
  #try:
 #  for x in range(0, 10):
   #  vector[x] = x
   #except :
    #pass
def pythonpureiterable():
  ppi = PythonPureIterable()
  for x in ppi:
    print x

print "=" * 80

dvectorstring()
simplevectorstring()
pythonvector()
simplevectorint()
reverseiterate()
#setvector()
#pushint()
#pythonpureiterable()

