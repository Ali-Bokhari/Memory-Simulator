#Ali Bokhari
#0970498
#abokhari@uoguelph.ca

CC = gcc
CFLAGS = -ansi -pedantic -Wall

all: ds_memory.o ds_list.o ds_array.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<
