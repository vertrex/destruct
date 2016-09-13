#!/usr/bin/python
import sys

sys.path.append('../../')
sys.path.append('../')

from _destruct import *

from PyQt4.QtGui import QApplication, QTreeWidget, QTreeWidgetItem, QMainWindow, QDockWidget, QTableWidget, QWidget, QBoxLayout, QTableWidgetItem, QDialog, QAction, QVBoxLayout, QDialogButtonBox, QLabel, QLineEdit, QGroupBox, QFormLayout, QSpinBox, QFileDialog
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
    self.setText(0, key.name)
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
    count = valuesList.size()
    for index in range(0, count):
        value = valuesList.get(index)
        self.setItem(index, 0, QTableWidgetItem(value.name))
        self.setItem(index, 1, QTableWidgetItem(value.valueTypeName()))
        data = value.data().data()
        if type(data) == bytearray:
          self.setItem(index, 2, QTableWidgetItem(hexlify(data)))
        elif type(data) == DObject:
          multiString = ""
          for string in data:
            multiString += string + ","
          self.setItem(index, 2, QTableWidgetItem(multiString[:-1]))
        else:
          if type(data) == unicode:
            self.setItem(index, 2, QTableWidgetItem(data))
          else:
            self.setItem(index, 2, QTableWidgetItem(str(data)))
    self.resizeColumnsToContents()
    self.resizeRowsToContents()

class RegistryBrowserWidget(QWidget):
  def __init__(self, regf):
    QWidget.__init__(self)
    self.regf = regf
    self.keyTreeView = KeyTreeView(self, regf.key)
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
    localAction = QAction("&Local", self)
    localAction.triggered.connect(self.local);
    self.menuBar().addAction(localAction)
    loader = DStructs().find("Import").newObject()
    loader.file("../../../examples/modules/libdestruct_rpc.so")
    loader.file("../../../examples/modules/libdestruct_registry.so")

  def addRegistryBrowserWidget(self, regf):
    self.registryBrowserWidget = RegistryBrowserWidget(regf)
    self.dockWidget = QDockWidget()
    self.dockWidget.setWidget(self.registryBrowserWidget) 
    self.addDockWidget(Qt.TopDockWidgetArea, self.dockWidget)

  def local(self):
    filePath = QFileDialog.getOpenFileName().toUtf8()
    self.registry = DStructs().find("Registry").newObject() 
    regf = self.registry.open(str(filePath))
    self.addRegistryBrowserWidget(regf)

  def connection(self):
    connectionDialog = ConnectionDialog(self)
    ok = connectionDialog.exec_()
    if not ok:
      return

    arg = DStructs().find("ClientArgument").newObject()
    arg.port = connectionDialog.port.value() 
    arg.address = str(connectionDialog.ipAddress.text())
    self.client = DStructs().find("Client").newObject(arg)
    #self.registry = self.client.findObject() #registry is hardcoded in rpc client change that 
    self.registry = self.client.createRoot("Registry") #registry is hardcoded in rpc client change that 
    regf = self.registry.open(str(connectionDialog.filePath.text()))
    self.addRegistryBrowserWidget(regf)

if __name__ == "__main__":
  app = QApplication(sys.argv)
  window = MainWindow()
  window.show()
  sys.exit(app.exec_())
