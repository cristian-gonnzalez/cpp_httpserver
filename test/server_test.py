import socket			 
import threading

port = 8080			
host = '127.0.0.1'

def do_work():
    s = socket.socket()		 
    s.connect((host, port)) 

    s.send("Hello server".encode())
    print (s.recv(1024).decode())

    s.close()	 

if __name__ == '__main__':	

    workers = []
    for i in range(10):
        w = threading.Thread(target=do_work, args=())
        workers += [w]
    for w in workers:
        w.start()
    for w in workers: 
        w.join()