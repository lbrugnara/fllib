#include <stdio.h>
#include <string.h>
#include <fllib.h>

typedef struct 
{
    char *code;
    char *name;
    char *numerical_value_1;
    char *numerical_value_2;
    char *numerical_value_3;
    char *decomposition_type;
    char *decomposition_mapping;
    char *general_category;
    char *simple_uppercase_mapping;
    char *simple_lowercase_mapping;
    char *simple_titlecase_mapping;
    char *canonical_combining_class;
    char *bidi_class;
    char *bidi_mirrored;
    char *full_composition_exclusion;
    char *nfd_quick_check;
    char *nfc_quick_check;
    char *nfkd_quick_check;
    char *nfkc_quick_check;
} UnicodeData;

typedef enum
{
    NORM_PROP_FCE,
    NORM_PROP_NFD_QC_NO,
    NORM_PROP_NFC_QC_NO,
    NORM_PROP_NFC_QC_MAYBE,
    NORM_PROP_NFKD_QC_NO,
    NORM_PROP_NFKC_QC_NO,
    NORM_PROP_NFKC_QC_MAYBE
} NormProperty;

// Handy typedefs
typedef FlVector FlStringVector;
typedef char** FlStringArray;

// This function translates from UCD to FlDecompositionType enum copying it 
// into dest returning the number of characters copied
size_t get_decomp_type(const char *udata, char *dest)
{
    static char *types[] = {"<font>", "<noBreak>", "<initial>", "<medial>", "<final>", "<isolated>", "<circle>", "<super>", "<sub>", "<vertical>", "<wide>", "<narrow>", "<small>", "<square>", "<fraction>", "<compat>"};
    static char *ctypes[] = {"DECOMP_TYPE_FONT", "DECOMP_TYPE_NOBREAK", "DECOMP_TYPE_INITIAL", "DECOMP_TYPE_MEDIAL", "DECOMP_TYPE_FINAL", "DECOMP_TYPE_ISOLATED", "DECOMP_TYPE_CIRCLE", "DECOMP_TYPE_SUPER", "DECOMP_TYPE_SUB", "DECOMP_TYPE_VERTICAL", "DECOMP_TYPE_WIDE", "DECOMP_TYPE_NARROW", "DECOMP_TYPE_SMALL", "DECOMP_TYPE_SQUARE", "DECOMP_TYPE_FRACTION", "DECOMP_TYPE_COMPAT"};
    size_t length = flm_array_length(types);
    for (size_t i=0; i < length; i++)
    {
        if (!flm_cstring_equals_n(udata, types[i], strlen(types[i])))
            continue;
        size_t size = strlen(ctypes[i]);
        memcpy(dest, ctypes[i], size);
        return size;
    }
    return 0;
}


int code_comparer(const void *a, const void *b)
{
    char *acode = (char*)a;
    UnicodeData *udb = *(UnicodeData**)b;
    long ha = strtol(acode, NULL, 16);
    long hb = strtol(udb->code, NULL, 16);
    return ha-hb;
}

int unicodedata_comparer(const void *a, const void *b)
{
    UnicodeData *uda = *(UnicodeData**)a;
    UnicodeData *udb = *(UnicodeData**)b;
    long ha = strtol(uda->code, NULL, 16);
    long hb = strtol(udb->code, NULL, 16);
    return ha-hb;
}
// Splits {source} (with length {length}) by character {chr} ignoring
// empty lines and lines starting with '#' (UCD comment).
// It returns a vector of char*, caller must free the memory.
FlStringVector split_text(const char *source, size_t length, char chr)
{
    FlStringVector lines = fl_vector_new(10, fl_container_cleaner_pointer);
    size_t s = 0;
    char *tmp = NULL;
    for (size_t i=0; i < length; i++)
    {
        if (source[i] != chr)
            continue;
        size_t nb = i-s;
        if (nb > 0 && source[s] != '#')
        {
            tmp = fl_cstring_dup_n((char*)source+s, nb);
            fl_vector_add(lines, &tmp);
        }
        s = i+1;
        tmp = NULL;
    }
    return lines;
}

// Receives a line from the DerivedNormalizationProps and returns
// a list of codes from the first column, 1 when the column has just
// one code, or the range when the column has the format <start_code>..<end_code>
FlStringArray get_codes_from_dnp(char *line)
{
    char *p = strstr(line, "..");
    if (p == NULL)
    {
        FlStringArray out = (FlStringArray)fl_array_new(sizeof(char*), 1);
        char *p2 = strchr(line, ' ');
        char *code = fl_cstring_dup_n(line, p2-line);
        out[0] = code;
        return out;
    }
    else
    {
        long start = strtol(line, NULL, 16);
        long end = strtol(p+2, NULL, 16);
        FlStringArray out = fl_array_new(sizeof(char*), end-start+1);
        size_t index = 0;
        for (long i=start; i <= end; i++)
        {
            char tmp[11];
            sprintf(tmp, "%lX", i);
            out[index++] = fl_cstring_dup(tmp);
        }
        return out;
    }
}

void parse_derived_normalization_property(FlVector data, const char *buffer, NormProperty property)
{
    const char *startv = NULL;
    const char *endv = "# ================================================";
    const char *value = NULL;
    switch (property)
    {
        case NORM_PROP_FCE:
            startv = "# Derived Property: Full_Composition_Exclusion";
            value = "true";
            break;
        case NORM_PROP_NFD_QC_NO:
            startv = "# NFD_Quick_Check=No";
            value = "NORM_QC_NO";
            break;
        case NORM_PROP_NFC_QC_NO:
            startv = "# NFC_Quick_Check=No";
            value = "NORM_QC_NO";
            break;
        case NORM_PROP_NFC_QC_MAYBE:
            startv = "# NFC_Quick_Check=Maybe";
            value = "NORM_QC_MAYBE";
            break;
        case NORM_PROP_NFKD_QC_NO:
            startv = "# NFKD_Quick_Check=No";
            value = "NORM_QC_NO";
            break;
        case NORM_PROP_NFKC_QC_NO:
            startv = "# NFKC_Quick_Check=No";
            value = "NORM_QC_NO";
            break;
        case NORM_PROP_NFKC_QC_MAYBE:
            startv = "# NFKC_Quick_Check=Maybe";
            value = "NORM_QC_MAYBE";
            break;
    }

    FlVector newcodepoints = fl_vector_new(1000, NULL);

    char* start = strstr((const char*)buffer, startv);
    char* end = strstr(start, endv);
    FlStringVector lines = split_text((const char*)start, (end-start), '\n');
    size_t nlines = fl_vector_length(lines);
    for (size_t i=0; i < nlines; i++)
    {
        char* line = *(char**)fl_vector_get(lines, i);
        FlStringArray codes = get_codes_from_dnp(line);
        size_t codes_length = fl_array_length(codes);
        for (size_t j=0; j < codes_length; j++)
        {
            UnicodeData *ud = NULL;
            UnicodeData **ptr = (UnicodeData**)bsearch(codes[j], fl_vector_get(data, 0), fl_vector_length(data), sizeof(UnicodeData*), &code_comparer);
            if (!ptr)
            {
                ptr = (UnicodeData**)bsearch(codes[j], fl_vector_get(newcodepoints, 0), fl_vector_length(newcodepoints), sizeof(UnicodeData*), &code_comparer);
            }
            if (ptr) ud = *ptr;

            if (ud == NULL)
            {
                ud = fl_malloc(sizeof(UnicodeData));
                ud->code = fl_cstring_dup(codes[j]);
                fl_vector_add(newcodepoints, &ud);
            }

            char *propval = fl_cstring_dup((const char*)value);
            switch (property)
            {
                case NORM_PROP_FCE:
                    ud->full_composition_exclusion = propval;
                    break;
                case NORM_PROP_NFD_QC_NO:
                    ud->nfd_quick_check = propval;
                    break;
                case NORM_PROP_NFC_QC_NO:
                case NORM_PROP_NFC_QC_MAYBE:
                    ud->nfc_quick_check = propval;
                    break;
                case NORM_PROP_NFKD_QC_NO:
                    ud->nfkd_quick_check = propval;
                    break;
                case NORM_PROP_NFKC_QC_NO:
                case NORM_PROP_NFKC_QC_MAYBE:
                    ud->nfkc_quick_check = propval;
                    break;
            }
            fl_cstring_delete(codes[j]);
        }
        fl_array_delete(codes);
    }
    fl_vector_delete(lines);

    fl_vector_concat(data, newcodepoints);
    fl_vector_delete(newcodepoints);
    qsort(fl_vector_get(data, 0), fl_vector_length(data), sizeof(UnicodeData*), &unicodedata_comparer);
}

// This function parses DerivedNormalizationProps and set some of these properties in {data}
// that contains UnicodeData pointers.
void parse_derived_normalization_properties(FlVector data)
{
    flm_assert(data != NULL && fl_vector_length(data) > 0, "Vector with UnicodeData.txt information must be populated before calling this function");
    FlArray tmpbuffer = fl_io_file_read_all_bytes("src/text/resources/DerivedNormalizationProps-9.0.0.txt");
    char *buffer = NULL;
    size_t size = fl_cstring_replace_n((char*)tmpbuffer, fl_array_length(tmpbuffer), "\r\n", 2, "\n", 1, &buffer);
    fl_array_delete(tmpbuffer);    

    // Full_Composition_Exclusion
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_FCE);

    // NFD_Quick_Check=No
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFD_QC_NO);

    // NFC_Quick_Check=No
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFC_QC_NO);

    // NFC_Quick_Check=Maybe
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFC_QC_MAYBE);

    // NFKD_Quick_Check=No
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFKD_QC_NO);

    // NFKC_Quick_Check=No
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFKC_QC_NO);

    // NFKC_Quick_Check=Maybe
    parse_derived_normalization_property(data, (const char*)buffer, NORM_PROP_NFKC_QC_MAYBE);
    fl_cstring_delete(buffer);
}

// This function parses the UnicodeData.txt file. It is expected to be called before
// any other parsing function to populate {data} with the UCD
void parse_unicode_data(FlVector data)
{
    FlArray tmpbuffer = fl_io_file_read_all_bytes("src/text/resources/UnicodeData-9.0.0.txt");
    //FlByte *buffer = (FlByte*)fl_cstring_replace((char*)tmpbuffer, "\r\n", "\n");
    char *buffer = NULL;
    size_t size = fl_cstring_replace_n((char*)tmpbuffer, fl_array_length(tmpbuffer), "\r\n", 2, "\n", 1, &buffer);
    fl_array_delete(tmpbuffer);
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
        
        ud->code = fl_cstring_dup_n((const char*)base, lengthField0);
        if (fl_equals(ud->code, "10FFFD", min(strlen(ud->code), 6)))
        {
            int i=0;
        }
        ud->name = fl_cstring_dup_n((const char*)endField1+1, lengthField1);

        if (lengthField8 > 0)
            ud->numerical_value_3 = fl_cstring_dup_n((const char*)endField8+1, lengthField8);
        
        if (lengthField6)
            ud->numerical_value_1 = fl_cstring_dup_n((const char*)endField6+1, lengthField6);

        if (lengthField7)
            ud->numerical_value_2 = fl_cstring_dup_n((const char*)endField7+1, lengthField7);


        // Decomposition_Mapping
        char* value = (char*)endField5+1;
        if (lengthField5 != 0)
        {
            char *start = value;
            if (endField5[1] == '<')
            {
                char buffer[50];
                int length = (int)get_decomp_type(value, buffer);
                if (length > 0)
                {
                    ud->decomposition_type = fl_cstring_dup_n(buffer, length);
                }
                size_t toremove = length - 12 + 2 +1; // DECOMP_TYPE_xxx - strlen(DECOMP_TYPE_) + '<' and '>' + ' '
                lengthField5 = lengthField5 - toremove;
                start = value + toremove;
            }
            int bufsize = (int)2+lengthField5+1;
            char *buf = fl_array_new(sizeof(char), bufsize);
            memset(buf, 0, bufsize);
            buf[0] = '0';
            buf[1] = 'x';
            fl_cstring_copy_n(buf+2, start, lengthField5);

            char *str;
            int strsize = (int)fl_cstring_replace_n(buf, bufsize, " ", 1, ", 0x", 4, &str);
            ud->decomposition_mapping = fl_cstring_dup_n(str, strsize);
            fl_cstring_delete(str);
            fl_array_delete(buf);
        }

        // General_Category
        if (lengthField2 != 0)
        {
            ud->general_category = fl_cstring_dup_n((const char*)endField2+1, lengthField2);
        }

        ud->bidi_class = fl_cstring_dup_n((const char*)endField4+1, lengthField4);

        if (lengthField12 != 0)
            ud->simple_uppercase_mapping = fl_cstring_dup_n((const char*)endField12+1, lengthField12);
        if (lengthField13 != 0)
            ud->simple_lowercase_mapping = fl_cstring_dup_n((const char*)endField13+1, lengthField13);
        if (lengthField14 != 0)
            ud->simple_titlecase_mapping = fl_cstring_dup_n((const char*)endField14+1, lengthField14);
        
        if (lengthField3 > 1 || (lengthField3 == 1 && (endField3+1)[0] != '0'))
        {
            size_t s = lengthField3+1;
            char *str = fl_array_new(sizeof(char), s);
            memset(str, 0, s);
            memcpy(str, endField3+1, lengthField3);
            str[s-1] = FL_EOS;
            ud->canonical_combining_class = fl_cstring_dup_n(str, s);
            fl_array_delete(str);
        }
        
        if (lengthField9 != 0 && (endField9+1)[0] == 'Y')
        {
            ud->bidi_mirrored = fl_cstring_dup("true");
        }

        fl_vector_add(data, &ud);
        base = endField15 + 1;
    } while (base < (const char*)buffer+size);
    fl_cstring_delete(buffer);
}

void delete_data_handler(void *ptr)
{
    UnicodeData *ud = (UnicodeData*)ptr;
    if (ud->code) fl_cstring_delete(ud->code);
    if (ud->name) fl_cstring_delete(ud->name);
    if (ud->numerical_value_1) fl_cstring_delete(ud->numerical_value_1);
    if (ud->numerical_value_2) fl_cstring_delete(ud->numerical_value_2);
    if (ud->numerical_value_3) fl_cstring_delete(ud->numerical_value_3);
    if (ud->decomposition_type) fl_cstring_delete(ud->decomposition_type);
    if (ud->decomposition_mapping) fl_cstring_delete(ud->decomposition_mapping);
    if (ud->general_category) fl_cstring_delete(ud->general_category);
    if (ud->simple_uppercase_mapping) fl_cstring_delete(ud->simple_uppercase_mapping);
    if (ud->simple_lowercase_mapping) fl_cstring_delete(ud->simple_lowercase_mapping);
    if (ud->simple_titlecase_mapping) fl_cstring_delete(ud->simple_titlecase_mapping);
    if (ud->canonical_combining_class) fl_cstring_delete(ud->canonical_combining_class);
    if (ud->bidi_class) fl_cstring_delete(ud->bidi_class);
    if (ud->bidi_mirrored) fl_cstring_delete(ud->bidi_mirrored);
    if (ud->full_composition_exclusion) fl_cstring_delete(ud->full_composition_exclusion);
    if (ud->nfd_quick_check) fl_cstring_delete(ud->nfd_quick_check);
    if (ud->nfc_quick_check) fl_cstring_delete(ud->nfc_quick_check);
    if (ud->nfkd_quick_check) fl_cstring_delete(ud->nfkd_quick_check);
    if (ud->nfkc_quick_check) fl_cstring_delete(ud->nfkc_quick_check);
    fl_free(ud);
}

void create_unicode_database_file(FlVector data)
{
    FILE * outfd = fl_io_file_open("src/text/resources/UnicodeDataDb.h", "w");
    size_t data_length = fl_vector_length(data);
    UnicodeData *prevud = NULL;
    for (size_t i=0; i < data_length; i++)
    {
        UnicodeData *ud = *(UnicodeData**)fl_vector_get(data, i);
        if (prevud != NULL)
        {
            size_t lp = strlen(prevud->code);
            size_t lc = strlen(ud->code);
            if (lp == lc && fl_equals(prevud->code, ud->code, lc))
            {
                flm_vexit(ERR_FATAL, "Repeated codepoint %s", ud->code);
            }
        }
        prevud = ud;
        fprintf(outfd, "{");
        fprintf(outfd, ".code = 0x%s",                        ud->code);

        if (ud->name)
            fprintf(outfd, ", .name = \"%s\"",                      ud->name);

        if (ud->numerical_value_3)
            fprintf(outfd, ", .numerical_value_3 = %s",            ud->numerical_value_3);
        else
            fprintf(outfd, ", .numerical_value_3 = -1");
        
        if (ud->numerical_value_1)
            fprintf(outfd, ", .numerical_value_1 = %s",            ud->numerical_value_1);
        else
            fprintf(outfd, ", .numerical_value_1 = -1");
        
        if (ud->numerical_value_2)
            fprintf(outfd, ", .numerical_value_2 = %s",            ud->numerical_value_2);
        else
            fprintf(outfd, ", .numerical_value_2 = -1");

        if (ud->decomposition_type)
            fprintf(outfd, ", .decomposition_type = %s",           ud->decomposition_type);
        if (ud->decomposition_mapping)
            fprintf(outfd, ", .decomposition_mapping = (const uint32_t[]){ %s, UINT32_MAX }", ud->decomposition_mapping);

        if (ud->general_category)
            fprintf(outfd, ", .general_category = \"%s\"",         ud->general_category);
        else
            fprintf(outfd, ", .general_category = \"Cn\"");

        if (ud->bidi_class)
            fprintf(outfd, ", .bidi_class = \"%s\"",               ud->bidi_class);

        if (ud->simple_uppercase_mapping)
            fprintf(outfd, ", .simple_uppercase_mapping = 0x%s",    ud->simple_uppercase_mapping);
        
        if (ud->simple_lowercase_mapping)
            fprintf(outfd, ", .simple_lowercase_mapping = 0x%s",    ud->simple_lowercase_mapping);
        
        if (ud->simple_titlecase_mapping)
            fprintf(outfd, ", .simple_titlecase_mapping = 0x%s",    ud->simple_titlecase_mapping);
        
        if (ud->canonical_combining_class)
            fprintf(outfd, ", .canonical_combining_class = %s",     ud->canonical_combining_class);
        
        if (ud->bidi_mirrored)
            fprintf(outfd, ", .bidi_mirrored = %s",                  ud->bidi_mirrored);

        if (ud->full_composition_exclusion)
            fprintf(outfd, ", .full_composition_exclusion = %s",                  ud->full_composition_exclusion);

        if (ud->nfd_quick_check)
            fprintf(outfd, ", .nfd_quick_check = %s",                  ud->nfd_quick_check);

        if (ud->nfc_quick_check)
            fprintf(outfd, ", .nfc_quick_check = %s",                  ud->nfc_quick_check);

        if (ud->nfkd_quick_check)
            fprintf(outfd, ", .nfkd_quick_check = %s",                  ud->nfkd_quick_check);

        if (ud->nfkc_quick_check)
            fprintf(outfd, ", .nfkc_quick_check = %s",                  ud->nfkc_quick_check);
        
        fprintf(outfd, " }");

        if (i < data_length-1)
        {
            fprintf(outfd, ",");
            fprintf(outfd, "\n");
        }
    }
    fprintf(outfd, "\n");
    fl_io_file_close(outfd);
}

int main(void)
{
    FlVector data = fl_vector_new(30000, delete_data_handler);
    parse_unicode_data(data);
    parse_derived_normalization_properties(data);
    create_unicode_database_file(data);
    fl_vector_delete(data);
}
