#ifndef GREP_H
#define GREP_H

#include "grep_utilities.h"



/**** struct for all possible program properties ****/

typedef struct GrepProperties{
	char* file_to_read_from;
	char* pattern;
	int also_print_line_number; /*n*/
	int ignore_upper_lower_case; /*i*/
	int print_num_lines_after_match; /*A NUM*/
	int print_file_offset_to_line; /*b*/
	int only_print_num_of_matched_lines; /*c*/
	int print_only_strict_match; /*x*/
	int print_only_lines_not_matching; /*v*/
	int use_regular_expressions; /*E*/
} GrepProperties;

/************** function declarations **************/

int grep_on_file(GrepProperties* grepProperties);
void get_program_properties(GrepProperties *  grepProperties,int num_of_args, char** program_arguments);

#endif /* GREP_H */