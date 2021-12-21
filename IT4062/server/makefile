all: server db

server: server.c
	gcc -o server server.c serverFunc.c `pkg-config --libs mysqlclient`

db: createDB.c
	gcc -o db createDB.c `pkg-config --libs mysqlclient`

clean:
	rm -f server db