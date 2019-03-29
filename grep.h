#ifndef GREP_H
#define GREP_H

/********** struct for all possible flags ***********/
typedef struct ProgramFlags{
	char* file_to_read_from;
	char* string_to_search;
	int also_print_line_number; /*n*/
	int ignore_upper_lower_case; /*i*/
	int print_num_lines_after_match; /*A NUM*/
	int print_file_offset_to_line; /*b*/
	int only_print_num_of_matched_lines; /*c*/
    int print_only_strict_match; /*x*/
    int print_only_lines_not_matching; /*v*/
    int use_regular_expressions; /*E*/
} ProgramFlags;

/************** function declarations ****************/
ProgramProperties* createProgramProperties(char* file_to_read_from, char* string_to_search,
								 int also_print_line_number, int ignore_upper_lower_case,
		                         int print_num_lines_after_match, int print_file_offset_to_line,
								 int only_print_num_of_matched_lines,
								 int print_only_strict_match, int print_only_lines_not_matching,
								 int use_regular_expressions);
#endif /* GREP_H */
