homework_server: homework_server.o
	gcc -Wall -D_REENTRANT homework_server.o -o homework_server

homework_server.o: homework_server.c
	gcc -Wall -c  homework_server.c -o homework_server.o
