#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "grep.h"

void to_lower_case(char* word) {
    int index;
    for (index = 0; word[index] != '\0'; index++){
        word[index] = tolower(word[index]);
    }
}

void get_program_properties(ProgramProperties* programProperties,int number_of_program_arguments, char** program_arguments){
    int index, set_string_to_search_flag = 0;
    for(index=1; index < number_of_program_arguments+1; index++){
        if(!strcmp(program_arguments[index],"-i")){
            flags[IGNORE_UPPER_LOWER_CASE] = 1;
        }
        else if(!strcmp(program_arguments[index], "-n")){
            flags[ALSO_PRINT_LINE_NUMBER] = 1;
        }
        else if(!strcmp(program_arguments[index], "-A")){
            flags[PRINT_NUM_LINES_AFTER_MATCH] = atoi(program_arguments[index+1]);
            index++;
        }
        else if(!strcmp(program_arguments[index], "-b")){
            flags[PRINT_FILE_OFFSET_TO_LINE] = 1;
        }
        else if(!strcmp(program_arguments[index], "-c")){
            flags[ONLY_PRINT_NUM_OF_MATCHED_LINES] = 1;
        }
        else if(!strcmp(program_arguments[index], "-x")){
            flags[PRINT_ONLY_STRICT_MATCH] = 1;
        }
        else if(!strcmp(program_arguments[index], "-v")){
            flags[PRINT_ONLY_LINES_NOT_MATCHING] = 1;
        }
        else if(!strcmp(program_arguments[index], "-E")){
            flags[USE_REGULAR_EXPRESSIONS] = 1;
        }
        else if(set_string_to_search_flag == 0){
            *string_to_search = program_arguments[index];
            set_string_to_search_flag=1;
        }
        else{
            *file_to_read_from = program_arguments[index];
        }
    }
}
