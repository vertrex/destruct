from _destruct import *
from pyterator import PyIterator

class Embed(object):
  def __init__(self):
    print 'embdeing start'

  def run(self):
     s = PyIterator()
     print 'running ' + str(dir(Destruct))
     Test().run()
