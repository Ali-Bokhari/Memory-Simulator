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
  for(i = 0; i < elements; i++){
    ds_read(&old, sizeof(long)+(i*sizeof(int)), sizeof(int));
    ds_write(sizeof(long)+(i*sizeof(int)), &new, sizeof(int));
    old = new;
  }
  elements++;
  return 0;
}

void array_test(){
  int i;
  int rread;
  for(i=0; i<MAX_ELEMENTS; i++){
    ds_read(&rread, sizeof(long)+(i*sizeof(int)), sizeof(int));
    printf("%d: %d\n", i, rread);
  }
}

int main( int argc, char**argv )
{
  int value;
  long index;

  ds_create("array.bin", 2048);
  ds_create_array();

  if(argc!=3){
    fprintf( stderr,"Usage:  %svalue index\n", argv[0] );
    return-1;
  }
  value = atoi( argv[1] );
  index = atol( argv[2] );

  ds_init_array();
  ds_insert( value, index );
  array_test();
  printf("%ld\n", elements);

  return 0;

}
