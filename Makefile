all: serv client

serv:
	gcc -Wall -o bin/serv src/serv.c def/serv*

client:
	gcc -Wall -o bin/client src/client.c def/client* def/list.c

kill:
	killall -9 serv
	killall -9 client
	ipcrm -a

clean:
	rm -rf bin/*
