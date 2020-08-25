from time import sleep
import socket
import sys

def get_bitcoins():
	'''
	Dummy function, doesn't do anything
	'''
	return True

c2 = sys.argv[1]

while True:
	try:
		s = socket.socket()
		s.connect((c2, 80))
		print('Mining started...')
		get_bitcoins()
		break
	except:
		print('No internet connection')
		sleep(10)

