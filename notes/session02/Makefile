all: glue illusionist referee malloc fptrs

glue:
	gcc -o glue glue.c

illusionist:
	gcc -o illusionist illusionist.c -fno-stack-protector

referee:
	gcc -o referee referee.c

malloc:
	gcc -o malloc malloc.c

fptrs:
	gcc -o fptrs fptrs.c

clean:
	rm -rf glue illusionist referee malloc fptrs
