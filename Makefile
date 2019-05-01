COMPILATION_FLAGS = -Wextra -Wall
SET_AUTHORIZATION = chmod 755 

my_grep: main.o grep_file.o grep_parcer.o
	@gcc $(COMPILATION_FLAGS) -o my_grep main.o grep_file.o grep_parcer.o
	@$(SET_AUTHORIZATION) my_grep
main.o: main.c grep_file.h grep_parcer.h
	@gcc $(COMPILATION_FLAGS) -c main.c
	@chmod 755 main.o  
grep_file.o: grep_file.c grep_file.h grep_parcer.h
	@gcc $(COMPILATION_FLAGS) -c grep_file.c
	@$(SET_AUTHORIZATION) grep_file.o 
grep_parcer.o: grep_parcer.c grep_parcer.h
	@gcc $(COMPILATION_FLAGS) -c grep_parcer.c 
	@$(SET_AUTHORIZATION) grep_parcer.o 
	
.PHONY: clean all test grep_parcer_test grep_file_test
clean:
	@-rm *.o my_grep
all:
	@gcc $(COMPILATION_FLAGS) -o my_grep main.c grep_file.c grep_parcer.c
	@$(SET_AUTHORIZATION) my_grep
test:
	@~nimrodav/grep_tests/run_all.sh
grep_parcer_test:
	@gcc $(COMPILATION_FLAGS) -o grep_parcer_test
	@$(SET_AUTHORIZATION) grep_parcer_test
	@./grep_parcer_test 
grep_file_test:
	@gcc $(COMPILATION_FLAGS) -o grep_file_test
	@$(SET_AUTHORIZATION) grep_file_test
	@./grep_file_test 