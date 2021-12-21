all: client server

client: client-socket.c
	gcc -o client client-socket.c
server: server-socket.c
	gcc -pthread -o server server-socket.c
clean:
	rm -f client server