#!/usr/bin/python 
import sys

#sys.path.append('../')
sys.path.append('../../')

import time, timeit
from _destruct import *

COUNT = 10
#COUNT = 10**6
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
   for i in dobject:
      #print i, type(i)
      s = i

class PySimpleIterator(DObject):
  def __init__(self):
     DObject.__init__(self, 'DIteratorString') 

class PySimpleDVectorInt(DObject):
  def __init__(self):
     DObject.__init__(self, "DVector<DInt64>")


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

print 'put int in string doit planter mais doit etre catchable'
#try:
 #for x in range(0, 10):
   #vector[x] = x
#except :
  #pass

vi = PySimpleDVectorInt()
for x in range(0, 10):
  vi.push(x)
for x in range(0, 10):
  vi[x] = x * 10

for i in vi:
  print i

#XXX faudrait le call en C++ ! 

class PyPureIterator(DObject):
  def __init__(self):
     DObject.__init__(self, "PyPureIterator")
     self.i = 0  

  def first(self):
     self.i = 0
 
  def next(self):
     self.i += 1

  def isDone(self):
     if self.i >= self.pyvector.size():
       return DInt8(1)
     return DInt8(0) 

  def currentItem(self):
     if self.i < self.pyvector.size():
       val = self.pyvector.get(self.i)
       return val

  def container(self, item):  
     self.pyvector = item
     return self.pyvector
      

class PythonPureIterable(DObject):
  def __init__(self):
     DObject.__init__(self, "DIterable") #faudrait un DIterable register avant DIterable64... sin on faut redefinir toute la struct soit meme c pas le but faudrait pouvoir heriter donc pour creer par un hex un objet node iterable 
     self.l = ['a', 'b', 'c']

  def get(self, index):
     return self.l[index] 

  def push(self, val):
     self.l.append(val)

  def size(self):
      return len(self.l)

  def iterator(self): 
     iterator = PyPureIterator()
     iterator.container(self)
     return iterator

ppi = PythonPureIterable()
for x in ppi:
  print x

#class Node(DObject):
  #def __int__(self):
    

  #def children(self): # iterable ! 
     #return self.__childrenslist

##  def size(self): #file ? 
##  def file() ? 

 
  #def exif():
    #pass

  #def ntfs() ? 

  #def attribute(self):#
     #return dobject   #



#class NtfsNode(Node):
  #def DObject.Node ... herite


  #def modified

  #def accessed

  #def created


#d


#class NtfsNodeMFT(NtfsNOde)
  #def mftid

  #def cluster

  #def toto


#new Destruct()NtfsNode()
#return .. .


#if read les attribut dynamiquement ? 

#for i in data:
  #if addAttribute(name, type)
    #//possibilite de register ds DMutable un truc genre DTemp ? en singleton
    #//comme ca ds DFF recupere toute les struct genre et connais les type ? 
    #//car c le prob du dmutable 
    #//par ex: si contruit avec un nom ca register mais faudrait pas que ca regiter avec le meme nom 
    #//
    #DMutable().setAttributeValue("toto", 1, type)


  #return DMutable #les node avec attrib dynamqiue 


