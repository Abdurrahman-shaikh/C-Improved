exec := a.out

sources := $(wildcard src/*.c)
objects := $(sources:.c=.o)

flags := -g -Wall -lm -ldl -fPIC -rdynamic

$(exec): $(objects)
	gcc $(flags) $^ -o $@

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm $(exec) $(objects)

lint:
	clang-tidy src/*.c src/include/*.h
