#include <stdio.h>
#include <stdlib.h>
#include "grep.h"
#include "parcer.h"

# define MAX_NUMBER_OF_FLAGS 8

int main(int argc, char** argv){
	char* file_to_read_from = NULL; char* string_to_search = NULL;
	int program_flags[MAX_NUMBER_OF_FLAGS] = {0};
	get_program_properties(&file_to_read_from, &string_to_search, program_flags, argc-1, argv);
	ProgramProperties* our_properties = createProgramProperties(file_to_read_from,
																string_to_search, program_flags);
	if(our_properties == NULL){
		exit(EXIT_FAILURE);
	}
	if(grep_on_file(our_properties)){
		free(our_properties);
		exit(EXIT_SUCCESS);
	}
	free(our_properties);
	exit(EXIT_FAILURE);
}
