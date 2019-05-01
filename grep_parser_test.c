#include "grep_parcer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_RESULT 0
#define VALID_RESULT 1

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
  test_to_lower_case();
  test_createParsedPattern();
}
