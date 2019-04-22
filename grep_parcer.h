#ifndef GREP_PARCER_H
#define GREP_PARCER_H

#define REGULAR_CHAR 0
#define DOT 1
#define SQUARE_BRACKETS 2
#define ROUND_BRACKETS 3
#define SUCCESS 1
#define FAILURE 0
#define FOUND 1
#define NOT_FOUND 0

typedef struct ParsedPattern{
    int number_of_pattern_nodes;
    struct ParsedPatternNode* start;
    struct ParsedPatternNode* end;
}ParsedPattern;

typedef struct ParsedPatternNode{
    int type;
    char regular_char;
    char** round_brackets_options_array;
    int number_of_round_brackets_options; 
    int round_brackets_content_size_bytes;
    int number_of_checked_round_brackets_options;
    char square_brackets_start_char;
    char square_brackets_end_char;
    struct ParsedPatternNode* next;
    struct ParsedPatternNode* prev;
}ParsedPatternNode;

void to_lower_case(char* word);
ParsedPattern* createParsedPattern(char* pattern, int use_regular_expressions);
void freeParsedPattern(ParsedPattern* parsed_pattern);

#endif /*GREP_PARCER_H*/