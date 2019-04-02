
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grep.h"
#include <ctype.h>

#define SUCCESS 1
#define FAILURE 0

int parse_pattern(Parsed_pattern * parsed_pattern,char * pattern,int use_regular) {
	int parsed_pattern_index = 0, pattern_index;
	Parsed_pattern *current_parsed_pattern_ptr;
	for (pattern_index = 0; pattern[pattern_index] != '\0'; pattern_index++, parsed_pattern_index++) {
		current_parsed_pattern_ptr=&parsed_pattern[parsed_pattern_index];
		if (pattern[pattern_index] == '\\') {
			if (check_special_chars(pattern[pattern_index + 1])) {
				pattern_index += 1;
			}
			initialize_type_regular_char(current_parsed_pattern_ptr, pattern[pattern_index]);
		} else if (pattern[pattern_index] == '.' && use_regular) {
			initialize_type_dot(current_parsed_pattern_ptr);

		} else if (pattern[pattern_index] == '[' && use_regular) {
			init_type_sqaure_brackets(current_parsed_pattern_ptr, pattern[pattern_index+1],pattern[pattern_index + 3]);
			pattern_index += 4;
		} else if (pattern[pattern_index] == '(' && use_regular) {
			pattern_index = initialize_type_round_brackets(current_parsed_pattern_ptr, pattern);
		} else
			initialize_type_regular_char(current_parsed_pattern_ptr, pattern[pattern_index]);
	}
	return parsed_pattern_index;
}

void get_program_properties(GrepProperties*  grep_properties, int num_of_args, char** program_arguments){
	int index, pattern_set = 0;
	for(index=1; index < num_of_args+1; index++) {
		if (!strcmp(program_arguments[index], "-i")) {
			grep_properties->ignore_upper_lower_case = 1;
		} else if (!strcmp(program_arguments[index], "-n")) {
			grep_properties->also_print_line_number=1;
		} else if (!strcmp(program_arguments[index], "-A")) {
			grep_properties->print_num_lines_after_match = atoi(program_arguments[index + 1]);
			index++;
		} else if (!strcmp(program_arguments[index], "-b")) {
			grep_properties->print_file_offset_to_line=1;
		} else if (!strcmp(program_arguments[index], "-c")) {
			grep_properties->only_print_num_of_matched_lines = 1;
		} else if (!strcmp(program_arguments[index], "-x")) {
			grep_properties->print_only_strict_match = 1;
		} else if (!strcmp(program_arguments[index], "-v")) {
			grep_properties->print_only_lines_not_matching = 1;
		} else if (!strcmp(program_arguments[index], "-E")) {
			grep_properties->use_regular_expressions = 1;
		} else if (!pattern_set) {
			grep_properties->pattern= program_arguments[index];
			pattern_set=1;
		} else {
			grep_properties->file_to_read_from = program_arguments[index];
		}
	}
}

int find_match(GrepProperties* grep_properties,char* line ,Parsed_pattern* parsedPattern,int parsedPattern_size) {
	unsigned int index;
	int pos;
	int match=0;
	int match_chars;
	for (index = 0; index < strlen(line); index++) {
		match =1;
		match_chars=0;
		for (pos = 0; pos < parsedPattern_size&&pos+index<strlen(line)&&match; pos++) {
			if (parsedPattern[pos].type == REGULAR_CHAR && grep_properties->ignore_upper_lower_case) {
				match=tolower(line[pos+index])==tolower(parsedPattern[pos].the_char);
			}
			else if(parsedPattern[pos].type == REGULAR_CHAR )
				match=line[pos+index]==parsedPattern[pos].the_char;
			else if (parsedPattern[pos].type == SQUARE_BRACKETS) {
				match=parsedPattern[pos].char_start<=line[pos+index]&&parsedPattern[pos].char_end <= line[pos+index];
			}
			else if (parsedPattern[pos].type == ROUND_BRACKETS) {
			}
			match_chars+=match;
		}

		if(grep_properties->print_only_strict_match) {
			if (match_chars>=parsedPattern_size && (pos + index >= strlen(line)||line[pos + index]=='\n')&&index==0) {
				break;
			}
		}
		else if (match_chars>=parsedPattern_size){
			break;
		}
	}
	return match;
}


int check_match(GrepProperties* grep_properties,Lines * lines){
	int match;
	int use_regular_expressions=grep_properties->use_regular_expressions;
	int parsed_pattern_length;
	Parsed_pattern parsed_pattern[strlen(grep_properties->pattern)];
	parsed_pattern_length=parse_pattern(parsed_pattern,grep_properties->pattern,use_regular_expressions);
	match=find_match(grep_properties,lines->current_line, parsed_pattern,parsed_pattern_length);
	if (grep_properties->print_only_lines_not_matching)
		match^=1;
	if (match)
		lines->num_of_matched_lines++;
	return match;
}

void print_matches(GrepProperties * grep_properties,Lines * lines ,char Delimiter){
	if(grep_properties->also_print_line_number)
		printf("%d%c", lines->current_row_number,Delimiter);

	if(grep_properties->print_file_offset_to_line)
		printf("%d%c", lines->num_of_bytes_before_current_line,Delimiter);
	printf("%s",lines->current_line);
}

void grep_line(GrepProperties* program_properties,Lines * lines) {
	int is_match;
	is_match = check_match(program_properties, lines);
	if (program_properties->only_print_num_of_matched_lines)
		return;
	if (is_match) {
		print_matches(program_properties,lines,':');
		if (program_properties->print_num_lines_after_match) {
			lines->num_of_lines_to_print= program_properties->print_num_lines_after_match;
		}
	}
	else if (lines->num_of_lines_to_print > 0){
		print_matches(program_properties,lines, '-');
		lines->num_of_lines_to_print--;
	}
}

int grep_on_file(GrepProperties* grep_properties) {
	FILE *file;
	size_t line_lenght = 0;
	ssize_t amount_chars_read;
	Lines lines;
	init_lines_struct(&lines);
	if (grep_properties->file_to_read_from == NULL) {
		file = stdin;
	} else {
		file = fopen(grep_properties->file_to_read_from, "r");
		if (file == NULL) {
			printf("Error: could not open file '%s'.\n", grep_properties->file_to_read_from);
			return FAILURE;
		}
	}
	while ((amount_chars_read = getline(&lines.current_line, &line_lenght, file)) != -1) {
		grep_line(grep_properties, &lines);
		lines.current_row_number++;
		lines.num_of_bytes_before_current_line += amount_chars_read;
	}
	if (grep_properties->only_print_num_of_matched_lines)
		printf("%d\n", lines.num_of_matched_lines);
	free(lines.current_line);
	fclose(file);
	return SUCCESS;
}