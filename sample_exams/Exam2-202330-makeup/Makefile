all: lunch.bin game.bin critical.bin threadpool.bin

CFLAGS+=-Wall -O0

lunch.bin: lunch.o lunch_tests.o

game.bin: game.o game_tests.o

critical.bin: critical.o critical_tests.o

%.bin: %.o
	$(CC) $(CFLAGS) $(LDLIBS) -o $@ $^ -lm -pthread

.PHONY: clean all
clean:
	rm -rf *.bin *.o

submit:
	@echo "\e[0;32m Cleaning up submission directory \e[0m"
	make clean
	@echo "\n\e[0;32m Generating tarball of modified files \e[0m"
	tar cvf makeup_exam2.tar lunch.c game.c critical.c threadpool.c
	@echo "\n\e[0;32m Generating patch file as backup \e[0m"
	git diff > makeup_exam2.patch
	@echo "\n\e[0;31m Please do not forget to submit to Gradescope \e[0m"
