my_grep: main.o grep.o grep_parcer.o
	gcc -Wextra -Wall -o my_grep main.o grep.o grep_parcer.o
	chmod 755 my_grep
main.o: main.c grep.h
	gcc -Wextra -Wall -c main.c
	chmod 755 main.o  
grep.o: grep.c grep.h grep_parcer.h
	gcc -Wextra -Wall -c grep.c
	chmod 755 grep.o 
grep_parcer.o: grep_parcer.c grep_parcer.h
	gcc -Wextra -Wall -c grep_parcer.c 
	chmod 755 grep_parcer.o 
.PHONY: clean all test
clean:
	-rm *.o my_grep
all:
	gcc -Wextra -Wall -o my_grep main.c grep.c grep_parcer.c
	chmod 755 my_grep
test:
	~nimrodav/grep_tests/run_all.sh