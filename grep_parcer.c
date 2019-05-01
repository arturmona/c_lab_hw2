#include "grep_parcer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NON_CHOOSEN -1
#define OPENING_BRACKET_SIZE_BYTES 1
#define OPENING_SQUARE_BRACKET '['
#define CLOSING_SQUARE_BRACKET ']'
#define OPENING_ROUND_BRACKET '('
#define CLOSING_ROUND_BRACKET ')'
#define OPENING_CURVED_BRACKET '{'
#define CLOSING_CURVED_BRACKET '}'
#define DOT_SIGN '.'
#define DELIMITER "|"
#define DELIMITER_CHAR '|'

void to_lower_case(char *word)
{
  int index;
  for (index = 0; word[index] != '\0'; index++) {
    word[index] = tolower(word[index]);
  }
}

int check_special_chars(char char_to_check)
{
  if ((char_to_check == DELIMITER_CHAR) || (char_to_check == DOT_SIGN) || (char_to_check == OPENING_SQUARE_BRACKET) ||
      (char_to_check == CLOSING_CURVED_BRACKET) || (char_to_check == OPENING_SQUARE_BRACKET) ||
      (char_to_check == CLOSING_SQUARE_BRACKET)) {
    return FOUND;
  }
  return NOT_FOUND;
}

void cut_pattern_start(char *pattern_to_cut, int start_index_of_kept_pattern)
{
  size_t pattern_size = strlen(pattern_to_cut);
  size_t new_pattern_size = pattern_size - start_index_of_kept_pattern;
  memmove(pattern_to_cut, pattern_to_cut + start_index_of_kept_pattern, new_pattern_size);
  pattern_to_cut[new_pattern_size] = '\0';
}

void initialize_regular_char_node(ParsedPatternNode *node, char regular_char)
{
  node->regular_char = regular_char;
  node->type = REGULAR_CHAR;
}

void initialize_dot_node(ParsedPatternNode *node) { node->type = DOT; }

void initialize_sqaure_brackets_node(ParsedPatternNode *node, char *pattern, int pattern_index)
{
  int found_valid_char, found_start_char_flag = OFF;
  char tested_char;
  cut_pattern_start(pattern, pattern_index + OPENING_BRACKET_SIZE_BYTES);
  node->brackets_content_size_bytes = 0;
  while (pattern[node->brackets_content_size_bytes] != CLOSING_SQUARE_BRACKET) {
    tested_char = pattern[node->brackets_content_size_bytes];
    found_valid_char = (tested_char != ' ' && tested_char != '\t' && tested_char != DASH);
    if (found_valid_char && !found_start_char_flag) {
      node->square_brackets_start_char = tested_char;
      found_start_char_flag = ON;
    } else if (found_valid_char) {
      node->square_brackets_end_char = tested_char;
    }
    node->brackets_content_size_bytes++;
  }
  node->type = SQUARE_BRACKETS;
}

void set_all_pattern_options_for_round_bracket_node(char *all_patterns_combined, ParsedPatternNode *node)
{
  int pattern_index = 1;
  if (node->number_of_round_brackets_options == 1) {
    strcpy(node->round_brackets_options_array[0], all_patterns_combined);
  } else {
    strcpy(node->round_brackets_options_array[pattern_index - 1], strtok(all_patterns_combined, DELIMITER));
    while (pattern_index < node->number_of_round_brackets_options) {
      pattern_index++;
      strcpy(node->round_brackets_options_array[pattern_index - 1], strtok(NULL, DELIMITER));
    }
  }
}

void set_round_bracket_node_values(ParsedPatternNode *node, const char *pattern)
{
  int allocation_index;
  node->number_of_checked_round_brackets_options = 0;
  node->brackets_content_size_bytes = 0;
  node->number_of_round_brackets_options = 0;
  while (pattern[node->brackets_content_size_bytes] != CLOSING_ROUND_BRACKET) {
    if (pattern[node->brackets_content_size_bytes] == DELIMITER_CHAR) {
      node->number_of_round_brackets_options++;
    }
    node->brackets_content_size_bytes++;
  }
  node->number_of_round_brackets_options++;
  node->round_brackets_options_array = malloc(sizeof(char *) * (node->number_of_round_brackets_options + 1));
  node->round_brackets_options_array[node->number_of_round_brackets_options] = '\0';
  for (allocation_index = 0; allocation_index < node->number_of_round_brackets_options; allocation_index++) {
    node->round_brackets_options_array[allocation_index] =
        malloc(sizeof(char *) * (node->brackets_content_size_bytes + 1));
    node->round_brackets_options_array[allocation_index][node->brackets_content_size_bytes] = '\0';
  }
}

void initialize_round_brackets_node(ParsedPatternNode *node, char *pattern, int pattern_index)
{
  cut_pattern_start(pattern, pattern_index + OPENING_BRACKET_SIZE_BYTES);
  set_round_bracket_node_values(node, pattern);
  char *pattern_copy = malloc(strlen(pattern) + 1);
  strcpy(pattern_copy, pattern);
  pattern_copy[node->brackets_content_size_bytes] = '\0';
  set_all_pattern_options_for_round_bracket_node(pattern_copy, node);
  free(pattern_copy);
  node->type = ROUND_BRACKETS;
}

ParsedPatternNode *createParsedPatternNode()
{
  ParsedPatternNode *node = (ParsedPatternNode *)malloc(sizeof(ParsedPatternNode));
  if (!node) {
    printf("Error: ParsedPatternNode has failed.\n");
    return NULL;
  }
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void safely_add_node_to_parsed_pattern(ParsedPattern *parsed_pattern, ParsedPatternNode *node)
{
  if (parsed_pattern->number_of_pattern_nodes == 0) {
    parsed_pattern->start = node;
    parsed_pattern->end = node;
  } else {
    parsed_pattern->end->next = node;
    node->prev = parsed_pattern->end;
    parsed_pattern->end = node;
  }
  parsed_pattern->number_of_pattern_nodes++;
}

ParsedPatternNode *addParsedPatternNode(ParsedPattern *parsed_pattern, char *pattern, int node_type, int pattern_index)
{
  ParsedPatternNode *new_node;
  if ((new_node = createParsedPatternNode()) == NULL) {
    return NULL;
  }
  safely_add_node_to_parsed_pattern(parsed_pattern, new_node);
  if (node_type == REGULAR_CHAR) {
    initialize_regular_char_node(new_node, pattern[pattern_index]);
  } else if (node_type == DOT) {
    initialize_dot_node(new_node);
  } else if (node_type == SQUARE_BRACKETS) {
    initialize_sqaure_brackets_node(new_node, pattern, pattern_index);
  } else {
    initialize_round_brackets_node(new_node, pattern, pattern_index);
  }
  return new_node;
}

void freeParsedPatternNode(ParsedPatternNode *node)
{
  int index;
  if (node->type == ROUND_BRACKETS) {
    for (index = 0; index < node->number_of_round_brackets_options; index++) {
      free(node->round_brackets_options_array[index]);
    }
    free(node->round_brackets_options_array);
  }
  free(node);
}

void freeParsedPattern(ParsedPattern *parsed_pattern)
{
  ParsedPatternNode *temp;
  ParsedPatternNode *to_delete;
  if (parsed_pattern->start == NULL) {
    free(parsed_pattern);
  } else {
    temp = parsed_pattern->start;
    while (temp->next != NULL) {
      to_delete = temp;
      temp = temp->next;
      freeParsedPatternNode(to_delete);
    }
    freeParsedPatternNode(temp);
    free(parsed_pattern);
  }
}

int set_parsed_pattern_values(ParsedPattern *parsed_pattern, char *pattern, int use_regular_expressions)
{
  int pattern_index, advance_index_by_brackets_size_flag, node_type;
  ParsedPatternNode *node;
  for (pattern_index = 0; pattern[pattern_index] != '\0'; pattern_index++) {
    advance_index_by_brackets_size_flag = 0;
    node_type = NON_CHOOSEN;
    if (pattern[pattern_index] == '\\') {
      if (check_special_chars(pattern[pattern_index + 1])) {
        pattern_index += 1;
      }
      node_type = REGULAR_CHAR;
    } else if ((pattern[pattern_index] == DOT_SIGN) && use_regular_expressions) {
      node_type = DOT;
    } else if ((pattern[pattern_index] == OPENING_SQUARE_BRACKET) && use_regular_expressions) {
      node_type = SQUARE_BRACKETS;
      advance_index_by_brackets_size_flag = ON;
    } else if ((pattern[pattern_index] == OPENING_ROUND_BRACKET) && use_regular_expressions) {
      node_type = ROUND_BRACKETS;
      advance_index_by_brackets_size_flag = 1;
    } else {
      node_type = REGULAR_CHAR;
    }
    if (node_type != NON_CHOOSEN) {
      if (!(node = addParsedPatternNode(parsed_pattern, pattern, node_type, pattern_index))) {
        return FAILURE;
      }
      if (advance_index_by_brackets_size_flag) {
        pattern_index += node->brackets_content_size_bytes - pattern_index;
      }
    }
  }
  return SUCCESS;
}

ParsedPattern *createParsedPattern(char *pattern, int use_regular_expressions)
{
  ParsedPattern *parsed_pattern = (ParsedPattern *)malloc(sizeof(ParsedPattern));
  if (!parsed_pattern) {
    printf("Error: createParsedPattern has failed.\n");
    return NULL;
  }
  parsed_pattern->start = NULL;
  parsed_pattern->end = NULL;
  parsed_pattern->number_of_pattern_nodes = 0;
  if (!set_parsed_pattern_values(parsed_pattern, pattern, use_regular_expressions)) {
    freeParsedPattern(parsed_pattern);
    return NULL;
  }
  return parsed_pattern;
}

void get_grep_properties(GrepProperties *grep_properties, int num_of_args, char **program_arguments)
{
  int index, pattern_set_flag = 0;
  for (index = 1; index < num_of_args + 1; index++) {
    if (!strcmp(program_arguments[index], "-i")) {
      grep_properties->ignore_upper_lower_case = ON;
    } else if (!strcmp(program_arguments[index], "-n")) {
      grep_properties->also_print_line_number = ON;
    } else if (!strcmp(program_arguments[index], "-A")) {
      grep_properties->print_num_lines_after_match = atoi(program_arguments[index + 1]);
      index++;
    } else if (!strcmp(program_arguments[index], "-b")) {
      grep_properties->print_file_offset_to_line = ON;
    } else if (!strcmp(program_arguments[index], "-c")) {
      grep_properties->only_print_num_of_matched_lines = ON;
    } else if (!strcmp(program_arguments[index], "-x")) {
      grep_properties->print_only_strict_match = ON;
    } else if (!strcmp(program_arguments[index], "-v")) {
      grep_properties->print_only_lines_not_matching = ON;
    } else if (!strcmp(program_arguments[index], "-E")) {
      grep_properties->use_regular_expressions = ON;
    } else if (!pattern_set_flag) {
      grep_properties->pattern = program_arguments[index];
      pattern_set_flag = ON;
    } else {
      grep_properties->file_to_read_from = program_arguments[index];
    }
  }
}
