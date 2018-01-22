typedef struct linked_list{
	void *DATA;
	struct linked_list *next;
	struct linked_list *prev;
}LINKED_LIST;

LINKED_LIST* queue_insert(void* table_ptr);
void queue_delete(LINKED_LIST* elem);
void queue_update(LINKED_LIST* elem);
void queue_print();
void queue_print_back();
