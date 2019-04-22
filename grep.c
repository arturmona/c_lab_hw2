#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grep_parcer.h"
#include "grep.h"

#define ON 1
#define OFF 0
#define DOTS ':'
#define HYPHEN '-'

typedef struct LinesToGrep{
    char* current_line;
    int current_row_number;
    int num_of_bytes_before_current_line;
    int num_of_lines_to_print;
    int num_of_matched_lines;
}LinesToGrep;

void initializeLinesToGrep(LinesToGrep* lines){
    lines->current_line = NULL;
    lines->num_of_bytes_before_current_line = 0;
    lines->num_of_matched_lines = 0;
    lines->num_of_bytes_before_current_line = 0;
    lines->current_row_number = 1;
    lines->num_of_lines_to_print = 0;
}

void print_matches(GrepProperties* grep_properties, LinesToGrep* lines , char Delimiter){
    if(grep_properties->also_print_line_number) {
        printf("%d%c", lines->current_row_number, Delimiter);
    }
    if(grep_properties->print_file_offset_to_line) {
        printf("%d%c", lines->num_of_bytes_before_current_line, Delimiter);
    }
    printf("%s",lines->current_line);
}

int try_backtrack_in_round_brackets(ParsedPatternNode** pattern_node){
    //printf("in try_backtrack_in_round_brackets\n");
    ParsedPatternNode* temp = (*pattern_node)->prev;
    int byte_amount_to_backtrack = 0;
    while(temp != NULL){
        if((temp->type == ROUND_BRACKETS) &&
           (temp->number_of_checked_round_brackets_options < temp->number_of_round_brackets_options)){
            byte_amount_to_backtrack += \
			   strlen(temp->round_brackets_options_array[temp->number_of_checked_round_brackets_options-1]);
            (*pattern_node) = temp;
            //printf("went back %d bytes\n", byte_amount_to_backtrack);
            return byte_amount_to_backtrack;
        } else if(temp->type == ROUND_BRACKETS){
			byte_amount_to_backtrack += \
			   strlen(temp->round_brackets_options_array[temp->number_of_checked_round_brackets_options-1]);
 	
		} else {
			byte_amount_to_backtrack++;
		}
		temp = temp->prev;
    }
    //printf("went back 0 bytes\n");
    return FAILURE;
}

int check_for_match_in_round_brackets(ParsedPatternNode* node, char* line_to_search){
    //printf("in check_for_match_in_round_brackets\n");
    //printf("line is: %s\n", line_to_search);
    int search_index;
    size_t bracket_option_size;
    size_t line_to_search_lenght = strlen(line_to_search) - 1;
	if((int)line_to_search_lenght == -1){
			exit(EXIT_FAILURE);
		}
    for(search_index = node->number_of_checked_round_brackets_options;
        search_index < node->number_of_round_brackets_options;
        search_index++){
        bracket_option_size = strlen(node->round_brackets_options_array[search_index]);
        //printf("comparing: %s with %s\n",node->round_brackets_options_array[search_index], line_to_search);
        //printf("line_to_search_lenght: %d , bracket_option_size: %d\n", line_to_search_lenght, bracket_option_size);
        if((line_to_search_lenght >= bracket_option_size) &&
           (!strncmp(node->round_brackets_options_array[search_index],
                     line_to_search, bracket_option_size))){
            search_index++;
            node->number_of_checked_round_brackets_options = search_index;
			//printf("found match in op number %d\n", search_index-1);
            //printf("return is %d\n", bracket_option_size);
            return (int)bracket_option_size;
        }
    }
    //printf("return not_found\n");
    return NOT_FOUND;
}

unsigned int search_pattern_recursively(ParsedPatternNode** pattern_node,
                                        int* matched_chars, char* line_to_search,
                                        int* search_index){
    //printf("in search_pattern_recursively.\n");

    size_t line_size = strlen(line_to_search) - 1;
    //printf("search_index: %d ,line_size: %d, line: %s\n", *search_index, line_size, line_to_search);
    while(((*pattern_node) != NULL) && (*matched_chars) && ((*search_index) < (int)line_size)){
        if ((*pattern_node)->type == REGULAR_CHAR){
            *matched_chars = (line_to_search[(*search_index)] == (*pattern_node)->regular_char);
            //printf("matching node char: %c with: %c return is %d\n", (*pattern_node)->regular_char,
            		//line_to_search[(*search_index)], *matched_chars);
        } else if((*pattern_node)->type == DOT){
            *matched_chars = 1;
            //printf("matched dot\n");
        } else if ((*pattern_node)->type == SQUARE_BRACKETS){
            *matched_chars = ((*pattern_node)->square_brackets_start_char <= line_to_search[(*search_index)] &&
                              (*pattern_node)->square_brackets_end_char >= line_to_search[(*search_index)]);
            //printf("matched sr char: %c is %d\n", line_to_search[(*search_index)], *matched_chars);
        } else if ((*pattern_node)->type == ROUND_BRACKETS) {
			//printf("matching from ROUND_BRACKETS\n");
			if((*search_index)==-15){
				exit(EXIT_FAILURE);
			}
            *matched_chars = check_for_match_in_round_brackets((*pattern_node), line_to_search+(*search_index));
        }
        (*search_index) += (*matched_chars);
		if ((*matched_chars)){
			//printf("updated index: %d\n", (*search_index));
			(*pattern_node) = (*pattern_node)->next;
		}
    }
    return (*search_index);
}

void initilize_recursion_flags(ParsedPattern* parsed_pattern){
    //printf("in initilize_recursion_flags\n");
    ParsedPatternNode* pattern_node = parsed_pattern->start;
    while(pattern_node != NULL){
        if(pattern_node->type == ROUND_BRACKETS){
            pattern_node->number_of_checked_round_brackets_options = 0;
        }
        pattern_node = pattern_node->next;
    }
}

int find_pattern_in_line(GrepProperties* grep_properties, char* line, ParsedPattern* parsed_pattern){
    //printf("in find_pattern_in_line\n");
	int continue_search_flag, total_matched_chars, iteration_found_match, search_index, line_index = 0, 
		line_size, backtrack_round_brackets;
    ParsedPatternNode* pattern_node;
    char* line_copy = malloc(strlen(line)+1);
    line_copy[strlen(line)] = '\0';
    strcpy(line_copy, line);
    if(grep_properties->ignore_upper_lower_case){
        to_lower_case(line_copy);
    }
    line_size = (int)(strlen(line)-1);
	//printf("line size: %d\n", line_size);
    while (line_index < line_size) {
        pattern_node = parsed_pattern->start;
        total_matched_chars = 0; search_index = 0;
        initilize_recursion_flags(parsed_pattern);
        continue_search_flag = ON;
        while(continue_search_flag){
			iteration_found_match = 1;
            total_matched_chars = search_pattern_recursively(&pattern_node, &iteration_found_match,
                                                             line_copy+line_index, &search_index);
            if ((grep_properties->print_only_strict_match == ON) && (iteration_found_match) &&
                (total_matched_chars == line_size && (pattern_node == NULL))){
                free(line_copy);
                return FOUND;
            } else if (pattern_node == NULL && iteration_found_match){
                free(line_copy);
                return FOUND;
            } else if (total_matched_chars == line_size && pattern_node != NULL){
				free(line_copy);
                return NOT_FOUND;				
			}
            if ((backtrack_round_brackets = try_backtrack_in_round_brackets(&pattern_node)) == 0){
                continue_search_flag = OFF;
                line_index++;
            } else{
                search_index -= backtrack_round_brackets;
            }
        }
    }
    free(line_copy);
    return NOT_FOUND;
}

int check_for_match(GrepProperties* grep_properties, LinesToGrep* lines, ParsedPattern* parsed_pattern){
    int match_flag;
    match_flag = find_pattern_in_line(grep_properties, lines->current_line, parsed_pattern);
    if (grep_properties->print_only_lines_not_matching) {
        match_flag ^= 1; /*xor with 1 to flip the result*/
    }
    if (match_flag) {
        lines->num_of_matched_lines++;
    }
    return match_flag;
}

void grep_line(GrepProperties* program_properties, LinesToGrep* lines, ParsedPattern* parsed_pattern) {
    int is_match;
    is_match = check_for_match(program_properties, lines, parsed_pattern);
    if (program_properties->only_print_num_of_matched_lines) {
        return;
    }
    if (is_match) {
        print_matches(program_properties, lines, DOTS);
        if (program_properties->print_num_lines_after_match) {
            lines->num_of_lines_to_print = program_properties->print_num_lines_after_match;
        }
    }
    else if (lines->num_of_lines_to_print > 0){
        print_matches(program_properties, lines, HYPHEN);
        lines->num_of_lines_to_print--;
    }
}

int grep_on_file(GrepProperties* grep_properties) {
    FILE* file;
    size_t line_lenght = 0;
    ssize_t amount_chars_read;
    LinesToGrep lines;
    if(grep_properties->ignore_upper_lower_case){
        to_lower_case(grep_properties->pattern);
    }
    ParsedPattern* parsed_pattern = createParsedPattern(grep_properties->pattern,
                                                        grep_properties->use_regular_expressions);
    if(parsed_pattern == NULL){
        return FAILURE;
    }
    /* prints for testing..
    ParsedPatternNode* node = parsed_pattern->start;
    while(node != NULL){
        if(node->type == REGULAR_CHAR || node->type == DOT)
            printf("node char is %c\n", node->regular_char);
        else if(node->type == SQUARE_BRACKETS)
            printf("node start_char is: %c, end is: %c\n", node->square_brackets_start_char, node->square_brackets_end_char);
        else if(node->type == ROUND_BRACKETS){
            printf("round, options are:\n");
            for(int i=0; i<node->number_of_round_brackets_options; i++){
                printf("option %d is: %s\n",i,node->round_brackets_options_array[i]);
            }
        }
        node = node->next;
    }
	*/
    initializeLinesToGrep(&lines);
    if (grep_properties->file_to_read_from == NULL) {
        file = stdin;
    }
    else {
        file = fopen(grep_properties->file_to_read_from, "r");
        if (file == NULL) {
            printf("Error: could not open file '%s'.\n", grep_properties->file_to_read_from);
            return FAILURE;
        }
    }
    while ((amount_chars_read = getline(&lines.current_line, &line_lenght, file)) != -1) {
        grep_line(grep_properties, &lines, parsed_pattern);
        lines.current_row_number++;
        lines.num_of_bytes_before_current_line += amount_chars_read;
    }
    if (grep_properties->only_print_num_of_matched_lines){
        printf("%d\n", lines.num_of_matched_lines);
    }
    free(lines.current_line);
    freeParsedPattern(parsed_pattern);
    fclose(file);
    return SUCCESS;
}

/*
* we did not split get_grep_properties to several functions
* because it makes more sencse and it is better understood if keeping it as one.
*/
void get_grep_properties(GrepProperties* grep_properties, int num_of_args,
                         char** program_arguments){
    int index, pattern_set_flag = 0;
    for(index=1; index < num_of_args+1; index++) {
        if (!strcmp(program_arguments[index], "-i")) {
            grep_properties->ignore_upper_lower_case = ON;
        }
        else if (!strcmp(program_arguments[index], "-n")) {
            grep_properties->also_print_line_number = ON;
        }
        else if (!strcmp(program_arguments[index], "-A")) {
            grep_properties->print_num_lines_after_match = atoi(program_arguments[index + 1]);
            index++;
        }
        else if (!strcmp(program_arguments[index], "-b")) {
            grep_properties->print_file_offset_to_line = ON;
        }
        else if (!strcmp(program_arguments[index], "-c")) {
            grep_properties->only_print_num_of_matched_lines = ON;
        }
        else if (!strcmp(program_arguments[index], "-x")) {
            grep_properties->print_only_strict_match = ON;
        }
        else if (!strcmp(program_arguments[index], "-v")) {
            grep_properties->print_only_lines_not_matching = ON;
        }
        else if (!strcmp(program_arguments[index], "-E")) {
            grep_properties->use_regular_expressions = ON;
        }
        else if (!pattern_set_flag) {
            grep_properties->pattern = program_arguments[index];
            pattern_set_flag = ON;
        }
        else {
            grep_properties->file_to_read_from = program_arguments[index];
        }
    }
}