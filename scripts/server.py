#!/usr/bin/env python3

import socket
import threading
import rospy
from std_msgs.msg import Bool
import signal
import sys


HOST = '10.42.0.1'
PORT = 8080

BUTTON_1 = '10.42.0.75'
BUTTON_2 = '10.42.0.66'
BUTTON_3 = '10.42.0.32'

class ButtonServer:
    def __init__(self):
        self.button_1_pub = rospy.Publisher('button_1', Bool, queue_size=1)
        self.button_2_pub = rospy.Publisher('button_2', Bool, queue_size=1)
        self.button_3_pub = rospy.Publisher('button_3', Bool, queue_size=1)

        self.start_server()

    def handle_client(self, client_socket, client_address):
        print(f'Client {client_address} connected')

        try:
            while not rospy.is_shutdown():
                data = client_socket.recv(16).decode("utf-8")
                if client_address[0] == BUTTON_1 and data == "Pressed":
                    self.button_1_pub.publish(True)
                elif client_address[0] == BUTTON_2 and data == "Pressed":
                    self.button_2_pub.publish(True)
                elif client_address[0] == BUTTON_3 and data == "Pressed":
                    self.button_3_pub.publish(True)
            client_socket.close()

        except Exception as e:
            print(f"{client_address} disconnected: {e}")
        except KeyboardInterrupt:
            client_socket.close()
            print("Client closed")

    def start_server(self):
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
                try:
                    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                    server_socket.bind((HOST, PORT))
                    server_socket.listen()

                    print(f'TCP-Server started {HOST}:{PORT}')
                    while not rospy.is_shutdown():
                        client_socket, client_address = server_socket.accept()
                        client_thread = threading.Thread(target=self.handle_client, args=(client_socket, client_address))
                        client_thread.start()
                except KeyboardInterrupt:
                    server_socket.close()
                    print("Server closed")
                    sys.exit(0)

if __name__ == '__main__':
    rospy.init_node('buttons_tcp_server', anonymous=True)
    signal.signal(signal.SIGINT, signal.default_int_handler)
    bs = ButtonServer()
