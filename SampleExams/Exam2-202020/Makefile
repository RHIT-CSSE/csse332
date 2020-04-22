all: downloads road polite_threads parallel_add

downloads: downloads.c
	gcc -pthread downloads.c -o downloads

road: road.c
	gcc -pthread road.c -o road

polite_threads: polite_threads.c
	gcc -pthread polite_threads.c -o polite_threads

parallel_add: parallel_add.c
	gcc -pthread parallel_add.c -o parallel_add

clean:
	rm -f downloads road polite_threads parallel_add

