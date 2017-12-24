RC_HomeWork: RC_HomeWork.o
	gcc -Wall -D_REENTRANT RC_HomeWork.o -o RC_HomeWork

RC_HomeWork.o: RC_HomeWork.c
	gcc -Wall -c  RC_HomeWork.c -o RC_HomeWork.o
