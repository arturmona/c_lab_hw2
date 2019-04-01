#include <stdio.h>
#include <stdlib.h>
#include "grep.h"
#include "parcer.h"

int main(int argc, char** argv){
	int success;
	ProgramProperties* our_properties = calloc(1, sizeof(ProgramProperties));
	if(our_properties == NULL){
		exit(EXIT_FAILURE);
	}
	get_program_properties(our_properties, argc-1, argv);
	if(grep_on_file(our_properties)) {
		free(our_properties);
		exit(EXIT_SUCCESS);
	}
	free(our_properties);
	exit(EXIT_FAILURE);
}