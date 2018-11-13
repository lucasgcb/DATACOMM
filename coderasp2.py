import socket
import sys
 	
HOST = ''   # Symbolic name meaning all available interfaces
PORT = 8888 # Arbitrary non-privileged port
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Socket created')
alive = True
try:
	s.bind((HOST, PORT))
	go = 1
except socket.error as msg:
	print('Bind failed. Error Code : ')
	print(msg)
	sys.exit()


print('Socket bind complete')

s.listen(10)
print('Socket now listening')

	#wait to accept a connection - blocking call
while alive:
	conn, addr = s.accept()
 	
	print('Connected with ' + addr[0] + ':' + str(addr[1]))
 	
	#now keep talking with the client
	data = conn.recv(1024)
	
	print(data)
	
	conn.sendall(data)
	conn.close()
	
	

