#!/usr/bin/env python2.4

#    Copyright (C) 2010 by RoboLab - University of Extremadura
#
#    This file is part of RoboComp
#
#    RoboComp is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    RoboComp is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
#


from PyQt4 import QtCore, QtGui, Qt

import sys, traceback, Ice, time, threading

Ice.loadSlice('../../../../Interfaces/Base.ice')

import RoboCompBase

ic = None

orders = dict()
orders['stop'] = 0
orders['forwards'] = 1
orders['backwards'] = 2
orders['left'] = 3
orders['right'] = 4

lastOrder = orders['stop']


class GuiThread (threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.app = QtGui.QApplication(sys.argv)
		self.gui = Gui()
		self.gui.show();
		print 'Gui init()'
	def run(self):
		print 'Gui running'
		self.app.exec_()

class Gui (QtGui.QWidget):
	def __init__(self, parent = None):
		print 'ggggggg'
		QtGui.QWidget.__init__(self)
		self.setParent(parent)
		self.setFixedSize(100, 100)
		self.show()
		self.button1 = QtGui.QPushButton(self)
		self.button1.setGeometry(0, 0, 100, 25)
		self.button1.setText('up')

class ClientThread (threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		print 'Client init()'
		
		self.c = Client()
	def run(self):
		print 'Client run()'
		self.c.main(sys.argv)

class Client (Ice.Application):
	def run (self, argv):
		print 'Client running'
		global ic
		status = 0
		self.shutdownOnInterrupt()

		ic = self.communicator()

		# Get connection config
		try:
			proxyString = ic.getProperties().getProperty('BaseProxy')
			print 'Proxy string: ', proxyString
		except:
			print 'Cannot get BaseProxy property.'
			return

		self.basePrx = None
		basebasePrx = None
		try:
			basebasePrx = self.communicator().stringToProxy(proxyString)
			print 'BasePrx: ', type(basebasePrx)
		except:
			traceback.print_exc()
			print 'stringToProxy :-('
			status = 1
		
		try:
			self.basePrx = RoboCompBase.BasePrx.checkedCast(basebasePrx)
		except:
			traceback.print_exc()
			print 'checkedCast :-('
			status = 1

		if status != 1:
			self.basePrx.setSpeedWheels(100., 100.)
			time.sleep(3)
			self.basePrx.setMotorsStateFree()
		else:
			print ':\'('

	def stop (self):
		if self.communicator():
			try:
				self.communicator().destroy()
			except:
				traceback.print_exc()
				status = 1


print 'a'
gui = GuiThread()
print 'b'
client = ClientThread()
print 'c'
gui.start()
print 'd'
print 'e'
client.run()




