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
     DObject.__init__(self, "DVectorInt64")


class PySimpleDVectorString(DObject):
  def __init__(self):
     DObject.__init__(self, "DVectorString")

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
     DObject.__init__(self, "DVectorString")
     self.l = []

  def get(self, index):
     #return DUnicodeString('a')
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      return len(self.l)

class PyReverseIterator(DObject):
  def __init__(self, args):
     DObject.__init__(self, "DIterator", args)

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
     DObject.__init__(self, "DVectorString") ##Donc faire XXX DContainerString comme ca c overwriteable !!!!!!!!!!!!!
     self.l = ['a', 'b', 'c']

  def get(self, index):
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      return len(self.l)

class PyMap(DObject):
  def __init__(self):
     DObject.__init__(self, "DMapString")

class PyMapObject(DObject):
  def __init__(self):
     DObject.__init__(self, "DMapObject")

def dvectorstring():
  print "------ Python create c++ object via Destruct --"
  cs = Destruct().find('DVectorString')
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
  cs = Destruct().find('DVectorInt32')
  c = cs.newObject()
  a = timeFunc(fillInt, c)
  b = timeFunc(iterate, c)
  total((a, b,))

def reverseiterate():
  print "------ PyReverseItertor test --------""" 
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 
  iterator = PyReverseIterator(vector)
  print 'reverse iterate'
  for i in iterator:
    print i

  for x in range(0, len(vector)): #implem len
    print vector[x]


def setvector():
  print "------ Vector string set v[x] = ---------"
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 
  vector[0] = '100 strings'
  for x in range(0, 10):
    vector[x] = 'test ' +str(x)

  for i in vector:
    print i

def pushint():
 print "------ Vector string set v[x] = ---------"
 vi = PySimpleDVectorInt()
 for x in range(0, 10):
   vi.push(x)
 for x in range(0, 10):
    vi[x] = x * 10
 for i in vi:
    print i

def tryerror():
 print '------ VectorString set int : error -----'
 try:
  vector = PySimpleDVectorString()
  for i in range(0, 10):
    vector.push(str(i)) 
  for x in range(0, 10):
     vector[x] = x
 except Exception as e:
    print e

def pythonpurecontainer():
  print '------ PyPureContainer --------'
  ppi = PythonPureContainer()
  for x in ppi:
    print x

def fillMapString(m):
   for i in range(0, COUNT):
     m[str(i)] = 'A'

def getMap(m):
   for i in range(0, COUNT):
     m[str(i)]

def iterateMap(m):
   l = {}
   s = 0
   print 'will iteratte on ', len(m)
   for k in m:
      print k
      #print m[k] 
      #s += 1
      #l[k] = m[k]
   print 'have iterate on ' + str(s)

def simplemapstring():
  print "------ Fill & iterate Map string -----"
  m = PyMap()
  a = timeFunc(fillMapString, m)
  b = timeFunc(iterateMap, m)
  total((a, b,))

def pymapobject():
  mss = PyMap()
  mss['aa'] = '11'
  mss['bb'] = '22'

  mo = PyMapObject()
  mo[mss] = mss
  mo[mo] = mo
  
  for i in mo:
    print i, mo[i]
    for x in mo[i]:
      print x, mo[i][x]

  print type(mo)

def maperror():
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

  try:
    for key, value in d.iteritems():
      print key, value
  except AttributeError as e:
    print 'name error ok ' , e

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
simplemapstring()
pymapobject()

class PyDStream(DObject):
 def __init__(self, args):
   DObject.__init__(self, "DStream", args)

class PyDStreamCout(DObject):
  def __init__(self):
    DObject.__init__(self, "DStreamCout")

class DStreamArgumentS(DStruct):
  def __init__(self):
    DStruct.__init__(self, DNone, "Argument")
    self.addAttribute(DAttribute("input", DInt8))   
    self.addAttribute(DAttribute("filePath", DUnicodeString))   
    Destruct().registerDStruct(self)
DStreamArgumentS()

class DStreamArgument(DObject):
  def __init__(self, filePath, input):
    DObject.__init__(self, "Argument") 
    self.filePath = filePath 
    self.input = input

stream = PyDStream(DStreamArgument("pyfilearg", 1))
vector = PySimpleDVectorString()
for i in range(10000):
  vector.push(str(i))

print 'deserialization of map of ' +str(len(vector)) + ' item'

serializer = DSerialize("XML")
serializer.serialize(stream, vector)
serializer.serialize(PyDStreamCout(), vector)
#serializer = DSerialize("Text")

#stream = PyDStream(DStreamArgument("deserialieztext", 1))
#serializer.serialize(stream, vector)
#serializer.serialize(PyDStream(), vector)

def serialize(self):
  serializer = DSerialize("XML")

  stream = PyDStreamCout()
  stream = PyDStream(DStreamArgument("pyfilearg", 0))
  vector = PySimpleDVectorString()
  for i in range(10**2):
    vector.push(str(i))
  for i in range(10**2):
    print i
    serializer.serialize(stream, vector)

timeFunc(serialize, None)
