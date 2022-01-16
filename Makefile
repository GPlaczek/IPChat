all: serv client

serv:
	mkdir -p bin/
	gcc -Wall -o bin/serv src/serv.c def/serv*

client:
	mkdir -p bin/
	gcc -Wall -o bin/client src/client.c def/client* def/list.c

kill:
	ipcrm -a || true
	killall -9 serv || true
	killall -9 client || true

clean:
	rm -rf bin/*
