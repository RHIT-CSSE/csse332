
all: server server-admin prodcon

server: web-server.c
	gcc web-server.c -o server

server-admin: server-admin.c
	gcc server-admin.c -o server-admin

prodcon: producer-consumer.c
	gcc -pthread producer-consumer.c -o prodcon

clean:
	rm -rf *~ server server-admin prodcon