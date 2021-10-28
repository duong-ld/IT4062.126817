.PHONY: all, clean

all: client server

client: account.c fileFunc.c linkedList.c tcpUtil.c tcp_client.c
	@echo "Compiling client"
	@gcc -o client account.c fileFunc.c linkedList.c tcpUtil.c tcp_client.c
	@echo "Done"
server: account.c fileFunc.c linkedList.c tcpUtil.c tcp_server.c
	@echo "Compiling server"
	@gcc -o server account.c fileFunc.c linkedList.c tcpUtil.c tcp_server.c
	@echo "Done"

clean:
	rm -f client server