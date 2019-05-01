#include "grep_parcer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_RESULT 0
#define VALID_RESULT 1

int check_grep_properties(const int* flags_values, const GrepProperties* grep_properties, const char* filename,
                          const char* pattern)
{
  int num_of_valid_flags = 0;
  num_of_valid_flags += grep_properties->print_num_lines_after_match == flags_values[0];
  num_of_valid_flags += grep_properties->print_file_offset_to_line == flags_values[1];
  num_of_valid_flags += grep_properties->only_print_num_of_matched_lines == flags_values[2];
  num_of_valid_flags += grep_properties->ignore_upper_lower_case == flags_values[3];
  num_of_valid_flags += grep_properties->also_print_line_number == flags_values[4];
  num_of_valid_flags += grep_properties->print_only_lines_not_matching == flags_values[5];
  num_of_valid_flags += grep_properties->print_only_strict_match == flags_values[6];
  num_of_valid_flags += grep_properties->use_regular_expressions == flags_values[7];
  num_of_valid_flags += grep_properties->pattern == pattern;
  num_of_valid_flags += grep_properties->file_to_read_from == filename;
  if (num_of_valid_flags != 10) {
    return VALID_RESULT;
  }
  return INVALID_RESULT;
}

int reset_grep_properties(GrepProperties* grep_properties)
{
  grep_properties->file_to_read_from = NULL;
  grep_properties->pattern = NULL;
  grep_properties->use_regular_expressions = 0;
  grep_properties->print_only_strict_match = 0;
  grep_properties->print_only_lines_not_matching = 0;
  grep_properties->also_print_line_number = 0;
  grep_properties->ignore_upper_lower_case = 0;
  grep_properties->only_print_num_of_matched_lines = 0;
  grep_properties->print_file_offset_to_line = 0;
  grep_properties->print_num_lines_after_match = 0;
}

void test_get_grep_properties()
{
  int num_of_valid_results = 0;
  GrepProperties grep;
  char* array_of_arguments_1[7] = {"program", "-n", "-b", "-A", "6", "temp1", "tests/input.txt"};
  int valid_flags_values_1[8] = {6, 1, 0, 0, 0, 1, 0, 0};
  char* array_of_arguments_2[6] = {"program", "temp2", "-i", "-v", "-x", "tests/input.txt"};
  int valid_flags_values_2[8] = {0, 0, 0, 1, 0, 1, 1, 0};
  char* array_of_arguments_3[5] = {"program", "-t", "-m", "tests/input.txt"};
  int valid_flags_values_3[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  get_grep_properties(&grep, 6, (char**)array_of_arguments_1);
  num_of_valid_results += check_grep_properties(valid_flags_values_1, &grep, "tests/input.txt", "temp1");
  reset_grep_properties(&grep);
  get_grep_properties(&grep, 5, (char**)array_of_arguments_2);
  num_of_valid_results += check_grep_properties(valid_flags_values_2, &grep, "tests/input.txt", "temp1");
  reset_grep_properties(&grep);
  get_grep_properties(&grep, 3, (char**)array_of_arguments_3);
  num_of_valid_results += check_grep_properties(valid_flags_values_3, &grep, "tests/input.txt", "temp1");
  assert(num_of_valid_results == 3 && "check_get_grep_properties");
}



void test_to_lower_case()
{
  char* string = calloc(4, sizeof(char));
  int result = VALID_RESULT;
  strcpy(string, "AbaA31!");
  to_lower_case(string);
  if (strcmp(string, "abaa31!")) {
    result = INVALID_RESULT;
  }
  free(string);
  assert(result == VALID_RESULT && "test_to_lower_case");
}

int check_first_string(ParsedPattern* parsedPattern)
{
  int result = VALID_RESULT, index = 0;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == 0) {
      if (root->regular_char != 'a') {
        result = INVALID_RESULT;
      }
    }
    if (index == 1) {
      if (root->regular_char != 'b') {
        result = INVALID_RESULT;
      }
    }
    if (index == 2) {
      if (root->regular_char != 'c') {
        result = INVALID_RESULT;
      }
    }
    if (index == 3) {
      if (root->regular_char != 'd') {
        result = INVALID_RESULT;
      }
    }
    if (root->type != REGULAR_CHAR) {
      result = INVALID_RESULT;
    }
    index++;
    root = root->next;
  }
  if (index != parsedPattern->number_of_pattern_nodes) {
    result = INVALID_RESULT;
  }
  return result;
}

int check_second_string(ParsedPattern* parsedPattern)
{
  int result = VALID_RESULT, index = 0;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == 0) {
      if (root->regular_char != '.' || root->type != REGULAR_CHAR) {
        result = INVALID_RESULT;
      }
    }
    if (index == 1) {
      if (root->regular_char != '{' || root->type != REGULAR_CHAR) {
        result = INVALID_RESULT;
      }
    }
    if (index == 2) {
      if (root->type != DOT) {
        result = INVALID_RESULT;
      }
    }
    index++;
    root = root->next;
  }
  if (index != parsedPattern->number_of_pattern_nodes) {
    result = INVALID_RESULT;
  }
  return result;
}

int check_third_string(ParsedPattern* parsedPattern)
{
  int result = VALID_RESULT, index = 0;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == 0) {
      if (root->type != SQUARE_BRACKETS) {
        result = INVALID_RESULT;
      }
      if (root->square_brackets_start_char != 'a' || root->square_brackets_end_char != 'c') {
        result = INVALID_RESULT;
      }
    }
    index++;
    root = root->next;
  }
  if (index != parsedPattern->number_of_pattern_nodes) {
    result = INVALID_RESULT;
  }
  return result;
}

int check_fourth_string(ParsedPattern* parsedPattern)
{
  int result = VALID_RESULT, index = 0;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == 0) {
      if (root->type != ROUND_BRACKETS) {
        result = INVALID_RESULT;
      }
      if (strcmp(root->round_brackets_options_array[0], "ab")) {
        result = INVALID_RESULT;
      }
      if (strcmp(root->round_brackets_options_array[1], "a")) {
        result = INVALID_RESULT;
      }
    }
    root = root->next;
    index++;
  }
  if (index != parsedPattern->number_of_pattern_nodes) {
    result = INVALID_RESULT;
  }
  return result;
}

void test_createParsedPattern()
{
  int num_of_valid_result = 0;
  ParsedPattern* parsed_string;
  char* string = calloc(9, sizeof(char));
  strcpy(string, "abcd");
  parsed_string = createParsedPattern(string, 0);
  num_of_valid_result += check_first_string(parsed_string);
  strcpy(string, "\\.\\{.");
  parsed_string = createParsedPattern(string, 1);
  num_of_valid_result += check_second_string(parsed_string);
  strcpy(string, "[a-c]");
  parsed_string = createParsedPattern(string, 1);
  num_of_valid_result += check_third_string(parsed_string);
  strcpy(string, "(ab|a)");
  parsed_string = createParsedPattern(string, 1);
  num_of_valid_result += check_fourth_string(parsed_string);
  free(string);
  freeParsedPattern(parsed_string);
  assert(num_of_valid_result == 4 && "test_createParsedPattern");
}

int main()
{
  test_get_grep_properties();
  test_to_lower_case();
  test_createParsedPattern();
}
