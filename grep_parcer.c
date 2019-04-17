#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "grep_parcer.h"

#define SQUARE_BRACKETS_SIZE_BYTES 5
#define DELIMITER "|"
#define NOT_USED_CHAR 'c'

void cut_pattern(char* pattern_to_cut, int index_to_cut_from, int lenght_to_cut){
    size_t pattern_size = strlen(pattern_to_cut);
    memmove(pattern_to_cut + index_to_cut_from,
            pattern_to_cut + index_to_cut_from + lenght_to_cut + 1, pattern_size - lenght_to_cut);
    pattern_to_cut[pattern_size - lenght_to_cut + 1] = '\0';
}

void safely_add_node_to_parsed_pattern(ParsedPattern* parsed_pattern, ParsedPatternNode* node){
    if (parsed_pattern->number_of_pattern_nodes == 0){
        parsed_pattern->start = node;
        parsed_pattern->end = node;
    }
    else{
        parsed_pattern->end->next = node;
        node->prev = parsed_pattern->end;
        parsed_pattern->end = node;
    }
    parsed_pattern->number_of_pattern_nodes++;
}

void set_all_pattern_options(char* all_patterns_combined, ParsedPatternNode* node){
    int pattern_index = 1;
    if(node->number_of_round_brackets_options == 1){
        node->round_brackets_options_array[0] = malloc(strlen(all_patterns_combined)+1);
        strcpy(node->round_brackets_options_array[0] ,all_patterns_combined);
    }
    else{
        strcpy(node->round_brackets_options_array[pattern_index-1],
               strtok(all_patterns_combined, DELIMITER));
        while(pattern_index < node->number_of_round_brackets_options){
            pattern_index++;
            strcpy(node->round_brackets_options_array[pattern_index-1], strtok(NULL, DELIMITER));
        }
    }
}

void initialize_regular_char_node(ParsedPatternNode* node, char regular_char) {
    node->regular_char = regular_char;
    node->type = REGULAR_CHAR;
}

void initialize_dot_node(ParsedPatternNode* node){
    node->type = DOT;
}

void initialize_sqaure_brackets_node(ParsedPatternNode* node, char start, char end){
    node->square_brackets_start_char = start;
    node->square_brackets_end_char = end;
    node->type = SQUARE_BRACKETS;
}

void initialize_round_brackets_node(ParsedPatternNode* node, char* pattern){
    int allocation_index;
    char* pattern_copy = malloc(strlen(pattern)+1);
    strcpy(pattern_copy, pattern);
    node->number_of_checked_round_brackets_options = 0;
    node->round_brackets_content_size = 0;
    node->number_of_round_brackets_options = 0;
    while (pattern[node->round_brackets_content_size] != '\\'){
        if(pattern[node->round_brackets_content_size] == '|'){
            node->number_of_round_brackets_options++;
        }
        node->round_brackets_content_size++;
    }
    cut_pattern(pattern_copy, node->round_brackets_content_size,
                (int)strlen(pattern) - node->round_brackets_content_size);
    node->number_of_round_brackets_options++;
    node->round_brackets_options_array = malloc(node->number_of_round_brackets_options);
    for(allocation_index=0; allocation_index < node->number_of_round_brackets_options; allocation_index++){
        node->round_brackets_options_array[allocation_index] = malloc(node->round_brackets_content_size + 1);
    }
    set_all_pattern_options(pattern_copy, node);
    node->type = ROUND_BRACKETS;
    free(pattern_copy);
}

ParsedPatternNode* createParsedPatternNode(){
    ParsedPatternNode* node = (ParsedPatternNode*) malloc(sizeof(ParsedPatternNode));
    if (!node){
        printf("Error: ParsedPatternNode has failed.\n");
        return NULL;
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}

ParsedPatternNode* addParsedPatternNode(ParsedPattern* parsed_pattern, char* pattern, int node_type,
                                        char regular_char, char square_brackets_start_char,
                                        char square_brackets_end_char){
    ParsedPatternNode* new_node;
    if((new_node = createParsedPatternNode()) == NULL){
        return NULL;
    }
    safely_add_node_to_parsed_pattern(parsed_pattern, new_node);
    if(node_type == REGULAR_CHAR) {
        initialize_regular_char_node(new_node, regular_char);
    }
    else if(node_type == DOT) {
        initialize_dot_node(new_node);
    }
    else if(node_type == SQUARE_BRACKETS) {
        initialize_sqaure_brackets_node(new_node, square_brackets_start_char, square_brackets_end_char);
    }
    else {
        initialize_round_brackets_node(new_node, pattern);
    }
    return new_node;
}

void freeParsedPatternNode(ParsedPatternNode* node){
    int index;
    if(node->type == ROUND_BRACKETS){
        for(index = 0; index < node->number_of_round_brackets_options; index++){
            free(node->round_brackets_options_array[index]);
        }
        free(node->round_brackets_options_array);
    }
    free(node);
}

void freeParsedPattern(ParsedPattern* parsed_pattern){
    ParsedPatternNode* temp; ParsedPatternNode* to_delete;
    if (parsed_pattern->start == NULL){
        free(parsed_pattern);
    }
    else {
        temp = parsed_pattern->start;
        while(temp->next != NULL){
            to_delete = temp;
            temp = temp->next;
            freeParsedPatternNode(to_delete);
        }
        freeParsedPatternNode(temp);
        free(parsed_pattern);
    }
}

int check_special_chars(char char_to_check){
    if ((char_to_check == '(') || (char_to_check == ')') ||
        (char_to_check == '|') || (char_to_check == '.') ||
        (char_to_check == '[') || (char_to_check == ']') ||
        (char_to_check == '{') || (char_to_check == '}')){
        return FOUND;
    }
    return NOT_FOUND;
}

int set_parsed_pattern_values(ParsedPattern* parsed_pattern, char* pattern, int use_regular_expressions){
    int pattern_index, advance_index_by_round_brackets_flag;
    ParsedPatternNode* node;
    for (pattern_index = 0; pattern[pattern_index] != '\0'; pattern_index++){
        advance_index_by_round_brackets_flag = 0;
        if (pattern[pattern_index] == '\\'){
            if (!check_special_chars(pattern[pattern_index + 1])){
                pattern_index += 1;
                node = addParsedPatternNode(parsed_pattern ,pattern, REGULAR_CHAR, pattern[pattern_index],
                                            NOT_USED_CHAR, NOT_USED_CHAR);
            }
        }
        else if ((pattern[pattern_index] == '.') && use_regular_expressions){
            node = addParsedPatternNode(parsed_pattern ,pattern, DOT, NOT_USED_CHAR,
                                        NOT_USED_CHAR, NOT_USED_CHAR);
        }
        else if ((pattern[pattern_index] == '[') && use_regular_expressions){
            node = addParsedPatternNode(parsed_pattern ,pattern, SQUARE_BRACKETS, NOT_USED_CHAR,
                                        pattern[pattern_index + 1], pattern[pattern_index + 3]);
            pattern_index += SQUARE_BRACKETS_SIZE_BYTES;
        }
        else if ((pattern[pattern_index] == '(') && use_regular_expressions){
            cut_pattern(pattern, 0, pattern_index);
            node = addParsedPatternNode(parsed_pattern ,pattern, ROUND_BRACKETS, NOT_USED_CHAR,
                                        NOT_USED_CHAR, NOT_USED_CHAR);
            advance_index_by_round_brackets_flag = 1;
        }
        else{
            node = addParsedPatternNode(parsed_pattern ,pattern, REGULAR_CHAR, pattern[pattern_index],
                                        NOT_USED_CHAR, NOT_USED_CHAR);
        }
        if (node == NULL){
            return FAILURE;
        }
        if (advance_index_by_round_brackets_flag){
            pattern_index += node->round_brackets_content_size - pattern_index + 1;
        }
    }
    return SUCCESS;
}

ParsedPattern* createParsedPattern(char* pattern, int use_regular_expressions){
    ParsedPattern* parsed_pattern = (ParsedPattern*) malloc(sizeof(ParsedPattern));
    if (!parsed_pattern){
        printf("Error: createParsedPattern has failed.\n");
        return NULL;
    }
    parsed_pattern->start = NULL;
    parsed_pattern->end = NULL;
    parsed_pattern->number_of_pattern_nodes = 0;
    if(!set_parsed_pattern_values(parsed_pattern, pattern, use_regular_expressions)){
        freeParsedPattern(parsed_pattern);
        return NULL;
    }
    return parsed_pattern;
}

void to_lower_case(char* word) {
    int index;
    for (index = 0; word[index] != '\0'; index++){
        word[index] = tolower(word[index]);
    }
}

