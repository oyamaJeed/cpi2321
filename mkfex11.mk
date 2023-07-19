TARGET := ${arg}
SRC := $(TARGET).cpp
OBJ := $(TARGET).o

CFLAGS := -g -Wall
LFLAGS := -lm -pthread
OPTION := `pkg-config --cflags --libs opencv`
CC := g++ 

all : $(TARGET)

$(TARGET): $(OBJ1)
	$(CC) $^ $(CFLAGS) $(LFLAGS) $(OPTION) -o $@
$(OBJ1): $(SRC1) 
	$(CC) -c $^ $(CFLAGS) $(LFLAGS) $(OPTION)
clean:
	rm -f $(TARGET) $(OBJ)
