#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* 
 * tree.h has a complete red-black binary tree implementation. The manual is
 * not available in Linux but you can find it on the web:
 *
 * http://www.openbsd.org/cgi-bin/man.cgi?query=tree&apropos=0&sektion=0&manpath=OpenBSD+Current&arch=i386&format=html
 *
 * Another example of how to use the library:
 *
 * http://elephant.cs.fiu.edu/source/xref/openbsd-src/regress/sys/sys/tree/rb/rb-test.c
 */

#define PROMPT	"> "

struct student {
	char first_name[32];
	char last_name[32];
	int id;

	RB_ENTRY(student)
	tnode;
};

int studentcmp(struct student *s1, struct student *s2) {
	return s1->id - s2->id;
}

RB_HEAD(tree, student);
RB_PROTOTYPE(tree, student, tnode, studentcmp);
RB_GENERATE(tree, student, tnode, studentcmp);

void db_add_student(struct tree *db, char *first_name, char *last_name, int id) {
	struct student *re;
	struct student *s1 = malloc(sizeof(struct student));

	s1->id = id;
	strcpy(s1->first_name, first_name);
	strcpy(s1->last_name, last_name);

	re = RB_INSERT(tree, db, s1);
	if (re == NULL) {
		printf("student added\n");
	} else {
		printf("student already in db\n");
		free(s1);
	}
}

void print_student(struct student *st) {
	printf("%s %s %d\n", st->first_name, st->last_name, st->id);
}

void db_query_student(struct tree *db, int id) {
	struct student *re;
	struct student tmp;

	tmp.id = id;

	re = RB_FIND(tree, db, &tmp);
	if (re == NULL) {
		printf("not such student\n");
	} else {
		print_student(re);
	}
}

void db_delete_student(struct tree *db, int id) {
	struct student *re;
	struct student tmp;

	tmp.id = id;

	re = RB_FIND(tree, db, &tmp);
	if (re == NULL) {
		printf("not such student\n");
	} else {
		RB_REMOVE(tree, db, re);
		free(re);
		printf("student deleted\n");
	}
}

void db_print(struct tree *db) {
	struct student *s1;

	RB_FOREACH(s1, tree, db)
	{
		print_student(s1);
	}
}

void db_deallocate(struct tree *db) {
	struct student *s1;
	struct student *tmp;

	RB_FOREACH_SAFE(s1, tree, db, tmp)
	{
		RB_REMOVE(tree, db, s1);
		free(s1);
	}
}

int main(int argc, const char *argv[]) {
	char comm;
	char first_name[32];
	char last_name[32];
	int id;

	struct tree db;

	/* init db */
	RB_INIT(&db);

	/* operate over the db */
	printf(PROMPT);
	while ((comm = getchar()) != EOF) {
		switch (comm) {
		case 'a':
			/* 1. add new register to db */
			scanf("%s %s %d", first_name, last_name, &id);
			db_add_student(&db, first_name, last_name, id);
			break;
		case 'q':
			/* 2. query for student */
			scanf("%d", &id);
			db_query_student(&db, id);
			break;
		case 'd':
			/* 3. delete register from db */
			scanf("%d", &id);
			db_delete_student(&db, id);
			break;
		case 'p':
			/* 4. print complete database */
			db_print(&db);
			break;

		default:
			printf("wrong command\n");
			break;
		}

		while (getchar() != '\n')
			/* discard the character */;
		printf(PROMPT);
	}

	/* deallocate database */
	db_deallocate(&db);

	return 0;
}
