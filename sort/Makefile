CC		:= gcc
CFLAGS	:= -Wall -g

OBJS	+= insert.o bubble.o select.o merge.o
OBJS	+= main.o
TARGET	:= sort

.PHONY: clean

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

clean:
	rm -rf $(OBJS) $(TARGET)
