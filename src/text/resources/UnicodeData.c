#include <string.h>
#include <stdint.h>

#include "../../Types.h"
#include "../../Array.h"
#include "UnicodeData.h"

/* -------------------------------------------------------------
* Private DataTypes
* -------------------------------------------------------------
*/
typedef enum
{
    DECOMP_TYPE_CANONICAL, // default
    DECOMP_TYPE_FONT,
    DECOMP_TYPE_NOBREAK,
    DECOMP_TYPE_INITIAL,
    DECOMP_TYPE_MEDIAL,
    DECOMP_TYPE_FINAL,
    DECOMP_TYPE_ISOLATED,
    DECOMP_TYPE_CIRCLE,
    DECOMP_TYPE_SUPER,
    DECOMP_TYPE_SUB,
    DECOMP_TYPE_VERTICAL,
    DECOMP_TYPE_WIDE,
    DECOMP_TYPE_NARROW,
    DECOMP_TYPE_SMALL,
    DECOMP_TYPE_SQUARE,
    DECOMP_TYPE_FRACTION,
    DECOMP_TYPE_COMPAT
} FlDecompositionType;

typedef enum
{
    NORM_QC_YES,
    NORM_QC_NO,
    NORM_QC_MAYBE
} FlNormQuickCheckResult;

// http://unicode.org/reports/tr44/#Property_List_Table
// unicode_1_name obsolete                  // 10
// iso_comment deprecated                   // 11
typedef struct
{
    double numerical_value_3;               // 8- nt, nv | -1 default value (NULL)
    int numerical_value_1;                  // 6- nt, nv | -1 default value (NULL)
    int numerical_value_2;                  // 7- nt, nv | -1 default value (NULL)
    FlDecompositionType decomposition_type; // 5
    FlNormQuickCheckResult nfd_quick_check; // (DerivedNormalizationProps: NFD_Quick_Check)
    FlNormQuickCheckResult nfc_quick_check; // (DerivedNormalizationProps: NFC_Quick_Check)
    FlNormQuickCheckResult nfkd_quick_check;// (DerivedNormalizationProps: NFKD_Quick_Check)
    FlNormQuickCheckResult nfkc_quick_check;// (DerivedNormalizationProps: NFKC_Quick_Check)
    const uint32_t* decomposition_mapping;  // 5
    const uint32_t code;                    // 0
    const char *name;                      // 1
    const char *general_category;          // 2- gc
    const char *bidi_class;                // 4- bc
    uint32_t simple_uppercase_mapping;      // 12- suc | 0 is default value (NULL)
    uint32_t simple_lowercase_mapping;      // 13- slc | 0 is default value (NULL)
    uint32_t simple_titlecase_mapping;      // 14- stc | 0 is default value (NULL)
    bool bidi_mirrored;                     // 9
    bool full_composition_exclusion;        // (DerivedNormalizationProps: Full_Composition_Exclusion)
    FlByte canonical_combining_class;       // 3- ccc | 0 is default value
} FlUnicodeData;

static FlUnicodeData UnicodeData[] = {
    #ifdef FL_UNICODE_DB
        #include "UnicodeDataDb.h"
    #else
        0
    #endif
};

// TODO: Use Interpolation
size_t lookup_data(uint32_t codepoint)
{
    uint32_t max = flm_array_length(UnicodeData);
    uint32_t min = 0;
    do
    {
        if (min > max)
            break;
        
        size_t index = (max+min) / 2;
        
        FlUnicodeData tmp = UnicodeData[index];
        if (tmp.code < codepoint)
        {
            min = index+1;
        }
        else if (tmp.code > codepoint)
        {
            max = index-1;
        }
        else
        {
            return index;
        }
    } while (true);
    return SIZE_MAX;
}

size_t fl_unicode_get_decomposition_mapping(uint32_t codepoint, bool canonical, uint32_t *destination)
{
    size_t i = lookup_data(codepoint);
    if (i == SIZE_MAX || UnicodeData[i].decomposition_mapping == NULL 
        || (canonical && UnicodeData[i].decomposition_type != DECOMP_TYPE_CANONICAL)
        || (!canonical && UnicodeData[i].decomposition_type == DECOMP_TYPE_CANONICAL))
    {
        // Default value for Decomposition_Mapping is the same value
        *destination = codepoint;
        return 1;
    }
    
    const uint32_t *mapping = UnicodeData[i].decomposition_mapping;
    
    const uint32_t *p = mapping;
    size_t mappingsize = 0;
    while (*p++ != UINT32_MAX)
    { 
        mappingsize++;
    }

    memcpy(destination, mapping, sizeof(uint32_t) * mappingsize);
    return mappingsize;
}
