#ifndef GREP_H
#define GREP_H



/**** struct for all possible program properties ****/
typedef  struct Parsed_pattern{
	int type;
	char the_char;
	char * string;
	char  char_start;
	char  char_end;
}Parsed_pattern;

typedef struct Lines{
	char current_line[30];
	int offset;
	int num_of_bytes;
	int line;
	int print_next_line;
	int match_lines;
}Lines;

typedef struct ProgramProperties{
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
} ProgramProperties;

/************** function declarations **************/

ProgramProperties* createProgramProperties(char* file_to_read_from, char* string_to_search, int* flags);
int grep_on_file(ProgramProperties* program_properties);
void get_program_properties(ProgramProperties *  programProperties,int number_of_program_arguments, char** program_arguments);
#endif /* GREP_H */