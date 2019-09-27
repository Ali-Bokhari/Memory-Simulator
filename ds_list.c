#include "ds_list.h"
#include "ds_memory.c"

void ds_create_list(){
  long negone = -1;
  long address;
  if (ds_init( "list.bin" ) != 0){
    return;
  }
  address = ds_malloc(sizeof(long));
  if (address == -1){
    return;
  }
  if (ds_write(address, &negone, sizeof(long)) == -1){
    return;
  }
  if (ds_finish()==0){
    return;
  }
  return;
}

int ds_init_list(){
  if (ds_init( "list.bin" ) != 0){
    return 1;
  }
  return 0;
}

int ds_replace(int value, long index){
  struct ds_list_item_struct list;
  int i;
  long prev;
  ds_read(&list.next, 0, sizeof(long));
  for (i = 0;list.next!= -1; i++){
    prev = list.next;
    ds_read(&list, list.next, sizeof(struct ds_list_item_struct));
    if(index == i){
      list.item = value;
      ds_write(prev, &list, sizeof(struct ds_list_item_struct));
    }
  }
  return 0;
}

int ds_insert(int value, long index){
  struct ds_list_item_struct previous, new;
  long previous_loc = 0;
  int i;
  if (ds_read(&previous.next, 0, sizeof(long)) == NULL){
    return 1;
  }
  for (i = index; i > 0; i--){
    if(previous.next == -1){
      return -1;
    }
    previous_loc = previous.next;
    if (ds_read(&previous, previous.next, sizeof(struct ds_list_item_struct)) == NULL){
      return 1;
    }
  }
  new.item = value;
  new.next = previous.next;
  previous.next = ds_malloc(sizeof(struct ds_list_item_struct));
  if (ds_write(previous.next, &new, sizeof(struct ds_list_item_struct)) == -1){
    return 2;
  }
  if(previous_loc == 0){
    if (ds_write(previous_loc, &previous.next, sizeof(long)) == -1){
      return 2;
    }
  } else {
    if (ds_write(previous_loc, &previous, sizeof(struct ds_list_item_struct)) == -1){
      return 2;
    }
  }
  return 0;
}

int ds_find(int target){
  struct ds_list_item_struct list;
  int i;
  ds_read(&list.next, 0, sizeof(long));
  for (i = 0;list.next!= -1; i++){
    ds_read(&list, list.next, sizeof(struct ds_list_item_struct));
    if(list.item == target){
      return i;
    }
  }
  return -1;
}

int ds_read_elements(char * filename){
  int i;
  FILE *fp;
  int value;

  fp = fopen(filename, "r");
  if(fp==NULL){
    return 1;
  }
  for (i = 0;; i++){
    if(fscanf(fp, "%d", &value)==EOF){
      return 0;
    }
    ds_insert(value, i);
  }
  return 2;
}

void print_list(){
  struct ds_list_item_struct list;
  int i;
  ds_read(&list.next, 0, sizeof(long));
  for (i = 0;list.next!= -1; i++){
    ds_read(&list, list.next, sizeof(struct ds_list_item_struct));
    printf("%d. item: %d next: %ld\n", i,list.item, list.next);
  }
}
