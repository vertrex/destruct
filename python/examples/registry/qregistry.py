#!/usr/bin/python
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *
from _registryrpc import *

from PyQt4.QtGui import QApplication, QTreeWidget, QTreeWidgetItem, QMainWindow, QDockWidget, QTableWidget, QWidget, QBoxLayout, QTableWidgetItem, QDialog, QAction, QVBoxLayout, QDialogButtonBox, QLabel, QLineEdit, QGroupBox, QFormLayout, QSpinBox
from PyQt4.QtCore import SIGNAL, Qt

from binascii import hexlify

class DObjectTreeWidgetItem(QTreeWidgetItem):
  def __init__(self, parent, dobject):
    super(DObjectTreeWidgetItem, self).__init__(parent)
    self.dobject = dobject

  def __getattr__(self, name):
    try :
      attr = self.__getattribute__(name)
    except Exception as exception:
      attr = getattr(self.dobject,  name)
    return attr

class KeyTreeWidgetItem(DObjectTreeWidgetItem):
  def __init__(self, parent, key):
    super(KeyTreeWidgetItem, self).__init__(parent, key)
    self.setText(0, key.keyName.name)
    self.__populated = False 

  def populateItemSubKeys(self):
    if not self.__populated:
      subKeyList = self.subkeys.list
      for subKey in subKeyList:
        item = KeyTreeWidgetItem(self, subKey)
      self.__populated = True

class KeyTreeView(QTreeWidget):
  def __init__(self, parent, dobject):
    super(KeyTreeView, self).__init__(parent)
    self.root = dobject
    self.setHeaderLabels(["key"])
    self.populateTree(dobject)
    self.itemExpanded.connect(self.expand)
 
  def populateTree(self, key):
    subKeysList = key.subkeys.list
    for subKey in subKeysList: 
      item = KeyTreeWidgetItem(None, subKey)
      self.addTopLevelItem(item)
      item.populateItemSubKeys()

  def expand(self, keyItem):
    for index in range(0, keyItem.childCount()):
      childItem = keyItem.child(index)
      childItem.populateItemSubKeys()

class ValuesTableWidget(QTableWidget):
  def __init__(self, parent):
    super(ValuesTableWidget, self).__init__(parent)
    self.setColumnCount(3)
    self.setHorizontalHeaderItem(0, QTableWidgetItem("Name"))
    self.setHorizontalHeaderItem(1, QTableWidgetItem("Type"))    
    self.setHorizontalHeaderItem(2, QTableWidgetItem("Data"))    
    self.verticalHeader().hide() 
    self.setShowGrid(False) 
 
  def populate(self, keyItem):
    valuesList = keyItem.values.list
    size = valuesList.size()
    self.setRowCount(size)
    index = 0
    for value in valuesList:
      self.setItem(index, 0, QTableWidgetItem(value.name))
      self.setItem(index, 1, QTableWidgetItem(value.valueTypeName()))
      vd = value.data() #or segfault !
      data = vd.data()
      if type(data) == bytearray:
        self.setItem(index, 2, QTableWidgetItem(hexlify(data)))
      elif type(data) == DObject:
        multiString = ""
        for string in data:
          multiString += string + ","
        self.setItem(index, 2, QTableWidgetItem(multiString[:-1]))
      else:
        self.setItem(index, 2, QTableWidgetItem(str(data)))
      index += 1
    self.resizeColumnsToContents()
    self.resizeRowsToContents()

class RegistryBrowserWidget(QWidget):
  def __init__(self, root):
    QWidget.__init__(self)
    self.keyTreeView = KeyTreeView(self, root)
    self.valuesTableWidget = ValuesTableWidget(self)
    self.keyTreeView.itemClicked.connect(self.valuesTableWidget.populate)
    boxLayout = QBoxLayout(QBoxLayout.LeftToRight, self)
    boxLayout.addWidget(self.keyTreeView)
    boxLayout.addWidget(self.valuesTableWidget)
    self.setLayout(boxLayout)

class ConnectionDialog(QDialog):
  def __init__(self, parent):
    super(ConnectionDialog, self).__init__(parent)
    layout = QVBoxLayout(self)
    formLayout = QFormLayout()
    self.filePath = QLineEdit("/home/vertrex/dump/registry/system") 
    formLayout.addRow(QLabel("server registry file path :"), self.filePath)
    self.ipAddress = QLineEdit("127.0.0.1")
    formLayout.addRow(QLabel("server ip address :"), self.ipAddress)
    self.port= QSpinBox()
    self.port.setMinimum(1)
    self.port.setMaximum(65536)
    self.port.setValue(0xdff)
    formLayout.addRow(QLabel("port :"), self.port)
    configurationGroupBox = QGroupBox("Configuration")
    configurationGroupBox.setLayout(formLayout)
    layout.addWidget(configurationGroupBox)

    self.buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel, Qt.Horizontal, self)
    layout.addWidget(self.buttons)
    self.buttons.accepted.connect(self.accept)
    self.buttons.rejected.connect(self.reject)

class MainWindow(QMainWindow):
  def __init__(self):
    super(MainWindow, self).__init__()
    openAction = QAction("&Connect", self)
    openAction.triggered.connect(self.connection);
    self.menuBar().addAction(openAction)
    self.registryRPC = RegistryRPC()

  def connection(self):
    connectionDialog = ConnectionDialog(self)
    ok = connectionDialog.exec_()
    if not ok:
      return

    #destruct import rpc
    #rpc connect
    #...

    registry = self.registryRPC.connect(str(connectionDialog.ipAddress.text()), connectionDialog.port.value())
    regf = registry.open(str(connectionDialog.filePath.text()))
    self.registryBrowserWidget = RegistryBrowserWidget(regf.key)
    self.dockWidget = QDockWidget()
    self.dockWidget.setWidget(self.registryBrowserWidget) 
    self.addDockWidget(Qt.TopDockWidgetArea, self.dockWidget)

if __name__ == "__main__":
  app = QApplication(sys.argv)
  window = MainWindow()
  window.show()
  sys.exit(app.exec_())
