#include "ds_memory.h"
#include <stdio.h>
#include <stdlib.h>

struct ds_file_struct ds_file;
struct ds_counts_struct ds_counts;

int ds_create( char * filename, long size){
  FILE *fp;
  struct ds_blocks_struct new_block[MAX_BLOCKS];
  char zero_bytes = 0;
  int i;
  if ((fp = fopen(filename, "wb+")) == 0){
    return 1;
  }
  new_block[0].start = 0;
  new_block[0].length = size;
  new_block[0].alloced = 0;
  for (i = 1; i < MAX_BLOCKS; i++){
    new_block[i].start = 0;
    new_block[i].length = 0;
    new_block[i].alloced = 0;
  }
  if (fwrite(new_block, sizeof(struct ds_blocks_struct), MAX_BLOCKS, fp) != MAX_BLOCKS){
    return 2;
  }
  if (fwrite(&zero_bytes, sizeof(char), size, fp) != size){
    return 3;
  }
  fclose(fp);
  return 0;
}

int ds_init( char * filename ){
  FILE *fp;
  if ((fp = fopen(filename, "rb+")) == 0){
    return 1;
  }
  ds_file.fp = fp;
  if (fread(ds_file.block, sizeof(struct ds_blocks_struct), MAX_BLOCKS, fp) != MAX_BLOCKS){
    return 2;
  }
  ds_counts.reads = 0;
  ds_counts.writes = 0;
  return 0;
}

long ds_malloc( long amount ){
  int i;
  int j;
  for(i = 0; i < MAX_BLOCKS; i++){
    if (ds_file.block[i].length >= amount && ds_file.block[i].alloced == 0){
      ds_file.block[i].length = amount;
      ds_file.block[i].alloced = 1;
      for(j = 0; j < MAX_BLOCKS; j++){
        if (ds_file.block[j].length == 0){
          ds_file.block[j].start = ds_file.block[i].start + amount;
          ds_file.block[j].length = ds_file.block[i].length - amount;
          ds_file.block[j].alloced = 0;
          break;
        }
      }
      return ds_file.block[i].start;
    }
  }
  return -1;
}

void ds_free( long start ){
  int i;
  for(i = 0; i < MAX_BLOCKS; i++){
    if (ds_file.block[i].start == start){
      ds_file.block[i].alloced = 0;
    }
  }
}

void *ds_read( void *ptr, long start, long bytes){
  if (fseek(ds_file.fp, (sizeof(struct ds_blocks_struct)*MAX_BLOCKS) + start, SEEK_SET) != 0){
    return NULL;
  }
  if (fread(ptr, sizeof(char), bytes, ds_file.fp) != bytes){
    return NULL;
  }
  ds_counts.reads += 1;
  return ptr;
}

long ds_write( long start, void *ptr, long bytes){
  if (fseek(ds_file.fp, (sizeof(struct ds_blocks_struct)*MAX_BLOCKS) + start, SEEK_SET) != 0){
    return -1;
  }
  if (fwrite(ptr, sizeof(char), bytes, ds_file.fp) != bytes){
    return -1;
  }
  ds_counts.writes += 1;
  return start;
}

int main(){
  return 0;
}
