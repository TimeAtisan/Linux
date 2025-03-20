SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
TARGET := server


$(TARGET):$(OBJS)
	$(CC) $^ -o $@ -lpthread

%.o:%.c
	$(CC) -c $< -o $@ -g

.PHONY:clean
clean:
	$(RM) -rf $(OBJS) server

rebuild:clean $(TARGET)