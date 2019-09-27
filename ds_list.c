#include "ds_list.h"
#include "ds_memory.c"

void ds_create_list(){
  long negone = -1;
  if (ds_init( "list.bin" ) != 0){
    return 1;
  }
  address = ds_malloc(sizeof(long));
  if (address == -1){
    return 1;
  }
  if (ds_write(address, &negone, sizeof(long)) == -1){
    return 2;
  }
  if (ds_finish()==0){
    return 3;
  }
  return 0;
}

int ds_init_list(){
  if (ds_init( "list.bin" ) != 0){
    return 1;
  }
  return 0;
}

int ds_replace(int value, long index){
  long next;
  struct ds_list_item_struct list;
  int i;
  if (ds_read(&next, 0, sizeof(long)) == NULL){
    return 1;
  }
  for(i = 0; next != -1; i++){
    if (ds_read(&list, next, sizeof(struct ds_list_item_struct)) == NULL){
      return 2;
    }
    next = list.next;
    if(index == i){
      list.item = value;
      if (ds_write(next, &list, sizeof(struct ds_list_item_struct)) == -1){
        return 3;
      }
      return 0;
    }
  }
  return 4;
}
