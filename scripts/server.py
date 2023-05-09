#!/usr/bin/env python3

import socket
import sys
import signal
import threading

HOST = '10.42.0.1'
PORT = 8080

def handle_client(client_socket, client_address):
    print(f'Client {client_address} verbunden')
    # Hier können Sie den Datenfluss mit dem Client behandeln
    try:
        while True:
            #data = client_socket.recv(1024)
            data = client_socket.recv(16).decode("utf-8")
            #if not data:
            #    break
            print(f"Empfangene Daten von {client_address}: {data}")
            #client_socket.sendall(data)
    except Exception as e:
        print(f"Verbindung mit {client_address} wurde getrennt: {e}")
    finally:
        client_socket.close()
        print(f"Client {client_address} getrennt")

def start_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((HOST, PORT))
        server_socket.listen()

        print(f'TCP-Server gestartet auf {HOST}:{PORT}')
        while True:
            client_socket, client_address = server_socket.accept()
            client_thread = threading.Thread(target=handle_client, args=(client_socket, client_address))
            client_thread.start()

if __name__ == '__main__':
    start_server()
