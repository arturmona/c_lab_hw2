#include <stdio.h>
#include <stdlib.h>
#include "grep.h"

ProgramProperties* createProgramProperties(char* file_to_read_from, char* string_to_search,
								 int also_print_line_number, int ignore_upper_lower_case,
		                         int print_num_lines_after_match, int print_file_offset_to_line,
								 int only_print_num_of_matched_lines,
								 int print_only_strict_match, int print_only_lines_not_matching,
								 int use_regular_expressions){
	ProgramProperties* program_properties = (ProgramProperties*) malloc(sizeof(ProgramProperties));
    if (!program_properties){ /* allocation failed */
        printf("Error: createProgramProperties has failed\n");
        return NULL;
    }
    program_properties->file_to_read_from = file_to_read_from;
    program_properties->string_to_search = string_to_search;
    program_properties->also_print_line_number = also_print_line_number;
    program_properties->ignore_upper_lower_case = ignore_upper_lower_case;
    program_properties->print_num_lines_after_match = print_num_lines_after_match;
    program_properties->print_file_offset_to_line = print_file_offset_to_line;
    program_properties->use_regular_expressions = use_regular_expressions;
    program_properties->only_print_num_of_matched_lines = only_print_num_of_matched_lines;
    program_properties->print_only_strict_match = print_only_strict_match;
    program_properties->print_only_lines_not_matching = print_only_lines_not_matching;
    /************************************************************************************/
    program_properties->file_to_read_from = NULL;
	program_properties->string_to_search = NULL;
	program_properties->also_print_line_number = 0;
	program_properties->ignore_upper_lower_case = 0;
	program_properties->print_num_lines_after_match = 0;
	program_properties->print_file_offset_to_line = 0;
	program_properties->use_regular_expressions = 0;
	program_properties->only_print_num_of_matched_lines = 0;
	program_properties->print_only_strict_match = 0;
	program_properties->print_only_lines_not_matching = 0;
	return program_properties;
}

int term_matched(char* name){
    if(strstr(name, our_directories->term) != NULL){
        return 1;
    }
    return 0;
}
