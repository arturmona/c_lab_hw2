#include "grep_parcer.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

void test_to_lower_case(){
    int success=1;
    char * temp="Aba";
    char * temp1="AbA";
    char * temp2="123";
    char * temp3="aba";
    to_lower_case(temp);
    to_lower_case(temp1);
    to_lower_case(temp2);
    to_lower_case(temp3);
    if(strcmp(temp,"aba")||strcmp(temp1,"aba")||strcmp(temp2,"123")||strcmp(temp3,"aba"))
        success=0;
    assert(success&&"test_to_lower_case");
}

int check_first_temp(ParsedPattern* parsedPattern){
    int fail=0;
    ParsedPatternNode * root=parsedPattern->start;
    int index=0;
    while(root!=NULL){
        if(index==0){
            if (root->regular_char!='a') {
                fail = 1;
            }
        }
        if(index==1){
            if (root->regular_char!='b') {
                fail = 1;
            }
        }
        if(index==2){
            if (root->regular_char!='c') {
                fail = 1;
            }
        }
        if(index==3){
            if (root->regular_char!='d') {
                fail = 1;
            }
        }
        if (root->type!=REGULAR_CHAR)
            fail=1;
        index++;
    }
    if(index>4)
        fail=1;
    return fail;
}

int check_temp2(ParsedPattern* parsedPattern){
    int fail=0;
    ParsedPatternNode * root=parsedPattern->start;
    int index=0;
    while(root!=NULL){
        if(index==0){
            if (root->regular_char!='.') {
                fail = 1;
            }
        }
        if(index==1){
            if (root->regular_char!='{') {
                fail = 1;
            }
        }
        if(index==2){
            if (root->regular_char!='a') {
                fail = 1;
            }
        }
        if(index==3){
            if (root->regular_char!='b') {
                fail = 1;
            }
        }
        if(index==4){
            if (root->regular_char!='c') {
                fail = 1;
            }
        }
        if (root->type!=REGULAR_CHAR)
            fail=1;
        index++;
    }
    if(index>5)
        fail=1;
    return fail;
}


int check_temp3(ParsedPattern* parsedPattern){
    int fail=0;
    ParsedPatternNode * root=parsedPattern->start;
    int index=0;
    while(root!=NULL){
        if(index==0){
            if (root->type==SQUARE_BRACKETS)
                fail = 1;
            if (root->square_brackets_start_char!='a'||root->square_brackets_end_char!='c')
                fail=1;

        }
        index++;
    }
    if(index>2)
        fail++;
    return fail;
}

int check_temp4(ParsedPattern* parsedPattern){
    int fail=0;
    ParsedPatternNode * root=parsedPattern->start;
    int index=0;
    while(root!=NULL){
        if(index==0){
            if (root->type==DOT)
                fail = 1;
        }
        index++;
    }
    if(index>2)
        fail++;
    return fail;
}

int check_temp5(ParsedPattern* parsedPattern) {
    int fail = 0;
    ParsedPatternNode *root = parsedPattern->start;
    int index = 0;
    while (root != NULL) {
        if (index == 0) {
            if (root->type != ROUND_BRACKETS)
                fail = 1;
        }
        index++;
    }
    return fail;
}



void test_createParsedPattern() {
    char *temp = "abcd";
    char *temp2 = "\\.\\{abc";
    char *temp3 = "[a-c]";
    char *temp4 = ".a";
    char *temp5 = "(abc|a|)";
    ParsedPattern *parsed_temp1 = createParsedPattern(temp, 0);
    ParsedPattern *parsed_temp2 = createParsedPattern(temp, 0);
    ParsedPattern *parsed_temp3 = createParsedPattern(temp, 1);
    ParsedPattern *parsed_temp4 = createParsedPattern(temp, 1);
    ParsedPattern *parsed_temp5 = createParsedPattern(temp, 1);
    freeParsedPattern(parsed_temp1);
    freeParsedPattern(parsed_temp2);
    freeParsedPattern(parsed_temp3);
    freeParsedPattern(parsed_temp4);
    freeParsedPattern(parsed_temp5);



}