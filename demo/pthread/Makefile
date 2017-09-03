
all:
	gcc pthread1.c -o pthread1 -lpthread
	gcc pthread2.c -o pthread2 -lpthread
	gcc pthread3.c -o pthread3 -lpthread
	gcc pthread4.c -o pthread4 -lpthread
	gcc mutex.c -D_REENTRANT -o mutex -lpthread
	gcc semaphore.c -D_REENTRANT -o semaphore -lpthread

clean:
	rm pthread1 pthread2 pthread3 pthread4 mutex semaphore

