all: serv client

serv:
	gcc -Wall -o bin/serv src/serv.c def/serv*

client:
	gcc -Wall -o bin/client src/client.c def/client*


clean:
	rm -rf bin/*
