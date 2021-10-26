.PHONY: all, clean

all: client server

client: client.o serverUtil.o
	@echo "Compiling client"
	@gcc -o client client.o serverUtil.o
	@echo "Done"
server: server.o serverUtil.o
	@echo "Compiling server"
	@gcc -o server server.o serverUtil.o
	@echo "Done"

serverUtil.o server.o client.o: serverConstain.h
serverUtil.o server.o client.o: serverUtil.h


clean:
	@echo "Cleaning"
	@rm -f client server serverUtil.o server.o client.o
	@echo "Done"