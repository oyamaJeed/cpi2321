TARGET := display
SRC := $(TARGET).c
OBJ := $(TARGET).o
SOBJ := libdlink.so


CFLAGS := -g -Wall
CC := gcc $(CFLAGS)

all : $(OBJ) $(SOBJ)
.c.o:
	$(CC) -c $^
$(SOBJ): $(SRC)
	gcc -shared $(SRC) -fPIC -o $(SOBJ)

clean:
	rm -f $(OBJ) $(SOBJ)

