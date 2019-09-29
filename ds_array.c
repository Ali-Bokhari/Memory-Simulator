/*
Ali Bokhari
0970498
abokhari@uoguelph.ca
*/

#include "ds_array.h"
#include "ds_memory.c"

long elements;

int ds_create_array(){
  long address;
  long zero = 0;
  if (ds_init( "array.bin" ) != 0){
    return 1;
  }
  address = ds_malloc(sizeof(long));
  if (address == -1){
    return 1;
  }
  if (ds_write(address, &zero, sizeof(long)) == -1){
    return 2;
  }
  if (ds_malloc(MAX_ELEMENTS*sizeof(int)) == -1){
    return 3;
  }
  if (ds_finish()==0){
    return 4;
  }
  return 0;
}

int ds_init_array(){
  if (ds_init("array.bin") != 0){
    return 1;
  }
  if (ds_read(&elements, 0, sizeof(long)) == NULL){
    return 2;
  }
  return 0;
}

int ds_replace(int value, long index){
  if (ds_write(sizeof(long) + (index*sizeof(int)), &value, sizeof(int)) == -1){
    return 1;
  }
  return 0;
}

int ds_insert(int value, long index){
  int old;
  int new = value;
  int i;
  if(index > elements){
    return 1;
  }
  elements++;
  for(i = index; i < elements; i++){
    ds_read(&old, sizeof(long)+(i*sizeof(int)), sizeof(int));
    ds_write(sizeof(long)+(i*sizeof(int)), &new, sizeof(int));
    new = old;
  }
  return 0;
}

int ds_delete(long index){
  int old;
  int i;
  if(index > elements){
    return 1;
  }
  for(i = index; i < elements; i++){
    ds_read(&old, sizeof(long)+((i+1)*sizeof(int)), sizeof(int));
    ds_write(sizeof(long)+(i*sizeof(int)), &old, sizeof(int));
  }
  elements--;
  return 0;
}

int ds_swap(long index1, long index2){
  int value1;
  int value2;
  if(index1 > elements || index2 > elements){
    return 1;
  }
  ds_read(&value1, sizeof(long)+(index1*sizeof(int)), sizeof(int));
  ds_read(&value2, sizeof(long)+(index2*sizeof(int)), sizeof(int));
  ds_write(sizeof(long)+(index1*sizeof(int)), &value2, sizeof(int));
  ds_write(sizeof(long)+(index2*sizeof(int)), &value1, sizeof(int));
  return 0;
}

int ds_find(int target){
  int value;
  int i;
  for (i = 0; i < elements; i++){
    ds_read(&value, sizeof(long)+(i*sizeof(int)), sizeof(int));
    if(value == target){
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
  for (i = 0; i < MAX_ELEMENTS; i++){
    if(fscanf(fp, "%d", &value)==EOF){
      return 0;
    }
    ds_insert(value, i);
  }
  return 2;
}

int ds_finish_array(){
  if (ds_write(0, &elements, sizeof(long)) == -1){
    return 1;
  }
  if (ds_finish()==0){
    return 2;
  }
  return 0;
}

void array_test(){
  int i;
  int rread;
  for(i=0; i<elements; i++){
    ds_read(&rread, sizeof(long)+(i*sizeof(int)), sizeof(int));
    printf("%d: %d\n", i, rread);
  }
}
