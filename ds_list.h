/*
Ali Bokhari
0970498
abokhari@uoguelph.ca
*/

struct ds_list_item_struct {
  int item;
  long next;
};

void ds_create_list();
int ds_init_list();
int ds_replace(int value, long index);
int ds_insert(int value, long index);
int ds_delete(long index);
int ds_swap(long index1, long index2);
int ds_find(int target);
int ds_read_elements(char * filename);
int ds_finish_list();
void print_list();
