CC=g++
CPPFLAGS=-std=c++17 -Wall -Wextra -g

.PHONY: clean

%: %.cpp
	$(CC) $(CPPFLAGS) -o $@ $^

clean:
	find . -exec file {} \; | rg -i elf | cut -d":" -f1 | xargs --no-run-if-empty rm -v
