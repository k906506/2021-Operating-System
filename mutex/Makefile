pc : producer-consumer.o
	gcc -o pc producer-consumer.o -lpthread

producer-consumer.o : producer-consumer.c
	gcc -c producer-consumer.c

clean:
	rm -rf *.o
	rm -rf pc
