
from threading import Thread
from SocketServer import ThreadingMixIn

import io
import socket
import struct
from PIL import Image

image_index = 1

TCP_IP = '0.0.0.0'
TCP_PORT = 9001
BUFFER_SIZE = 1024

class ClientThread(Thread):

    #def __init__(self,ip,port,sock):
    def __init__(self,sock, image_index):
        Thread.__init__(self)
        #self.ip = ip
        #self.port = port
        self.sock = sock
        self.image_index = image_index
        #print " New thread started for "+ip+":"+str(port)

    def run(self):
 
        while True:
            # Read the length of the image as a 32-bit unsigned int. If the
            # length is zero, quit the loop
            image_len = struct.unpack('<L', self.sock.read(struct.calcsize('<L')))[0]
            if not image_len:
                break
            # Construct a stream to hold the image data and read the image
            # data from the connection
            image_stream = io.BytesIO()
            image_stream.write(self.sock.read(image_len))
            # Rewind the stream, open it as an image with PIL and do some
            # processing on it
            image_stream.seek(0)
            image = Image.open(image_stream)
            
            image.save('robot_images/image' + str(self.image_index) + '.jpg')
            #image_index += 1
            
            print('Image is %dx%d' % image.size)
            image.verify()
            print('Image is verified')
            self.sock.close()
            break


tcpsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcpsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
tcpsock.bind((TCP_IP, TCP_PORT))
threads = []

while True:
    tcpsock.listen(8)
    print "Waiting for incoming connections..."
    #(conn, (ip,port)) = tcpsock.accept()[0].makefile('rb')
    conn = tcpsock.accept()[0].makefile('rb')
    #print 'Got connection from ', (ip,port)
    #newthread = ClientThread(ip,port,conn)
    newthread = ClientThread(conn, image_index)
    image_index += 1
    newthread.start()
    threads.append(newthread)

for t in threads:
    t.join()