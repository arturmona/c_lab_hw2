my_grep: main.o grep.o grep_utilities.o
	gcc -Wextra -Wall -o my_grep main.o grep.o grep_utilities.o
	chmod 755 my_grep
main.o: main.c grep.h
	gcc -Wextra -Wall -c main.c
	chmod 755 main.o  
grep.o: grep.c grep.h grep_utilities.h
	gcc -Wextra -Wall -c grep.c
	chmod 755 grep.o 
grep_utilities.o: grep_utilities.c grep_utilities.h
	gcc -Wextra -Wall -c grep_utilities.c 
	chmod 755 grep_utilities.o 
.PHONY: clean all test
clean:
	-rm *.o my_grep
all:
	gcc -Wextra -Wall -o my_grep main.c grep.c grep_utilities.c
	chmod 755 my_grep
test:
	~nimrodav/grep_tests/beta1.sh