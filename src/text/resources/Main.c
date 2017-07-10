#include <stdio.h>
#include <string.h>
#include <fllib.h>

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

int main(void)
{
    FlFile *outfd = fl_file_open("src/text/resources/UnicodeDataDb.h", "w");
    FlByteArray buffer = fl_file_read_all_bytes("src/text/resources/UnicodeData-9.0.0.txt");
    size_t size = fl_array_length(buffer);
    const char *base = (const char*)buffer;
    do
    {
        const char* endField1 = strchr((const char*)base, ';');
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

        fprintf(outfd, "{");
        
        fprintf(outfd, ".code = 0x%.*s, ", lengthField0, (const char*)base);
        fprintf(outfd, ".name = \"%.*s\", ", lengthField1, endField1+1);
        fprintf(outfd, ".numerical_value_3 = %.*s, ", lengthField8 == 0 ? 2 : lengthField8, lengthField8 == 0 ? "-1" : endField8+1);
        fprintf(outfd, ".numerical_value_1 = %.*s, ", lengthField6 == 0 ? 2 : lengthField6, lengthField6 == 0 ? "-1" : endField6+1);
        fprintf(outfd, ".numerical_value_2 = %.*s, ", lengthField7 == 0 ? 2 : lengthField7, lengthField7 == 0 ? "-1" : endField7+1);


        // Decomposition_Mapping
        FlCstr value = (char*)endField5+1;
        if (lengthField5 != 0)
        {
            char *start = value;
            if (endField5[1] == '<')
            {
                char buffer[50];
                size_t length = get_decomp_type(value, buffer);
                if (length > 0)
                {
                    fprintf(outfd, ".decomposition_type = %.*s, ", length, buffer);
                }
                size_t toremove = length - 12 + 2 +1; // DECOMP_TYPE_xxx - strlen(DECOMP_TYPE_) + '<' and '>' + ' '
                lengthField5 = lengthField5 - toremove;
                start = value + toremove;
            }
            //fprintf(outfd, ".decomposition_mapping = %.*s, ", lengthField5, start);
            int bufsize = (int)2+lengthField5+1;
            char buf[bufsize];
            memset(buf, 0, bufsize);
            buf[0] = '0';
            buf[1] = 'x';
            fl_cstr_copy_n(buf+2, start, lengthField5);

            char *str;
            int strsize = (int)fl_cstr_replace_n(buf, bufsize, " ", 1, ", 0x", 4, &str);
            fprintf(outfd, ".decomposition_mapping = (const uint32_t[]){ %.*s }, ", strsize, str);
            fl_cstr_delete(str);
        }

        // General_Category
        if (lengthField2 != 0)
        {
            fprintf(outfd, ".general_category = \"%.*s\", ", lengthField2, endField2+1);
        }
        else
        {
            fprintf(outfd, ".general_category = \"Cn\", ");
        }

        fprintf(outfd, ".bidi_class = \"%.*s\", ", lengthField4, endField4+1);

        if (lengthField12 != 0)
            fprintf(outfd, ".simple_uppercase_mapping = 0x%.*s, ", lengthField12, endField12+1);
        if (lengthField13 != 0)
            fprintf(outfd, ".simple_lowercase_mapping = 0x%.*s, ", lengthField13, endField13+1);
        if (lengthField14 != 0)
            fprintf(outfd, ".simple_titlecase_mapping = 0x%.*s, ", lengthField14, endField14+1);
        
        if (lengthField3 > 1 || (lengthField3 == 1 && (endField3+1)[0] != '0'))
        {
            size_t s = lengthField3+1;
            char str[s];
            memcpy(str, endField3+1, lengthField3);
            str[s] = FL_EOS;
            int intval = atoi(str);
            fprintf(outfd, ".canonical_combining_class = %d, ", intval);
        }
        
        if (lengthField9 != 0 && (endField9+1)[0] == 'Y')
        {
            fprintf(outfd, ".bidi_mirrored = true ");
        }
        else
        {
            fprintf(outfd, ".bidi_mirrored = false ");
        }

        fprintf(outfd, "}");
        
        base = endField15 + 1;
        if (base < (const char*)buffer+size)
            fprintf(outfd, ",");
        fprintf(outfd, "\n");

    } while (base < (const char*)buffer+size);
    fl_file_close(outfd);
    fl_array_delete(buffer);
}
