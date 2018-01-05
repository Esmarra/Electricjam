homework_client: homework_client.o
	gcc -Wall -D_REENTRANT homework_client.o -o homework_client

homework_client.o: homework_client.c
	gcc -Wall -c  homework_client.c -o homework_client.o
