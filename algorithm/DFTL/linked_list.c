#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

struct linked_list {
	int data;
	struct linked_list *next;
	struct linked_list *prev;
}*head, *tail;

void insert(int num) {
	struct linked_list *next = head;
	struct linked_list *prev;
	struct linked_list *elem = malloc(sizeof(struct linked_list));
	elem->data = num;
	elem->next = NULL;
	elem->prev = NULL;

	if (head != NULL) {
		while (next != NULL) {
			if (num <= next->data)
				break;
			else
				next = next->next;
		}
		if (next != NULL) {
			prev = next->prev;
			next->prev = elem;
			prev->next = elem;
			elem->next = next;
			elem->prev = prev;
		}
		else {
			elem->prev = tail;
			tail->next = elem;
			tail = elem;
		}
	}
	else {
		head = elem;
		tail = elem;
	}
}

void delete(int num) {
	struct linked_list *elem = head;
	struct linked_list *prev;
	struct linked_list *next;
	if (tail != NULL) {
		while (elem != NULL) {
			if (num == elem->data)
				break;
			else
				elem = elem->next;
		}
		if (elem != NULL) {
			prev = elem->prev;
			next = elem->next;
			if (next != NULL)
				next->prev = prev;
			else
				tail = prev;
			if (prev != NULL)
				prev->next = next;
			else
				head = next;
			free(elem);
		}
		else
			printf("No such an element!\n");
	}
	else
		printf("No such an element!\n");

}

void print() {
	if (head != NULL) {
		struct linked_list *elem = head;
		while (elem != NULL) {
			printf("%d\n", elem->data);
			elem = elem->next;
		}
	}
	else
		printf("No element!\n");
}

void print_back() {
	if (tail != NULL) {
		struct linked_list *elem = tail;
		while (elem != NULL) {
			printf("%d\n", elem->data);
			elem = elem->prev;
		}
	}
	else
		printf("No element!\n");
}

void show_menu() { // ¸Þ´ºÆÇ 
	printf("******************************\n");
	printf("INSERT                      1\n");
	printf("DELETE                      2\n");
	printf("PRINT                       3\n");
	printf("PRINT_BACK                  4\n");
	printf("Show Menu                   5\n");
	printf("Exit                        6\n");
	printf("******************************\n");
}

int main(int agrc, char* argv[]) {
	head = NULL;
	tail = head;

	show_menu();

	while (1) {
		char n[100] = ""; 
		printf("Select : ");
		scanf("%s", n);
		int num;

		switch (atoi(n)) {
		case 1:
			printf("Type a number to insert : ");
			scanf("%d", &num);
			insert(num);
			break;
		case 2:
			printf("Type a number to delete : ");
			scanf("%d", &num);
			delete(num);
			break;
		case 3:
			print();
			break;
		case 4:
			print_back();
			break;
		case 5:
			show_menu();
			break;
		case 6:
			exit(0);
			break;
		}
	}

}