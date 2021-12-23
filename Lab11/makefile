all: client server

client: tcp_client.c message.c
	gcc -o client tcp_client.c message.c
server: select_server.c message.c serverFunc.c
	gcc -o server select_server.c message.c serverFunc.c

clean:
	rm -f client server