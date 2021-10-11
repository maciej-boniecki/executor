compilation_flags = -D_GNU_SOURCE -Wall -Wextra -Wshadow -std=c17
objects = errors.o execution_result.o help.o io.o main.o options.o process.o resources.o time.o
program = executor

$(program): $(objects)
	gcc -o $(program) $(objects)

errors.o: errors.c errors.h
	gcc $(compilation_flags) -c errors.c

execution_result.o: execution_result.c execution_result.h
	gcc $(compilation_flags) -c execution_result.c

help.o: help.c help.h
	gcc $(compilation_flags) -c help.c

io.o: io.c io.h
	gcc $(compilation_flags) -c io.c

main.o: main.c execution_result.h options.h process.h resources.h
	gcc $(compilation_flags) -c main.c

options.o: options.c options.h
	gcc $(compilation_flags) -c options.c

process.o: process.c process.h errors.h io.h options.h resources.h time.h
	gcc $(compilation_flags) -c process.c

resources.o: resources.c resources.h
	gcc $(compilation_flags) -c resources.c

time.o: time.c time.h
	gcc $(compilation_flags) -c time.c

clean:
	rm -f $(program) $(objects)
