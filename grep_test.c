#include "grep_text.h"
#include <assert.h>
#include <stdio.h>




int check_prep_properies( const int * flags_values,const GrepProperties* grep_properties,const char * filename,const
char * pattern) {
    int num_of_equalities=0;
    num_of_equalities+=grep_properties->print_num_lines_after_match ==flags_values[0];
    num_of_equalities+=grep_properties->print_file_offset_to_line == flags_values[1];
    num_of_equalities+=grep_properties->only_print_num_of_matched_lines == flags_values[2];
    num_of_equalities+= grep_properties->ignore_upper_lower_case == flags_values[3];
    num_of_equalities+=grep_properties->also_print_line_number == flags_values[4];
    num_of_equalities+= grep_properties->print_only_lines_not_matching == flags_values[5];
    num_of_equalities+=grep_properties->print_only_strict_match == flags_values[6];
    num_of_equalities+= grep_properties->use_regular_expressions == flags_values[7];
    num_of_equalities+=grep_properties->pattern==pattern;
    num_of_equalities+=grep_properties->file_to_read_from==filename;
    if (num_of_equalities!=10)
        return 0;
    return 1;
}

int reset_grep_properties(GrepProperties* grep_properties){
    grep_properties->file_to_read_from=NULL;
    grep_properties->pattern=NULL;
    grep_properties->use_regular_expressions=0;
    grep_properties->print_only_strict_match=0;
    grep_properties->print_only_lines_not_matching=0;
    grep_properties->also_print_line_number=0;
    grep_properties->ignore_upper_lower_case=0;
    grep_properties->only_print_num_of_matched_lines=0;
    grep_properties->print_file_offset_to_line=0;
    grep_properties->print_num_lines_after_match=0;
}

void check_get_grep_properties(GrepProperties* grep_properties, int num_of_args,
                               char** program_arguments) {
    int check_valid_properties=0;
    GrepProperties grep;
    char * temp[6] = {"-n", "-b", "-A", "6", "temp1", "p.txt"};
    int temp_flags_values[8]={6,1,0,0,0,1,0,0};
    char * temp2[6] = {"temp2", "i", "v", "x", "p.txt"};
    int  temp2_flags_values[8]={0,0,0,1,0,1,1,0};
    char * temp3[5] = {"temp3","-t","-m","p.txt"};
    int  temp3_flags_values[8]={0,0,0,0,0,0,0,0};
    get_grep_properties(&grep,6,(char **)temp);
    check_valid_properties+=check_prep_properies(temp_flags_values,&grep,"p.txt","temp1");
    reset_grep_properties(&grep);
    get_grep_properties(&grep,5,(char **)temp2);
    check_valid_properties+=check_prep_properies(temp2_flags_values,&grep,"p.txt","temp1");
    reset_grep_properties(&grep);
    get_grep_properties(&grep,4,(char **)temp3);
    check_valid_properties+=check_prep_properies(temp3_flags_values,&grep,"p.txt","temp1");
    assert(check_valid_properties==3&&"check_get_grep_properties");

}