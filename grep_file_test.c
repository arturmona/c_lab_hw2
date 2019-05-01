#include "grep_file.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VALID_RESULT 1
#define INVALID_RESULT 0

void init_array(char** array, char* argument1, int index1, char* argument2, int index2, char* argument3, int index3)
{
  array[index1] = argument1;
  array[index2] = argument2;
  if (index3 != -1) {
    array[index3] = argument3;
  }
}

void free_array(char** array, int index_pattern)
{
  free(array[index_pattern]);
  free(array);
}

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

int check_grep_on_file_prints(GrepProperties* grep, char* array, char** temp, int size, char* expected_chars)
{
  FILE* file;
  file = freopen("tests/output.txt", "w+", stdout);
  get_grep_properties(grep, size, temp);
  grep_on_file(grep);
  fclose(file);
  file = freopen("tests/output.txt", "r", stdout);
  fgets(array, 60, file);
  if (strcmp(expected_chars, array) != 0) {
    return INVALID_RESULT;
  }
  return VALID_RESULT;
}

void test_grep_on_file()
{
  char result[60];
  int num_of_valid_results = 0;
  GrepProperties grep;
  char** array_of_arguments = calloc(5, sizeof(char*));
  array_of_arguments[1] = calloc(8, sizeof(char));
  array_of_arguments[0] = "Program";
  strcpy(array_of_arguments[1], "bla");
  init_array(array_of_arguments, "-i", 2, "tests/input.txt", 3, NULL, -1);
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, 3, "Bla\n");
  free(array_of_arguments[1]);
  init_array(array_of_arguments, "-E", 1, "-x", 3, "tests/input.txt", 4);
  array_of_arguments[2] = calloc(8, sizeof(char));
  strcpy(array_of_arguments[2], "Bl[a-c]");
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, 4, "Bla\n");
  init_array(array_of_arguments, "-E", 1, "-v", 3, "tests/input.txt", 4);
  strcpy(array_of_arguments[2], ".la");
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, 4, "daniel");
  array_of_arguments[3] = "-c";
  reset_grep_properties(&grep);
  num_of_valid_results += check_grep_on_file_prints(&grep, result, array_of_arguments, 4, "1\n");
  free_array(array_of_arguments, 2);
  assert(num_of_valid_results == 4 && "test_grep_on_file()");
}

int main()
{
  test_get_grep_properties();
  test_grep_on_file();
}