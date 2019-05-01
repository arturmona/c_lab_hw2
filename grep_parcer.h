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
#define ON 1
#define OFF 0
#define DASH '-'

typedef struct GrepProperties {
  char *file_to_read_from;
  char *pattern;
  int also_print_line_number;          /*n*/
  int ignore_upper_lower_case;         /*i*/
  int print_num_lines_after_match;     /*A NUM*/
  int print_file_offset_to_line;       /*b*/
  int only_print_num_of_matched_lines; /*c*/
  int print_only_strict_match;         /*x*/
  int print_only_lines_not_matching;   /*v*/
  int use_regular_expressions;         /*E*/
} GrepProperties;

typedef struct ParsedPattern {
  int number_of_pattern_nodes;
  struct ParsedPatternNode *start;
  struct ParsedPatternNode *end;
} ParsedPattern;

typedef struct ParsedPatternNode {
  int type;
  char regular_char;
  char **round_brackets_options_array;
  int number_of_round_brackets_options;
  int brackets_content_size_bytes;
  int number_of_checked_round_brackets_options;
  char square_brackets_start_char;
  char square_brackets_end_char;
  struct ParsedPatternNode *next;
  struct ParsedPatternNode *prev;
} ParsedPatternNode;

void get_grep_properties(GrepProperties *grepProperties, int num_of_args, char **program_arguments);
void to_lower_case(char *word);
ParsedPattern *createParsedPattern(char *pattern, int use_regular_expressions);
void freeParsedPattern(ParsedPattern *parsed_pattern);

#endif /*GREP_PARCER_H*/
