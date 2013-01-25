#!/usr/bin/env python2.4
import sys, traceback, Ice, time

Ice.loadSlice('../../../../Interfaces/Base.ice')

import RoboCompBase

ic = None

class Client (Ice.Application):
	def run (self, argv):
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
			self.basePrx.setMotorsStateNormal()
			self.basePrx.setSpeedWheels(-100., 80.)
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

c = Client()
c.main(sys.argv)




