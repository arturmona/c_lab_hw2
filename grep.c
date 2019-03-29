#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grep.h"

#define SUCCESS 1
#define FAILURE 0

ProgramProperties* createProgramProperties(char* file_to_read_from, char* string_to_search, int* flags){
	ProgramProperties* program_properties = (ProgramProperties*) malloc(sizeof(ProgramProperties));
    if (!program_properties){ /* allocation failed */
        printf("Error: createProgramProperties has failed\n");
        return NULL;
    }
    program_properties->file_to_read_from = file_to_read_from;
	program_properties->string_to_search = string_to_search;
	program_properties->also_print_line_number = flags[ALSO_PRINT_LINE_NUMBER];
	program_properties->ignore_upper_lower_case = flags[IGNORE_UPPER_LOWER_CASE];
	program_properties->print_num_lines_after_match = flags[PRINT_NUM_LINES_AFTER_MATCH];
	program_properties->print_file_offset_to_line = flags[PRINT_FILE_OFFSET_TO_LINE];
	program_properties->use_regular_expressions = flags[USE_REGULAR_EXPRESSIONS];
	program_properties->only_print_num_of_matched_lines = flags[ONLY_PRINT_NUM_OF_MATCHED_LINES];
	program_properties->print_only_strict_match = flags[PRINT_ONLY_STRICT_MATCH];
	program_properties->print_only_lines_not_matching = flags[PRINT_ONLY_LINES_NOT_MATCHING];
	return program_properties;
}

void grep_line(ProgramProperties* program_properties, char* line_to_check){
    /* here goes all the 'if's' that use the program_properties */
    if(strstr(line_to_check, program_properties->string_to_search)){
        printf("%s\n", line_to_check);
    }
}

int grep_on_file(ProgramProperties* program_properties){
    FILE* file;
    size_t line_lenght = 0;
    ssize_t amount_chars_read;
    char* read_line = NULL;
    if(program_properties->file_to_read_from == NULL){
		file = stdin;
	}
	else{
		file = fopen(program_properties->file_to_read_from, "r");
		if (file == NULL){
			printf("Error: could not open file '%s'.\n", program_properties->file_to_read_from);
			return FAILURE;
		}
	}
	while ((amount_chars_read = getline(&read_line, &line_lenght, file)) != -1) {
        grep_line(program_properties, read_line);
    }
	fclose(file);
	free(read_line);
    return SUCCESS;
}
