CC = clang

all: client server

client: udp_client.c
	$(CC) udp_client.c -o udp_client

server: udp_server.c
	$(CC) udp_server.c -o udp_server 

run_client: client
	./udp_client 127.0.0.1 1234

run_server: server
	./udp_server 1234