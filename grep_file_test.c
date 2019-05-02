#include "grep_file.h"
#include "grep_parcer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALID_RESULT 1
#define INVALID_RESULT 0
#define IGNORE_ARGUMENT -1
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
#define ARRAY_SIZE 15
#define DESIRE_VALID_FLAGS_OF_TEST_GREP_ON_FILE 4
#define INPUT_FILE_PATH "/specific/a/home/cc/students/cs/danielpeer/bbb/tests/input.txt"
#define OUTPUT_FILE_PATH "/specific/a/home/cc/students/cs/danielpeer/bbb/tests/output.txt"
#define NUM_OF_ARGS 4

void init_array(char** array, char* first_argument, int first_argument_index, char* second_argument,
                int second_argument_index, char* third_argument, int third_argument_index)
{
  array[first_argument_index] = first_argument;
  array[second_argument_index] = second_argument;
  if (third_argument_index != IGNORE_ARGUMENT) {
    array[third_argument_index] = third_argument;
  }
}

void free_array(char** array, int index_pattern)
{
  free(array[index_pattern]);
  free(array);
}

void reset_grep_properties(GrepProperties* grep_properties)
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

int check_grep_on_file_prints(GrepProperties* grep, char* array, char** arguments, int num_of_args,
                              char* expected_chars)
{
  FILE* file;
  file = freopen(OUTPUT_FILE_PATH, "w+", stdout);
  get_grep_properties(grep, num_of_args, arguments);
  grep_on_file(grep);
  fclose(file);
  file = freopen(OUTPUT_FILE_PATH, "r", stdout);
  fgets(array, ARRAY_SIZE, file);
  if (strcmp(expected_chars, array) != 0) {
    return INVALID_RESULT;
  }
  return VALID_RESULT;
}

void test_grep_on_file()
{
  char result[ARRAY_SIZE];
  int num_of_valid_results = 0;
  GrepProperties grep;
  char** array_of_arguments = calloc(ARRAY_SIZE, sizeof(char*));
  array_of_arguments[INDEX_ONE] = calloc(ARRAY_SIZE, sizeof(char));
  array_of_arguments[INDEX_ZERO] = "Program";
  strcpy(array_of_arguments[INDEX_ONE], "bla");
  init_array(array_of_arguments, "-i", INDEX_TWO, INPUT_FILE_PATH, INDEX_THREE, "-x", INDEX_FOUR);
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, NUM_OF_ARGS, "Bla\n");
  free(array_of_arguments[INDEX_ONE]);
  init_array(array_of_arguments, "-E", INDEX_ONE, "-x", INDEX_THREE, INPUT_FILE_PATH, INDEX_FOUR);
  array_of_arguments[INDEX_TWO] = calloc(ARRAY_SIZE, sizeof(char));
  strcpy(array_of_arguments[INDEX_TWO], "Bl[a-c]");
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, NUM_OF_ARGS, "Bla\n");
  init_array(array_of_arguments, "-E", INDEX_ONE, "-v", INDEX_THREE, INPUT_FILE_PATH, INDEX_FOUR);
  strcpy(array_of_arguments[INDEX_TWO], ".la");
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, NUM_OF_ARGS, "daniel");
  array_of_arguments[INDEX_THREE] = "-c";
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, NUM_OF_ARGS, "1\n");
  free_array(array_of_arguments, INDEX_TWO);
  assert(num_of_valid_results == DESIRE_VALID_FLAGS_OF_TEST_GREP_ON_FILE && "test_grep_on_file()");
}

int main()
{
  test_grep_on_file();
  return EXIT_SUCCESS;
}
