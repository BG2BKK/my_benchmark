CC = gcc
LD = gcc
CFLAGS = -c -O -g -D_GNU_SOURCE 
LDFLAGS = 
  
SRCS = $(wildcard *.c)  
OBJS = $(patsubst %c, %o, $(SRCS))  
TARGET = bench 
  
.PHONY: all clean
  
all: $(TARGET) hello.o
  
$(TARGET): $(OBJS)
	$(LD) -o $@ $^  $(LDFLAGS)
  
.c.o:
	$(CC) $(CFLAGS) -o $@ $<  

hello.o: hello.cpp
	g++ -O -o $@ $<

clean:
	rm -f *.o $(TARGET)  
	rm *~ out core* -f
