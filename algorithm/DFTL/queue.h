typedef struct linked_list{
	void *DATA;
	LINKED_LIST *next;
	LINEKD_LIST *prev;
}LINKED_LIST;

void queue_insert(void* table_ptr);
void queue_delete(LINKED_LIST* elem);
void queue_update(LINKED_LIST* elem);
void queue_print();
void queue_print_back();
