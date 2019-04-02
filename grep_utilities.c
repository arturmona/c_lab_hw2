#include "grep_utilities.h"
#include "stdio.h"

void init_lines_struct(Lines * lines){
    lines->current_line=NULL;
    lines->num_of_bytes_before_current_line = 0;
    lines->num_of_matched_lines = 0;
    lines->num_of_bytes_before_current_line=0;
    lines->current_row_number=1;
    lines->num_of_lines_to_print=0;
}

void initialize_type_regular_char(Parsed_pattern * parsedPattern,char the_char) {
    parsedPattern->the_char = the_char;
    parsedPattern->type = REGULAR_CHAR;
}

void initialize_type_dot(Parsed_pattern * parsedPattern){
    parsedPattern->the_char='.';
    parsedPattern->type=DOT;
}

void init_type_sqaure_brackets(Parsed_pattern * parsedPattern, char start,char end) {
    parsedPattern->char_start = start;
    parsedPattern->char_end = end;
    parsedPattern->type = SQUARE_BRACKETS;
}
int initialize_type_round_brackets(Parsed_pattern * parsedPattern,char * pattern) {
    int pattern_index = 0;
    while (pattern[pattern_index]!=')')
        pattern_index++;
    pattern[pattern_index]='\0';
    parsedPattern->string = &pattern[pattern_index+1];
    parsedPattern->type =ROUND_BRACKETS ;
    return pattern_index;
}

int check_special_chars(char  char_to_check) {
    if (char_to_check == '(' || char_to_check == ')')
        return 1;
    if (char_to_check == '|' || char_to_check == '.')
        return 1;
    if (char_to_check == '[' || char_to_check == ']')
        return 1;
    if (char_to_check == '{' || char_to_check == '}')
        return 1;
    return 0;
}


