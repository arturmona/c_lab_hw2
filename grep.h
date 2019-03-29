#ifndef GREP_H
#define GREP_H

#define ALSO_PRINT_LINE_NUMBER 0
#define IGNORE_UPPER_LOWER_CASE 1
#define PRINT_NUM_LINES_AFTER_MATCH 2
#define PRINT_FILE_OFFSET_TO_LINE 3
#define ONLY_PRINT_NUM_OF_MATCHED_LINES 4
#define PRINT_ONLY_STRICT_MATCH 5
#define PRINT_ONLY_LINES_NOT_MATCHING 6
#define USE_REGULAR_EXPRESSIONS 7

/**** struct for all possible program properties ****/
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

#endif /* GREP_H */
