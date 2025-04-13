SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
CC:=gcc
all:$(EXES)
%:%.c
	$(CC) $^ -o $@ -g -lmysqlclient

.PHONY:clean
clean:
	$(RM) -rf $(EXES)
rebuild:clean all