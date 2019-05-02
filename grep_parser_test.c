#include "grep_parcer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_RESULT 0
#define VALID_RESULT 1
#define FLAG_ON 1
#define FLAG_OF 0
#define INDEX_ZERO 0
#define INDEX_ONE 1
#define INDEX_TWO 2
#define INDEX_THREE 3
#define INDEX_FOUR 4
#define INDEX_FIVE 5
#define INDEX_SIX 6
#define INDEX_SIX 6
#define INDEX_SEVEN 7
#define INDEX_EIGHT 8
#define DESIRE_VALID_FLAGS_OF_CHECK_GREP_PROPERTIES 10
#define DESIRE_VALID_FLAGS_OF_TEST_CREATE_PARSED_PATTERN 4
#define DESIRE_VALID_FLAGS_OF_TEST_GET_GTEP_PROPERTIES 3
#define ARRAY_SIZE 15
#define NUM_OF_LINES_TO_PRINT "6"
#define FIRST_ARRAY_OF_ARGUMENTS_SIZE 6
#define SECOND_ARRAY_OF_ARGUMENTS_SIZE 5
#define THIRD_ARRAY_OF_ARGUMENTS_SIZE 3

int check_grep_properties(const int* flags_values, const GrepProperties* grep_properties, const char* filename,
                          const char* pattern)
{
  int num_of_valid_flags = 0;
  num_of_valid_flags += grep_properties->print_num_lines_after_match == flags_values[INDEX_ZERO];
  num_of_valid_flags += grep_properties->print_file_offset_to_line == flags_values[INDEX_ONE];
  num_of_valid_flags += grep_properties->only_print_num_of_matched_lines == flags_values[INDEX_TWO];
  num_of_valid_flags += grep_properties->ignore_upper_lower_case == flags_values[INDEX_THREE];
  num_of_valid_flags += grep_properties->also_print_line_number == flags_values[INDEX_FOUR];
  num_of_valid_flags += grep_properties->print_only_lines_not_matching == flags_values[INDEX_FIVE];
  num_of_valid_flags += grep_properties->print_only_strict_match == flags_values[INDEX_SIX];
  num_of_valid_flags += grep_properties->use_regular_expressions == flags_values[INDEX_SEVEN];
  num_of_valid_flags += grep_properties->pattern == pattern;
  num_of_valid_flags += grep_properties->file_to_read_from == filename;
  if (num_of_valid_flags != DESIRE_VALID_FLAGS_OF_CHECK_GREP_PROPERTIES) {
    return VALID_RESULT;
  }
  return INVALID_RESULT;
}

void reset_grep_properties(GrepProperties* grep_properties)
{
  grep_properties->file_to_read_from = NULL;
  grep_properties->pattern = NULL;
  grep_properties->use_regular_expressions = FLAG_OF;
  grep_properties->print_only_strict_match = FLAG_OF;
  grep_properties->print_only_lines_not_matching = FLAG_OF;
  grep_properties->also_print_line_number = FLAG_OF;
  grep_properties->ignore_upper_lower_case = FLAG_OF;
  grep_properties->only_print_num_of_matched_lines = FLAG_OF;
  grep_properties->print_file_offset_to_line = FLAG_OF;
  grep_properties->print_num_lines_after_match = FLAG_OF;
}

void test_get_grep_properties()
{
  int num_of_valid_results = 0;
  GrepProperties grep;
  char* first_array_of_arguments[INDEX_SEVEN] = {"program", "-n", "-b", "-A", NUM_OF_LINES_TO_PRINT, "pattern", "file"};
  int first_valid_flags_values[INDEX_EIGHT] = {
      atoi(NUM_OF_LINES_TO_PRINT), FLAG_ON, FLAG_OF, FLAG_OF, FLAG_OF, FLAG_ON, FLAG_OF, FLAG_OF};
  char* second_array_of_arguments[INDEX_SIX] = {"program", "pattern", "-i", "-v", "-x", "file"};
  int second_valid_flags_values[INDEX_EIGHT] = {FLAG_OF, FLAG_OF, FLAG_OF, FLAG_ON, FLAG_OF, FLAG_ON, FLAG_OF, FLAG_OF};
  char* third_array_of_arguments[INDEX_FIVE] = {"program", "-t", "-m", "file"};
  int third_valid_flags_values[INDEX_EIGHT] = {FLAG_OF, FLAG_OF, FLAG_OF, FLAG_OF, FLAG_OF, FLAG_OF, FLAG_OF, FLAG_OF};
  get_grep_properties(&grep, FIRST_ARRAY_OF_ARGUMENTS_SIZE, (char**)first_array_of_arguments);
  num_of_valid_results += check_grep_properties(first_valid_flags_values, &grep, "file", "pattern");
  reset_grep_properties(&grep);
  get_grep_properties(&grep, SECOND_ARRAY_OF_ARGUMENTS_SIZE, (char**)second_array_of_arguments);
  num_of_valid_results += check_grep_properties(second_valid_flags_values, &grep, "file", "pattern");
  reset_grep_properties(&grep);
  get_grep_properties(&grep, THIRD_ARRAY_OF_ARGUMENTS_SIZE, (char**)third_array_of_arguments);
  num_of_valid_results += check_grep_properties(third_valid_flags_values, &grep, "file", "pattern");
  assert(num_of_valid_results == DESIRE_VALID_FLAGS_OF_TEST_GET_GTEP_PROPERTIES && "check_get_grep_properties");
}

void test_to_lower_case()
{
  char* string = calloc(ARRAY_SIZE, sizeof(char));
  int result = VALID_RESULT;
  strcpy(string, "AbaA!");
  to_lower_case(string);
  if (strcmp(string, "abaa!")) {
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
    if (index == INDEX_ZERO) {
      if (root->regular_char != 'a') {
        result = INVALID_RESULT;
      }
    }
    if (index == INDEX_ONE) {
      if (root->regular_char != 'b') {
        result = INVALID_RESULT;
      }
    }
    if (index == INDEX_TWO) {
      if (root->regular_char != 'c') {
        result = INVALID_RESULT;
      }
    }
    if (index == INDEX_THREE) {
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
  int result = VALID_RESULT, index = INDEX_ZERO;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == INDEX_ZERO) {
      if (root->regular_char != '.' || root->type != REGULAR_CHAR) {
        result = INVALID_RESULT;
      }
    }
    if (index == INDEX_ONE) {
      if (root->regular_char != '{' || root->type != REGULAR_CHAR) {
        result = INVALID_RESULT;
      }
    }
    if (index == INDEX_TWO) {
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
  int result = VALID_RESULT, index = INDEX_ZERO;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == INDEX_ZERO) {
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
  int result = VALID_RESULT, index = INDEX_ZERO;
  ParsedPatternNode* root = parsedPattern->start;
  while (root != NULL) {
    if (index == INDEX_ZERO) {
      if (root->type != ROUND_BRACKETS) {
        result = INVALID_RESULT;
      }
      if (strcmp(root->round_brackets_options_array[INDEX_ZERO], "ab")) {
        result = INVALID_RESULT;
      }
      if (strcmp(root->round_brackets_options_array[INDEX_ONE], "a")) {
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
  char* string = calloc(ARRAY_SIZE, sizeof(char));
  strcpy(string, "abcd");
  parsed_string = createParsedPattern(string, FLAG_OF);
  num_of_valid_result += check_first_string(parsed_string);
  strcpy(string, "\\.\\{.");
  parsed_string = createParsedPattern(string, FLAG_ON);
  num_of_valid_result += check_second_string(parsed_string);
  strcpy(string, "[a-c]");
  parsed_string = createParsedPattern(string, FLAG_ON);
  num_of_valid_result += check_third_string(parsed_string);
  strcpy(string, "(ab|a)");
  parsed_string = createParsedPattern(string, FLAG_ON);
  num_of_valid_result += check_fourth_string(parsed_string);
  free(string);
  freeParsedPattern(parsed_string);
  assert(num_of_valid_result == DESIRE_VALID_FLAGS_OF_TEST_CREATE_PARSED_PATTERN && "test_createParsedPattern");
}

int main()
{
  test_get_grep_properties();
  test_to_lower_case();
  test_createParsedPattern();
  return EXIT_SUCCESS;
}
