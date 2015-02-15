#!/usr/bin/python 
import time, timeit
import sys

sys.path.append('../../')
sys.path.append('../test/')

from libdwrapper import DWrapper 
from _destruct import *

#def timeFunc(func, args, count = COUNT):
   #a = time.time()
   #for x in xrange(count):
     #w = func(args)
   #b = time.time()
   #print str(func) + " : "+  str(b-a)

wrapper = DWrapper()

def createDStruct():
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

def generateObject():
  print """Create a pure python object from a python DStruct"""
  dstruct = DStructs().find("PyNtfsBase")
  obj = dstruct.newObject()

  obj.setValue("mbr", 666)
  value = obj.getValue("mbr")

  obj.setValue("boot", DInt32(998))
  value = obj.getValue("boot")

  obj.setValue("info", "It's a kind of magic object") 
  obj.getValue("info")
    
  value = obj.getValue("parent")

  n = DStruct(None, "Node")
  n.addAttribute(DAttribute("size", DInt64))
  n.addAttribute(DAttribute("parent", DObject))
 
  parentObj = n.newObject()
  parentObj.setValue("size", 11232)
      
  obj.setValue("parent", parentObj)
   
  return obj

class PyNtfsDerived(DObject):
  def __init__(self):
     DObject.__init__(self, "PyNtfsBase")
     self.boot = 100
     self.info = "plop"

def getObject():
  res = wrapper.getObject()
  print res
  return res

def setObject(obj):
   print '-- pywrapper.setObject(' + str(obj) + ')'
   res = wrapper.setObject(obj)

def getValue():
   print '-- pywrapper.getValue()'
   res = wrapper.getValue()
   print 'getValue res ' + str(res)
   return res

def setValue(obj):
   print '-- pywrapper.setValue(' + str(obj) + ')'
   res = wrapper.setValue(obj)

print "=== setObject DNone =========="
setObject(DNone())
getObject()
print "==============================="

print "=== object None ==="
setObject(None)
getObject()
print "==================="

print "=== setObject PyNtfsBase object ==="
createDStruct()
obj = generateObject()
print obj
setObject(obj)
ntfs = getObject()
print ntfs.mbr
print "==============================="

print "=== setObject PyNtfsDerived object ==="
derived = PyNtfsDerived()
derived.mbr =4242
setObject(derived)
setObject(PyNtfsDerived())
ntfs = getObject()
print ntfs.mbr
print "==============================="


getObject()
print "==============================="

print "=== setObject (1) ==============="
setObject(1)
getObject()
print "==============================="

getValue()
print "=== setValue (DInt32(1)) ==============="
setValue(DInt32(42))
getValue()

setValue(100)
getValue()
#setValue(DInt64(64))
#getValue()
setValue(derived)
getValue()
print getValue().mbr
print "==============================="

#wrapper.setValue(DInt64(1))
#wrapper.setValue(11)
#wrapper.setValue(11)
#wrapper.setValue(11)
#wrapper.setValue(11)
#
#for i in range(0, 1000000):
  #wrapper.getValue()
  #wrapper.setValue(DInt64(i))
#  wrapper.setValue(derived)
  #wrapper.setValue(DUnicodeString("totototo"))
  #wrapper.setValue(11)
  #wrapper.getObject()
  #derived.info = 'tourne' + str(i)
#wrapper.setObject(derived)
#print wrapper.getValue()
#print wrapper.getObject()


print "define PyIterator(DObject)"
class PyIterator(DObject):
  def __init__(self):
    DObject.__init__(self, "DVectorString")
    self.l = [] #XXX class are frozen !!!   because of set /get qui call pas le truc original je suppose 

  def __getitem__(self, args):
     if args < self.size():     
       return self.get(args)   
     raise StopIteration()  
 
  def get(self, args):
     return self.l[args] 

  def push(self, args):
     self.l.append(args)
     return self.size()

  def size(self):
      return len(self.l)

print "define NtfsConfig(DStruct)"
class NtfsConfig(DStruct):
  def __init__(self):
     DStruct.__init__(self, None, "NtfsConfig")
     self.addAttribute(DAttribute("do_deleted", DInt8))
     self.addAttribute(DAttribute("block_size", DInt16))
     self.addAttribute(DAttribute("root", DObject))
     self.addAttribute(DAttribute("children", DObject))
     destruct = DStructs()
     destruct.registerDStruct(self)
   
 
print "create object NtfsConfig"
config = NtfsConfig()
print "create object argument = config.newObject()"
argument = config.newObject()

print "define NtfsDeFaultArgument(DObject)"
class NtfsDefaultArgument(DObject):
  def __init__(self):
     print 'NtfsDefaultArgumet'
     DObject.__init__(self, "NtfsConfig")
     self.do_deleted = 1
     self.block_size = 1024
     self.root = None
     iterator = PyIterator()
     iterator.push("python string 1")
     iterator.push("python string 2")
     iterator.push("python string 3")
     iterator.push("python string 4")
     print "XXXX FORING "
     #for i in iterator:
        #print i 
     self.children = iterator

print "create NtfsDefaultArugment"
default = NtfsDefaultArgument()
#wrapper.serialize(default)

print "Define Node(DStruct)"
class Node(DStruct):
  def __init__(self):
    DStruct.__init__(self, None, "Node")
    self.addAttribute(DAttribute("path", DUnicodeString))
    self.addAttribute(DAttribute("name", DUnicodeString))
    self.addAttribute(DAttribute("absolute", DUnicodeString))
#XXX pouvoir add des fonctions !!

print "define NtfsModule(DObject)"
class NtfsModule(DObject):
  def __init__(self):
    #print 'STARTING NTFS MODULE MEME S il sappelle juste module :)'
    DObject.__init__(self, 'DModule')

  def start(self, args):
    print 'python ntfs module'
    print  "config : "
    print args.root
    print args.block_size
    print args.do_deleted
    print args.children
    for x in range(args.children.size()):
       print args.children.get(x)

print "create ntfsModule"
ntfsModule = NtfsModule()
print "ntfs.module.start(default)"
ntfsModule.start(default)


print "===RUN ==="
#wrapper.serialize(ntfsModule)
#wrapper.serialize(default)
print "wrapper.run ntfsModule.start(default)"
wrapper.run(ntfsModule, default)

print "Define DPythonModule(DStruct)"
class DPythonModule(DStruct):
  def __init__(self):
     DStruct.__init__(self, None, "DPythonModule")
     self.addAttribute(DAttribute("start", DMethod, DNone, DObject))
     self.addAttribute(DAttribute("args", DObject))
     self.addAttribute(DAttribute("report", DMethod, DNone, DNone))
     destruct = DStructs() #RELOUTE DE DEVOIR CE REGISTER !!1 faut un DMutableOBject ..
     destruct.registerDStruct(self)
   
print "create DPythonModule()"
dpythonModule = DPythonModule()

print "Define DPythonNtfsModule(DObject"
class DPythonNtfsModule(DObject):
  def __init__(self):
   DObject.__init__(self, "DPythonModule")

  #@static
  #def new(self, args):
     #return DPthonNtfsModule() #XXX si on veu run plusieurs fois le module la c un singleton
     #la struct est enregistrer mais pas le 'module' faudrait une base d object avec un createur
     # == de MakeNewObj en c++ pour cree des instance differente et si c un singleton
     # il suffit de remplacer la methode statis makeNewObj pour renvoye tjrs le mem object  
 
  def start(self, args):
     print '!PythonNtfsModule start :'
     print  "config : "
     print "    ", args.root
     print "    ", args.block_size
     print "    ", args.do_deleted
     print "    ", args.children
     for x in range(args.children.size()):
       print "    ", args.children.get(x)
     self.args = args

  def report(self):
      print 'reporint NTFS MODULE'
      print 'block size is ', self.args.block_size

print "--create DPythonNtfsModule()"
pythonNtfs = DPythonNtfsModule()
wrapper.run(pythonNtfs, default)
print "--RUNING value object !"
wrapper.runValueObject(pythonNtfs, default)
print "----RUNING METHOD OBJECT !!!!!!!!!!!"
wrapper.runValueMethod(pythonNtfs.start, default)
wrapper.runValueMethod(pythonNtfs.report, default)

print "---XXXXXXXXXX pyre pure iterator def"
class PyPureIteratorDef(DStruct):
  def __init__(self):
    DStruct.__init__(self, "DVectorString", "DPureIterator")

    self.addAttribute(DAttribute("__getitem__", DMethod, DUnicodeString, DInt32))
    self.addAttribute(DAttribute("__iter__", DMethod, DObject, DNone))
    self.addAttribute(DAttribute("__next__", DMethod, DObject, DNone))
    destruct = DStructs() #RELOUTE DE DEVOIR CE REGISTER !!1 faut un DMutableOBject ..
    destruct.registerDStruct(self)
      #self.register()
pureIteratorDef = PyPureIteratorDef()

print "---py pure itrator"
class PyPureIterator(DObject):
  def __init__(self):
    #DObject.__init__(self, "DPureIterator") XXX XXX XXX douvle heritage fait sefgfault !!! strange 
    DObject.__init__(self, "DVectorString")
    self.l = [] 

  def __getitem__(self, args):
     if args < self.size():     
       return self.get(args)   
     raise StopIteration()  

  def __next__(self):
     print 'next called'

  def __iter__(self):
     print 'iter called'
     return self.l.__iter__()
 
  def get(self, args):
     return self.l[args] 

  def size(self):
     return len(self.l)

  def push(self, args):
     self.l.append(args)
     return self.size()

  #def iterator(self):
     #print "ITERATOORR XXX CAR SEFAULT DSI PAS LA" 
     ##XXX 
      #return self.l #return self.l !!!  doit etre possible

pureIterator = PyPureIterator()
pureIterator.push("pure string 1")
pureIterator.push("pure string 2")
print ' ZZZZZZZZZZZ'
print "Itreating a returned pure iterator"
for i in pureIterator:
  print i
print 'setting an iterator to an object and return to cpp'
print pythonNtfs.args.children
pythonNtfs.args.children = pureIterator
print pythonNtfs.args.children
it = pythonNtfs.args.children
#print it
#print it.__getitem__(0)
print 'foring '
#for i in it
  #print i

pyIterator = PyIterator()
pyIterator.push("string 1")
pyIterator.push("string 2")
pyIterator.push("string 3")
pyIterator.push("string 4")
print pyIterator.get(1)

diterator = DStructs().find('DIterator').newObject()
print diterator
print 'FORING USING A C++ PROTOCOL ITERATOR !' 

#DIterator(iterable) plus simple ds le constrcteur qu'avec set mais dois pouvoir passer objet a un constructeur de dobject semble compliquer 
diterator.setContainer(pyIterator)
di = diterator
di.first()
def iterate(iterator):
  print 'iterate ' + str(iterator)
  iterator.first()
  while not di.isDone():
    print iterator.currentItem(DNone)
    iterator.next()

iterate(di)
di.setContainer(pureIterator)
iterate(di)

ci = DStructs().find('DVectorString').newObject()
ci.push('c++ object push in pi 1')
ci.push('c++ object push in pi 2')
ci.push('c++ object push in pi 3')
di.setContainer(ci)
iterate(di)

print 'WWWWWWWWWWWWWWWWWWWW pure returned iterator ' 
citerator =  ci.iterator()
print citerator

print 'DESTRUCT'
a = time.time()
for x in range(0, 10):
  ci.push(str(x))
b = time.time()
print b-a

a = time.time()
print 'for i in ci'
for i in ci:
  #print i
  s = i
print s
b = time.time()
print b-a


print '||| CLASSICAL PYTHON'
a = time.time()
l = []
for x in range(0, 10):
   l.append(str(x))
b = time.time()
print b -a
a=time.time()
print 'for i in python'
for i in l:
  s = i 
print s 
b = time.time()
print b-a

class PyDIterator(DObject):
  def __init__(self, pyvector):
     DObject.__init__(self, "DIterator")
     self.pyvector = pyvector 
     self.i = 0  

  def first(self):
     print 'pyIterator first'
     self.i = 0
 
  def next(self):
     print 'pyIterator next'
     self.i += 1

  def isDone(self):
     print 'pyIterator isDone'
     if self.i >= self.pyvector.size():
       return 1
     print '0 isDone'
     return DInt8(0) 

  def currentItem(self):
     print 'currentItem' 
     if self.i < self.pyvector.size():
       val = self.pyvector.get(self.i)
       return val

  def setContainer(self, item):
     print 'setContainer'
      
class PySimpleIterator(DObject):
  def __init__(self):
     DObject.__init__(self, 'DIterator') 

class PythonDVector(DObject):
  def __init__(self):
     DObject.__init__(self, "DVectorString")
     self.l = ['AAAAA', 'BBBBB', 'CCCC']

  def get(self, index):
     return self.l[index] 

  def size(self):
      return len(self.l)

  def iterator(self):
     #XXX les 2 marche ici c accesible par heritage mais que si on utilise pas le dynamic_cast<> ds py__dobject::_iter.cpp
     print 'return a python iterator'
     iterator = PySimpleIterator()
     iterator.setContainer(self)
     return iterator

     print 'return a python iterator'
     return PyDIterator(self)  
  #def iterator(self):
     #print 'PyIterator next ' + str(self.l)
     #XXX oblider de coder un PyDIterator ...  pour convetir etc...
     #return self.l

print 'PYTHON VECTOR'
pyvector = PythonDVector()
print 'forward'
for i in pyvector:
  print i

wrapper.serialize(pyvector) 
# XXX march en python en tous cas (TEST EN CPP !!!) 
# mais doit retouruner un iterator eherite si non ca marche psa car le convertie en base ? 

#class PythonVector(DObject):
  #def __init__(self):
    #DObject.__init__(self, "DVectorString")



  #print i
#citerator.first()
#print citerator.next()
#print citerator.currentItem()

#for i in ci:
  #print i
#for i in pureIterator:
  #print i
# 
#print 'python :( il itere pas mmee s il a un __getitem__ :('
#for i in pythonNtfs.args.children:
  #print i
#print pythonNtfs.args.children
#for i in pythonNtfs.args.children:
  #print i


#XXX test EVENT HANDLER PUR DOBJECT : 
#
#class EventStruct(DStruct):
  #def __init__(self):
     #DStruct.__init__(None, "DEvent")
     #self.__addAttribute("EventType", DObject)
     #self.__addAttribute("function", DObject)
     #self.__addAttribute("argument", DObject)
# 
#class EventHandler(DStruct):
  #def __init__(self):
     #DStruct.__init__(None, "DEventHandler")
    # 
#
#class Toto(DObject):
     #EventHandler
#
#toto.connect(tutu, "eventype", "method")
#tutu.emit("method", method, config)

 
