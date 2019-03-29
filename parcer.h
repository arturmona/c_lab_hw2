#ifndef PARSER_H
#define PARSER_H

void to_lower_case(char* word);
void get_program_properties(char** file_to_read_from, char** string_to_search, int* flags,
                            int number_of_program_arguments, char** program_arguments);

#endif /* PARSER_H */
