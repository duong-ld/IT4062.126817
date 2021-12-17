all: server

server: server.c
	gcc -o server server.c serverFunc.c `pkg-config --libs mysqlclient`

clean:
	rm -f server