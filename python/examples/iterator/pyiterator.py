#!/usr/bin/python 
import sys

#sys.path.append('../')
sys.path.append('../../')

import time, timeit
from _destruct import *

COUNT = 10
#COUNT = 10**6
COUNT = 10**3

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
     self.index = 0  

  def first(self):
     self.index = 0
 
  def nextItem(self):
   self.index = self.index + 1

  def isDone(self):
    if self.index >= self.container.size():
      return 1
    return 0 

  def currentItem(self):
    if self.index < self.container.size():
      return self.container.get(self.index)  

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

class PyReverseIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "DIterator")

  def currentItem(self):
    if self.index < self.container.size():
      return self.container.get(self.container.size() - self.index - 1)  

class PyPureIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "PyPureIterator")
     self.i = 0  
     self.container = None

  def first(self):
     self.i = 0
 
  def nextItem(self):
     self.i += 1

  def isDone(self):
     if self.i >= self.container.size():
       return DInt8(1)
     return DInt8(0) 

  def currentItem(self):
     if self.i < self.container.size():
       val = self.pyvector.get(self.i)
       return val


class PythonPureContainer(DObject):
  def __init__(self):
     #DObject.__init__(self, "DContainer") ##faudrait changer le type dynamiquement ou faire un dmutable et set les valeur de retour etc...
     #DObject.__init__(self, "DContainerString") ##possible aussi juste pour etre overwrite par python ou autre ...
     DObject.__init__(self, "DVector<String>") ##Donc faire XXX DContainerString comme ca c overwriteable !!!!!!!!!!!!!
     self.l = ['a', 'b', 'c']

  def get(self, index):
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      return len(self.l)

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
  #iterator = PyDIterator()
  print 'iterator set container'
  iterator.container = vector
  #print 'iterate reverse'
  print 'Iterating'
  for i in iterator:
    print i

   #pass
  print 'for in len'
  for x in range(0, len(vector)): #implem len
    print vector[x]


def setvector():
  print 'set vector with test str'
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 
  vector[0] = '100 strings'
  for x in range(0, 10):
    vector[x] = 'test ' +str(x)

  for i in vector:
    print i

def pushint():
 print 'set int'
 vi = PySimpleDVectorInt()
 for x in range(0, 10):
   vi.push(x)
 for x in range(0, 10):
    vi[x] = x * 10
 for i in vi:
    print i

def tryerror():
 print 'put int in string must return error'
 try:
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 
  for x in range(0, 10):
     vector[x] = x
 except Exception as e:
    print e

def pythonpurecontainer():
  ppi = PythonPureContainer()
  for x in ppi:
    print x

print "=" * 80

dvectorstring()
simplevectorstring()
pythonvector()
simplevectorint()
reverseiterate()
setvector()
pushint()
tryerror()
pythonpurecontainer()

class PyMap(DObject):
  def __init__(self):
     DObject.__init__(self, "DMapString")

print 'py map'
d = PyMap()
#set
print d
print len(d)
d["a"] = "first string"
d["b"] = "second string"
print len(d)

#get 
print d
print d["a"]
print d["b"]
try:
  print d["c"]
except KeyError as e: #XXX must return keyerror c pas les variant :) 
  print 'Error OK', e

try:
  print d[1]
except TypeError as e:
  print 'Type error ok' , e
except KeyError as e:
  print 'Keyerror bad ! must be typerror'

print 'for key in dict' 
for key in d:
  print key, ',',  d[key]

try:
  print iteritems
  for key, value in d.iteritems():
    print key, value
except NameError as e:
  print 'name error ok ' , e

#print itervalues
#for value in k.itervalues():
  #print value
