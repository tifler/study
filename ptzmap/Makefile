CC		:= gcc
CFLAGS	:= -Wall -g -lm
CFLAGS	+= -DXDEBUG

OBJS	+= sct.o ptzmap.o
OBJS	+= main.o
TARGET	:= ptz

.PHONY: clean

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
	rm -rf $(OBJS) $(TARGET)
