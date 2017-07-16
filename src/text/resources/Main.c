#include <stdio.h>
#include <string.h>
#include <fllib.h>

typedef struct 
{
    FlCstr code;
    FlCstr name;
    FlCstr numerical_value_1;
    FlCstr numerical_value_2;
    FlCstr numerical_value_3;
    FlCstr decomposition_type;
    FlCstr decomposition_mapping;
    FlCstr general_category;
    FlCstr simple_uppercase_mapping;
    FlCstr simple_lowercase_mapping;
    FlCstr simple_titlecase_mapping;
    FlCstr canonical_combining_class;
    FlCstr bidi_class;
    FlCstr bidi_mirrored;
    FlCstr full_composition_exclusion;
} UnicodeData;

size_t get_decomp_type(const char *udata, char *dest)
{
    static char* types[] = {"<font>", "<noBreak>", "<initial>", "<medial>", "<final>", "<isolated>", "<circle>", "<super>", "<sub>", "<vertical>", "<wide>", "<narrow>", "<small>", "<square>", "<fraction>", "<compat>"};
    static char* ctypes[] = {"DECOMP_TYPE_FONT", "DECOMP_TYPE_NOBREAK", "DECOMP_TYPE_INITIAL", "DECOMP_TYPE_MEDIAL", "DECOMP_TYPE_FINAL", "DECOMP_TYPE_ISOLATED", "DECOMP_TYPE_CIRCLE", "DECOMP_TYPE_SUPER", "DECOMP_TYPE_SUB", "DECOMP_TYPE_VERTICAL", "DECOMP_TYPE_WIDE", "DECOMP_TYPE_NARROW", "DECOMP_TYPE_SMALL", "DECOMP_TYPE_SQUARE", "DECOMP_TYPE_FRACTION", "DECOMP_TYPE_COMPAT"};
    size_t length = flm_array_length(types);
    for (size_t i=0; i < length; i++)
    {
        if (!flm_cstr_nequals(udata, types[i], strlen(types[i])))
            continue;
        size_t size = strlen(ctypes[i]);
        memcpy(dest, ctypes[i], size);
        return size;
    }
    return 0;
}

typedef FlVector FlCstrVector;

FlCstrVector split_text(const FlCstr source, size_t length, char chr)
{
    FlCstrVector lines = fl_vector_new(sizeof(FlCstr*), 10);
    size_t s = 0;
    FlCstr tmp = NULL;
    for (size_t i=0; i < length; i++)
    {
        if (source[i] != chr)
            continue;
        size_t nb = i-s;
        if (nb > 0 && source[s] != '#')
        {
            tmp = fl_cstr_dup_n(source+s, nb);
            fl_vector_add(lines, &tmp);
        }
        s = i+1;
        tmp = NULL;
    }
    return lines;
}

void parse_composition_exclusion(FlVector data)
{
    FlByteArray buffer = fl_file_read_all_bytes("src/text/resources/DerivedNormalizationProps-9.0.0.txt");
    size_t size = fl_array_length(buffer);
    
    FlCstr fce_start = strstr((const FlCstr)buffer, "# Derived Property: Full_Composition_Exclusion");
    FlCstr fce_end = strstr(fce_start, "# ================================================");
    
    FlCstrVector lines = split_text((const FlCstr)fce_start, (fce_end-fce_start), '\n');

    fl_vector_delete_ptrs(lines);
    fl_array_delete(buffer);
}

void parse_unicode_data(FlVector data)
{
    FlByteArray buffer = fl_file_read_all_bytes("src/text/resources/UnicodeData-9.0.0.txt");
    size_t size = fl_array_length(buffer);
    const char *base = (const char*)buffer;
    do
    {
        UnicodeData *ud = fl_malloc(sizeof(UnicodeData));
        const char* endField1 = strchr(base, ';');
        const char* endField2 = strchr(endField1+1, ';');
        const char* endField3 = strchr(endField2+1, ';');
        const char* endField4 = strchr(endField3+1, ';');
        const char* endField5 = strchr(endField4+1, ';');
        const char* endField6 = strchr(endField5+1, ';');
        const char* endField7 = strchr(endField6+1, ';');
        const char* endField8 = strchr(endField7+1, ';');
        const char* endField9 = strchr(endField8+1, ';');
        const char* endField10 = strchr(endField9+1, ';');
        const char* endField11 = strchr(endField10+1, ';');
        const char* endField12 = strchr(endField11+1, ';');
        const char* endField13 = strchr(endField12+1, ';');
        const char* endField14 = strchr(endField13+1, ';');
        const char* endField15 = strchr(endField14+1, '\n');

        int lengthField8 = (endField9-1-endField8);
        int lengthField6 = (endField7-1-endField6);
        int lengthField7 = (endField8-1-endField7);
        int lengthField5 = (endField6-1-endField5);
        int lengthField0 = (endField1-(const char*)base);
        int lengthField1 = (endField2-1-endField1);
        int lengthField2 = (endField3-1-endField2);
        int lengthField4 = (endField5-1-endField4);
        int lengthField12 = (endField13-1-endField12);
        int lengthField13 = (endField14-1-endField13);
        int lengthField14 = (endField15-1-endField14);
        int lengthField9 = (endField10-1-endField9);
        int lengthField3 = (endField4-1-endField3);
        
        ud->code = fl_cstr_dup_n((const FlCstr)base, lengthField0);
        ud->name = fl_cstr_dup_n((const FlCstr)endField1+1, lengthField1);
        ud->numerical_value_3 = fl_cstr_dup_n((const FlCstr)(lengthField8 == 0 ? "-1" : endField8+1), lengthField8 == 0 ? 2 : lengthField8);
        ud->numerical_value_1 = fl_cstr_dup_n((const FlCstr)(lengthField6 == 0 ? "-1" : endField6+1), lengthField6 == 0 ? 2 : lengthField6);
        ud->numerical_value_2 = fl_cstr_dup_n((const FlCstr)(lengthField7 == 0 ? "-1" : endField7+1), lengthField7 == 0 ? 2 : lengthField7);


        // Decomposition_Mapping
        FlCstr value = (char*)endField5+1;
        if (lengthField5 != 0)
        {
            char *start = value;
            if (endField5[1] == '<')
            {
                char buffer[50];
                int length = (int)get_decomp_type(value, buffer);
                if (length > 0)
                {
                    ud->decomposition_type = fl_cstr_dup_n(buffer, length);
                }
                size_t toremove = length - 12 + 2 +1; // DECOMP_TYPE_xxx - strlen(DECOMP_TYPE_) + '<' and '>' + ' '
                lengthField5 = lengthField5 - toremove;
                start = value + toremove;
            }
            int bufsize = (int)2+lengthField5+1;
            char buf[bufsize];
            memset(buf, 0, bufsize);
            buf[0] = '0';
            buf[1] = 'x';
            fl_cstr_copy_n(buf+2, start, lengthField5);

            char *str;
            int strsize = (int)fl_cstr_replace_n(buf, bufsize, " ", 1, ", 0x", 4, &str);
            ud->decomposition_mapping = fl_cstr_dup_n(str, strsize);
            fl_cstr_delete(str);
        }

        // General_Category
        if (lengthField2 != 0)
        {
            ud->general_category = fl_cstr_dup_n((const FlCstr)endField2+1, lengthField2);
        }
        else
        {
            ud->general_category = fl_cstr_dup("\"Cn\"");
        }

        ud->bidi_class = fl_cstr_dup_n((const FlCstr)endField4+1, lengthField4);

        if (lengthField12 != 0)
            ud->simple_uppercase_mapping = fl_cstr_dup_n((const FlCstr)endField12+1, lengthField12);
        if (lengthField13 != 0)
            ud->simple_lowercase_mapping = fl_cstr_dup_n((const FlCstr)endField13+1, lengthField13);
        if (lengthField14 != 0)
            ud->simple_titlecase_mapping = fl_cstr_dup_n((const FlCstr)endField14+1, lengthField14);
        
        if (lengthField3 > 1 || (lengthField3 == 1 && (endField3+1)[0] != '0'))
        {
            size_t s = lengthField3+1;
            char str[s];
            memset(str, 0, s);
            memcpy(str, endField3+1, lengthField3);
            str[s] = FL_EOS;
            ud->canonical_combining_class = fl_cstr_dup_n(str, s);
        }
        
        if (lengthField9 != 0 && (endField9+1)[0] == 'Y')
        {
            ud->bidi_mirrored = fl_cstr_dup("true");
        }

        fl_vector_add(data, &ud);
        base = endField15 + 1;
    } while (base < (const char*)buffer+size);
    fl_array_delete(buffer);
}

void delete_data_handler(FlByte *ptr)
{
    UnicodeData *ud = (UnicodeData*)ptr;
    if (ud->code) fl_cstr_delete(ud->code);
    if (ud->name) fl_cstr_delete(ud->name);
    if (ud->numerical_value_1) fl_cstr_delete(ud->numerical_value_1);
    if (ud->numerical_value_2) fl_cstr_delete(ud->numerical_value_2);
    if (ud->numerical_value_3) fl_cstr_delete(ud->numerical_value_3);
    if (ud->decomposition_type) fl_cstr_delete(ud->decomposition_type);
    if (ud->decomposition_mapping) fl_cstr_delete(ud->decomposition_mapping);
    if (ud->general_category) fl_cstr_delete(ud->general_category);
    if (ud->simple_uppercase_mapping) fl_cstr_delete(ud->simple_uppercase_mapping);
    if (ud->simple_lowercase_mapping) fl_cstr_delete(ud->simple_lowercase_mapping);
    if (ud->simple_titlecase_mapping) fl_cstr_delete(ud->simple_titlecase_mapping);
    if (ud->canonical_combining_class) fl_cstr_delete(ud->canonical_combining_class);
    if (ud->bidi_class) fl_cstr_delete(ud->bidi_class);
    if (ud->bidi_mirrored) fl_cstr_delete(ud->bidi_mirrored);
    if (ud->full_composition_exclusion) fl_cstr_delete(ud->full_composition_exclusion);
    fl_free(ud);
}

void create_unicode_database_file(FlVector data)
{
    FlFile *outfd = fl_file_open("src/text/resources/UnicodeDataDb.h", "w");
    size_t data_length = fl_vector_length(data);
    for (size_t i=0; i < data_length; i++)
    {
        UnicodeData *ud = *(UnicodeData**)fl_vector_get(data, i);
        fprintf(outfd, "{");
        fprintf(outfd, ".code = 0x%s, ",                        ud->code);
        fprintf(outfd, ".name = \"%s\", ",                      ud->name);
        fprintf(outfd, ".numerical_value_3 = %s, ",            ud->numerical_value_3);
        fprintf(outfd, ".numerical_value_1 = %s, ",            ud->numerical_value_1);
        fprintf(outfd, ".numerical_value_2 = %s, ",            ud->numerical_value_2);

        if (ud->decomposition_type)
            fprintf(outfd, ".decomposition_type = %s, ",           ud->decomposition_type);
        if (ud->decomposition_mapping)
            fprintf(outfd, ".decomposition_mapping = (const uint32_t[]){ %s, UINT32_MAX }, ", ud->decomposition_mapping);

        fprintf(outfd, ".general_category = \"%s\", ",         ud->general_category);
        fprintf(outfd, ".bidi_class = \"%s\", ",               ud->bidi_class);

        if (ud->simple_uppercase_mapping)
            fprintf(outfd, ".simple_uppercase_mapping = 0x%s, ",    ud->simple_uppercase_mapping);
        
        if (ud->simple_lowercase_mapping)
            fprintf(outfd, ".simple_lowercase_mapping = 0x%s, ",    ud->simple_lowercase_mapping);
        
        if (ud->simple_titlecase_mapping)
            fprintf(outfd, ".simple_titlecase_mapping = 0x%s, ",    ud->simple_titlecase_mapping);
        
        if (ud->canonical_combining_class)
            fprintf(outfd, ".canonical_combining_class = %s, ",     ud->canonical_combining_class);
        
        if (ud->bidi_mirrored)
            fprintf(outfd, ".bidi_mirrored = %s ",                  ud->bidi_mirrored);
        
        fprintf(outfd, "}");

        if (i < data_length-1)
        {
            fprintf(outfd, ",");
            fprintf(outfd, "\n");
        }
    }
    fl_file_close(outfd);
}

int main(void)
{
    FlVector data = fl_vector_new(sizeof(UnicodeData*), 30000);
    parse_unicode_data(data);
    parse_composition_exclusion(data);
    create_unicode_database_file(data);
    fl_vector_delete_h(data, &delete_data_handler);
}
