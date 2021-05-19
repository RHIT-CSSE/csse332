#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct llnode {
	struct llnode *prev;
	struct llnode *next;
	char name[32];
};

struct llnode *list_insert(struct llnode *after)
{
	struct llnode *node;

	node = calloc(1, sizeof(struct llnode));
	if (after) {
		node->prev = after;
		if (after->next) {
			node->next = after->next;
			after->next->prev = node;
		}
		after->next = node;
	}

	return node;
}

void list_remove(struct llnode *node)
{
	if (node->prev) {
		node->prev->next = node->next;
	}

	if (node->next) {
		node->next->prev = node->prev;
	}
}

int
_main(int argc, char **argv)
{
	if (argc != 4) {
		fprintf(stderr,
			"ERROR: Need 3 arguments passed from cmdline!\n");
		return 1;
	}

	/* create three node back to back: a -> b -> c */
	struct llnode *a = list_insert(0);
	struct llnode *b = list_insert(a);
	struct llnode *c = list_insert(b);

	/* copy the nodes' names from the command line arguments */
	strcpy(a->name, argv[1]);
	strcpy(b->name, argv[2]);
	strcpy(c->name, argv[3]);

	/* cleanup everything */
	list_remove(c);
	list_remove(b);
	list_remove(a);

	return 0;
}
