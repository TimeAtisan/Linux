TARGET=tree.out

$(TARGET):tree.c
	cc $^ -o $@

.PHONY:clean echo run

clean:
	rm -rf *.out

clean_file:
	rm -rf $(ECHO_FILE)	

ECHO_FILE ?= file1
ECHO_CONTENT ?= "12345"
echo:
	echo -n $(ECHO_CONTENT) > $(ECHO_FILE)

RUN_ARGS ?=
run:
	./$(TARGET) $(RUN_ARGS)
