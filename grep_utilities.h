#ifndef GREP_UTILITIES_H
#define GREP_UTILITIES_H

#define REGULAR_CHAR 0
#define DOT 1
#define SQUARE_BRACKETS 2
#define ROUND_BRACKETS 3

typedef struct Lines{
    char* current_line;
    int current_row_number;
    int num_of_bytes_before_current_line;
    int num_of_lines_to_print;
    int num_of_matched_lines;
}Lines;

typedef struct Parsed_pattern{
    int type;
    char the_char;
    char* string;
    char char_start;
    char char_end;
}Parsed_pattern;

void initialize_lines_struct(Lines* lines);
void initialize_type_regular_char(Parsed_pattern* parsedPattern, char the_char);
void initialize_type_dot(Parsed_pattern* parsedPattern);
void initialize_type_sqaure_brackets(Parsed_pattern* parsedPattern,
                                     char start, char end);
int initialize_type_round_brackets(Parsed_pattern* parsedPattern, char* pattern);
int check_special_chars(char a_char);

#endif /*GREP_UTILITIES_H*/
