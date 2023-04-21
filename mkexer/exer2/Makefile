TARGET := test2
SRC := $(TARGET).c
OBJ := $(TARGET).o
SOBJ := libdlink.so


CFLAGS := -g -Wall
CC := gcc $(CFLAGS)

all : $(TARGET)
$(TARGET) : $(OBJ) $(SOBJ)
	export LD_LIBRARY_PATH="/home/meit2321/cpi2321/mkexer/exer2"
	gcc -o $@ $(SRC) -L. -ldlink
.c.o:
	$(CC) -c $^
$(SOBJ): 
	gcc -shared display.c -fPIC -o $(SOBJ)

clean:
	rm -f $(TARGET) $(OBJ) $(SOBJ)

