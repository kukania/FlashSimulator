#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

LINKED_LIST* queue_insert(void* table_ptr) {
	/* Queue allocation */
	LINKED_LIST *elem = (LINKED_LIST*)malloc(sizeof(LINKED_LIST));
	elem->DATA = table_ptr;
	elem->next = NULL;
	elem->prev = NULL;

	/* Insert queue to head */
	if (head != NULL) {
		head->prev = elem;
		elem->next = head;
		head = elem;
	}
	else {
		head = elem;
		tail = elem;
	}
	return elem;
}

void queue_delete(LINKED_LIST* elem) {
	LINKED_LIST *prev = elem->prev;
	LINKED_LIST *next = elem->next;

	/* Update linked list */
	if (elem != NULL) {
		if (next != NULL)
			next->prev = prev;
		else 
			tail = prev;
		if (prev != NULL)
			prev->next = next;
		else 
			head = next;
		free(elem); // Delete elem
	}
	else
		printf("No such an element!\n");
}

void queue_update(LINKED_LIST* elem){
	LINKED_LIST *prev = elem->prev;
	LINKED_LIST *next = elem->next;
	if (elem != NULL) { 
		if(elem != head) {
			if(next != NULL)
				next->prev = prev;
			else
				tail = prev;
			prev->next = next;
			elem->prev = NULL;
			elem->next = head;
			head->prev = elem;
			head = elem;
		}
	}
	else
		printf("No such an element!\n");
}

void queue_print() {
	if (head != NULL) {
		LINKED_LIST *elem = head;

		/* Print from head to tail */
		while (elem != NULL) {
			printf("%d\n", elem->DATA);
			elem = elem->next;
		}
	}
	else
		printf("Empty queue!\n");
}

void queue_print_back() {
	if (tail != NULL) {
		LINKED_LIST *elem = tail;

		/* Print from tail to head */
		while (elem != NULL) {
			printf("%d\n", elem->DATA);
			elem = elem->prev;
		}
	}
	else
		printf("Empty queue!\n");
}
