all: file_processor file_producer

file_processor.o: file_processor.c file.h
	gcc -c -ggdb file_processor.c

file_producer.o: file_producer.c file.h
	gcc -c -ggdb file_producer.c

file_processor: file_processor.o file.h
	gcc file_processor.o -ggdb -o file_processor

file_producer: file_producer.o file.h
	gcc file_producer.o -ggdb -o file_producer

clean:
	rm -f *.o file_processor file_producer
