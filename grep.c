
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grep.h"
#include <ctype.h>

#define REGULAR_CHAR 0
#define DOT 1
#define SQUARE_BRACKETS 2
#define ROUND_BRACKETS 3
#define SUCCESS 1
#define FAILURE 0


void init_lines_struct(Lines * lines){
	lines->current_line=NULL;
	lines->match_lines = 0;
	lines->print_next_line = 0;
	lines->num_of_bytes=0;
	lines->line=0;
	lines->offset=0;
}

void to_lower_case(char* word) {
	int index;
	for (index = 0; word[index] != '\0'; index++){
		word[index] = tolower(word[index]);
	}
}

void initialize_type_REGULAR_CHAR(Parsed_pattern * parsedPattern,char the_char) {
	parsedPattern->the_char = the_char;
	parsedPattern->type = REGULAR_CHAR;
}

void initialize_type_DOT(Parsed_pattern * parsedPattern){
	parsedPattern->the_char='.';
	parsedPattern->type=DOT;
}

void initialize_type_SQUARE_BRACKETS(Parsed_pattern * parsedPattern, char start,char end){
	parsedPattern->char_start=start;
	parsedPattern->char_end=end;
	parsedPattern->type=SQUARE_BRACKETS;
}
int initialize_type_ROUND_BRACKETS(Parsed_pattern * parsedPattern,char * pattern) {
	int index = 0;
	while (pattern[index]!=')')
		index++;
	pattern[index]='\0';
	parsedPattern->string = &pattern[index+1];
	parsedPattern->type =ROUND_BRACKETS ;
	return index;
}
int check_special_chars(char  a_char){
	if (a_char=='(')
		return 1;
	if (a_char==')')
		return 1;

	if (a_char=='|')
		return 1;

	if (a_char=='.')
		return 1;

	if (a_char=='[')
		return 1;
	if (a_char==']')
		return 1;
	if (a_char=='{')
		return 1;
	if (a_char=='}')
		return 1;
	return 0;
}



int parse_pattern(Parsed_pattern * parsed_pattern,char * pattern,int regular) {
	int parsed_pattern_index = 0, pattern_index;
	Parsed_pattern *current_parsed_pattern_ptr;
	for (pattern_index = 0; pattern[pattern_index] != '\0'; pattern_index++, parsed_pattern_index++) {
		current_parsed_pattern_ptr=&parsed_pattern[parsed_pattern_index];
		if (pattern[pattern_index] == '\\') {
			if (check_special_chars(pattern[pattern_index + 1])) {
				pattern_index += 1;
			}
			initialize_type_REGULAR_CHAR(parsed_pattern, pattern[pattern_index]);
		} else if (pattern[pattern_index] == '.' && regular) {
			initialize_type_DOT(current_parsed_pattern_ptr);

		} else if (pattern[pattern_index] == '[' && regular) {
			initialize_type_SQUARE_BRACKETS(current_parsed_pattern_ptr, pattern[pattern_index + 1], pattern[pattern_index + 3]);
			pattern_index += 4;
		} else if (pattern[pattern_index] == '(' && regular) {
			pattern_index = initialize_type_ROUND_BRACKETS(current_parsed_pattern_ptr, pattern);
		} else if (!check_special_chars(pattern[pattern_index]))
			initialize_type_REGULAR_CHAR(current_parsed_pattern_ptr, pattern[pattern_index]);
		else {
			parsed_pattern_index--;
		}
	}
	return parsed_pattern_index;

}

void get_program_properties(ProgramProperties*  programProperties, int number_of_program_arguments, char** program_arguments){
	int index, pattern_set = 0;
	for(index=1; index < number_of_program_arguments+1; index++) {
		if (!strcmp(program_arguments[index], "-i")) {
			programProperties->ignore_upper_lower_case = 1;
		} else if (!strcmp(program_arguments[index], "-n")) {
			programProperties->also_print_line_number=1;
		} else if (!strcmp(program_arguments[index], "-A")) {
			programProperties->print_num_lines_after_match = atoi(program_arguments[index + 1]);
			index++;
		} else if (!strcmp(program_arguments[index], "-b")) {
			programProperties->also_print_line_number = 1;
		} else if (!strcmp(program_arguments[index], "-c")) {
			programProperties->only_print_num_of_matched_lines = 1;
		} else if (!strcmp(program_arguments[index], "-x")) {
			programProperties->print_only_strict_match = 1;
		} else if (!strcmp(program_arguments[index], "-v")) {
			programProperties->print_only_lines_not_matching = 1;
		} else if (!strcmp(program_arguments[index], "-E")) {
			programProperties->use_regular_expressions = 1;
		} else if (!pattern_set) {
			programProperties->string_to_search = program_arguments[index];
			pattern_set=1;
		} else {
			programProperties->file_to_read_from = program_arguments[index];
		}
	}
}

//int find_delimiter(char * pattern) {
//	unsigned int index = -1;
//	for (index = 0; index < strlen(pattern); index++) {
//		if (pattern[index] == '|' && (index == 0 || pattern[index - 1] != '\\')) {
//			pattern[index] = '\0';
//			break;
//		}
//		return index;
//	}
//}

int find_match(char * string,Parsed_pattern * parsedPattern,int size,int strict_match, int skipped_chars) {
	unsigned int index;
	int pos;
	int match=0;
	for (index = 0; index < strlen(string); index++) {
		for (pos = 0; pos < size&&pos+index<strlen(string); pos++) {
			if (parsedPattern[pos].type == REGULAR_CHAR && parsedPattern[pos].the_char != string[index + pos]) {
				break;
			}
			if (parsedPattern[pos].type == DOT)
				continue;
			if (parsedPattern[pos].type == SQUARE_BRACKETS && (parsedPattern[pos].char_start > string[pos + index] ||
															   parsedPattern[pos].char_end < string[pos + index])) {
				break;
			}
			if (parsedPattern[pos].type == ROUND_BRACKETS) {

			}
		}
		if(strict_match) {
			if ((pos >= size && pos + index >= strlen(string))&&!skipped_chars) {
				match=1;
				break;
			}
		}
		else if (pos>=size){
			match = 1;
			break;
		}
	}
	return match;
}


int check_match(ProgramProperties* program_properties,Lines * lines){
	int match;
	int size;
	int skipped_chars=0;
	Parsed_pattern parsed_pattern[strlen(program_properties->string_to_search)];
	size=parse_pattern(parsed_pattern,program_properties->string_to_search,program_properties->use_regular_expressions);
	match=find_match(lines->current_line, parsed_pattern,size,program_properties->print_only_strict_match,skipped_chars);
	if (program_properties->print_only_lines_not_matching)
		match^=1;
	if (match)
		lines->match_lines++;
	return match;
}

void print_matches(ProgramProperties * programProperties,Lines * lines ,char Delimiter){
	if(programProperties->also_print_line_number)
		printf("%d%c", lines->line,Delimiter);

	if(programProperties->print_file_offset_to_line)
		printf("%d%c", lines->offset,Delimiter);
	printf("%s\n",lines->current_line);
}

void grep_line(ProgramProperties* program_properties,Lines * lines) {
	int num;
	if (program_properties->ignore_upper_lower_case) {
		to_lower_case(lines->current_line);
		to_lower_case(program_properties->string_to_search);
	}
	num = check_match(program_properties, lines);
	if (program_properties->only_print_num_of_matched_lines)
		return;
	if (num) {
		print_matches(program_properties,lines,':');
		if (program_properties->print_num_lines_after_match) {
			lines->print_next_line = program_properties->print_num_lines_after_match;
		}
	}
	else if (lines->print_next_line > 0){
		print_matches(program_properties,lines, '-');

	}
}

int grep_on_file(ProgramProperties* program_properties) {
	FILE *file;
	size_t line_lenght = 0;
	ssize_t amount_chars_read;
	Lines lines;
	init_lines_struct(&lines);
	if (program_properties->file_to_read_from == NULL) {
		file = stdin;
	} else {
		file = fopen(program_properties->file_to_read_from, "r");
		if (file == NULL) {
			printf("Error: could not open file '%s'.\n", program_properties->file_to_read_from);
			return FAILURE;
		}
	}
	while ((amount_chars_read = getline(&lines.current_line, &line_lenght, file)) != -1) {
		grep_line(program_properties, &lines);
		lines.line++;
		lines.num_of_bytes += amount_chars_read;
	}
	if (program_properties->only_print_num_of_matched_lines)
		printf("%d", lines.match_lines);
	fclose(file);
	return SUCCESS;
}