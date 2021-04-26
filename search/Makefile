CC		:= gcc
CFLAGS	:= -Wall -g

OBJS	+= bsearch.o bsearch_min.o
OBJS	+= main.o
TARGET	:= search

.PHONY: clean

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)
