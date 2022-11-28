all: glue illusionist referee

glue:
	gcc -o glue glue.c

illusionist:
	gcc -o illusionist illusionist.c -fno-stack-protector

referee:
	gcc -o referee referee.c

clean:
	rm glue illusionist referee
