from time import sleep
import socket

def get_bitcoins():
	'''
	Dummy function, doesn't do anything
	'''
	return True

while True:
	try:
		s = socket.socket()
		s.connect(('www.malicious-bitcoin-miner.com', 80))
		print('Mining started...')
		get_bitcoins()
		break
	except:
		print('No internet connection')
		sleep(10)

