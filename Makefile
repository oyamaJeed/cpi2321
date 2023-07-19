# TARGET NAME
TARGET := kadai12s

# OBJECT FILES
OBJS := kadai12s.o clThread_12.o tcpip_12.o utils_12.o

# SOURCE FILES
SRCS := $(OBJS:%.o=%.c)

# COMPILER TYPE
CC := gcc

# COMPILER OPTION
CFLAGS := -g -Wall -Wextra -DDEBUG=1
#						-D: definition for '#if DEBUG=1'

# DYNAMIC LIBRARY
LDFLAGS := -lpthread

# MAKE ALL (PHONY TARGET)
.PHONY: all
all: $(TARGET)

# LINK OBJECTS
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# COMPILE SOURCES
./%.o: ./%.c
	$(CC) $(CFLAGS) -c $<
# Ex.
# kadai12s.o: kadai12s.c
#	$(CC) $(CFLAGS) -c kadai12s.c
# clThread_12.o: clTthread_12.c
#	$(CC) $(CFLAGS) -c $<

# UPDATE HEADER FILE
$(OBJS): myTCPSv.h
# Ex.
# kadai12s.o: myTCPSv.h
# clThread_12.o: myTCPSv.h

# CLEAN FILES (PHONY TARGET)
.PHONY: clean
clean: 
	rm -f *.o *~ $(TARGET)
