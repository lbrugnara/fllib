#include <stdbool.h>
#include <flut/flut.h>

#include "../../src/text/Regex.c"

#define flut_assert_token_result_is(expected, regex)    \
do {                                            		\
    flut_assertion_explainv(                       		\
        assert_tokenize((expected), (regex)),   		\
        "Regex `%s` should tokenize as `%s`",      		\
        #regex, #expected                             	\
    );                                          		\
} while (0)

static FlutAssertResult* assert_tokenize(char *expect[], char *regex);

flut_define_suite(regex) {
	flut_suite_description("Regex functions");
	flut_suite_register_test(regex_tokenize, "Regex tokenization");
	flut_suite_register_test(regex_match, "fl_regex_match function");
}

flut_define_test(regex_tokenize) {
    #define STR_ARRAY(...) (char*[]){__VA_ARGS__,0x0}
    flut_assert_token_result_is(STR_ARRAY("a", "&", "b", "&", "c", "|", "\\&", "&", "d", "&", "e", "&", "f"), "abc|&def");
    flut_assert_token_result_is(STR_ARRAY("[", "a", "b", "c", "]"), "[abc]");
    flut_assert_token_result_is(STR_ARRAY("1", "&", "2", "&", "3", "&", "[", "a", "b", "c", "]"), "123[abc]");
    flut_assert_token_result_is(STR_ARRAY("[", "a", "b", "c", "]", "&", "4", "&", "5", "&", "6"), "[abc]456");
    flut_assert_token_result_is(STR_ARRAY("[", "^", "a", "b", "c", "]"), "[^abc]");
    flut_assert_token_result_is(STR_ARRAY("[", "a", "b", "\\^", "c", "d", "]"), "[ab^cd]");
	flut_assert_token_result_is(STR_ARRAY("[", "^", "\\^", "]"), "[^^]");
	flut_assert_token_result_is(STR_ARRAY("[", "a", "-", "z", "]"), "[a-z]");
    flut_assert_token_result_is(STR_ARRAY("[", "1", "-", "9", "]"), "[1-9]");
    
    flut_assert_token_result_is(NULL, "[3-1]");
    struct FlError err = fl_error_last();
	flut_assert_string_is_equals("Invalid range 3-1", err.message, false);

    flut_assert_token_result_is(NULL, "[x-j]");
	err = fl_error_last();
	flut_assert_string_is_equals("Invalid range x-j", err.message, false);

    flut_assert_token_result_is(STR_ARRAY("[", "j", "-", "x", "]"), "[j-x]");
    flut_assert_token_result_is(STR_ARRAY("[", "9", "-", "a", "o", "-", "z", "q", "w", "e", "]"), "[9-ao-zqwe]");
    flut_assert_token_result_is(STR_ARRAY("(", "a", "&", "b", "|", "c", "&", "d", ")"), "(ab|cd)");
    flut_assert_token_result_is(STR_ARRAY("(", "[", "0", "-", "9", "]", "|", "[", "a", "-", "z", "]", "+", ")"), "([0-9]|[a-z]+)");
    #undef STR_ARRAY
}

bool fl_regex_match_test(char *pattern, char *input)
{
	FlRegex *regex = fl_regex_compile(pattern);

	if (regex == NULL)
		return false;

	bool res = fl_regex_match(regex, input);
	fl_regex_free(regex);
	return res;
}

flut_define_test(regex_match) {
    flut_assert_is_true(fl_regex_match_test("ab(c|d)", "abc"));
	flut_assert_is_true(fl_regex_match_test("ab(c|d)", "abd"));
	flut_assert_is_false(fl_regex_match_test("ab(c|d)", "abe"));
	flut_assert_is_false(fl_regex_match_test("^ab(c|d)", "zabc"));
	flut_assert_is_true(fl_regex_match_test("a*", "a"));
	flut_assert_is_true(fl_regex_match_test("(a*)*", "a"));
	flut_assert_is_true(fl_regex_match_test("[a]", "a"));
	flut_assert_is_true(fl_regex_match_test("a(bc)(de)", "abcde"));
	flut_assert_is_true(fl_regex_match_test("a((bc)(de))", "abcde"));
	flut_assert_is_true(fl_regex_match_test("(a|b)c", "ac"));
	flut_assert_is_true(fl_regex_match_test("(a|b|c|d)e", "ae"));
	flut_assert_is_true(fl_regex_match_test("a+", "a"));
	flut_assert_is_true(fl_regex_match_test("(ab)+", "ab"));
	flut_assert_is_true(fl_regex_match_test("(ab)+(c|d)", "abc"));
	flut_assert_is_true(fl_regex_match_test("a*", "a"));
	flut_assert_is_true(fl_regex_match_test("(abc|def)*", "abc"));
	flut_assert_is_true(fl_regex_match_test("(a*)*", "a"));
	flut_assert_is_true(fl_regex_match_test("a", "a"));
	flut_assert_is_false(fl_regex_match_test("abc", "b"));
	flut_assert_is_true(fl_regex_match_test("^abc$", "abc"));
	flut_assert_is_false(fl_regex_match_test("^ab", "cab"));
	flut_assert_is_false(fl_regex_match_test("^ab", "ac"));
	flut_assert_is_true(fl_regex_match_test("^ab", "ab"));
	flut_assert_is_true(fl_regex_match_test("1[abc]9", "1b9"));
	flut_assert_is_true(fl_regex_match_test("ab$", "ab"));
	flut_assert_is_false(fl_regex_match_test("ab$", "abc"));
	flut_assert_is_true(fl_regex_match_test("(a(b)c((d)))((e)f)", "abcdef"));
	flut_assert_is_true(fl_regex_match_test("((a)*)", "aaa"));
	flut_assert_is_true(fl_regex_match_test("(abc)+(def)", "abcabcabcdef"));
	flut_assert_is_false(fl_regex_match_test("(abc)+(def)", "abcabcabdef"));
	flut_assert_is_true(fl_regex_match_test("c", "abcd"));
	flut_assert_is_true(fl_regex_match_test("a*", ""));
	flut_assert_is_false(fl_regex_match_test("a+", ""));
	flut_assert_is_true(fl_regex_match_test("a*a", "aaaa"));
	flut_assert_is_true(fl_regex_match_test("a", "aaa"));
	flut_assert_is_false(fl_regex_match_test("a", "b"));
	flut_assert_is_true(fl_regex_match_test("a", "ba"));
	flut_assert_is_true(fl_regex_match_test("ab", "ab"));
	flut_assert_is_true(fl_regex_match_test("a|b", "a"));
	flut_assert_is_true(fl_regex_match_test("a|b", "b"));
	flut_assert_is_false(fl_regex_match_test("a|b", "c"));
	flut_assert_is_true(fl_regex_match_test("a*", ""));
	flut_assert_is_true(fl_regex_match_test("a*", "bbbaacc"));
	flut_assert_is_true(fl_regex_match_test("a+c*d+", "bbbaaccd"));
	flut_assert_is_true(fl_regex_match_test("a*b+", "bbbaacc"));
	flut_assert_is_false(fl_regex_match_test("a*b+", "aacc"));
	flut_assert_is_false(fl_regex_match_test("b+", ""));
	flut_assert_is_true(fl_regex_match_test("[012345]", "123321"));
	flut_assert_is_false(fl_regex_match_test("[012345]", "99"));
	flut_assert_is_true(fl_regex_match_test("[^012345]", "789987"));
	flut_assert_is_false(fl_regex_match_test("[^01]", "0"));
	flut_assert_is_true(fl_regex_match_test("[^0]", "9"));
	flut_assert_is_true(fl_regex_match_test("[0-5]", "2"));
	flut_assert_is_false(fl_regex_match_test("[0-5]", "6"));
	flut_assert_is_false(fl_regex_match_test("[^0-5]", "2"));
	flut_assert_is_true(fl_regex_match_test("[^0-5]", "6"));
	flut_assert_is_true(fl_regex_match_test("a?", ""));
	flut_assert_is_true(fl_regex_match_test("a?", "a"));
	flut_assert_is_true(fl_regex_match_test("a?", "b"));
	flut_assert_is_false(fl_regex_match_test("a?b", "c"));
	flut_assert_is_true(fl_regex_match_test("(a)?", ""));
	flut_assert_is_true(fl_regex_match_test("(a?)", ""));
	flut_assert_is_true(fl_regex_match_test("(a|b?)", "a"));
	flut_assert_is_true(fl_regex_match_test("(a|b?)", "b"));
	flut_assert_is_true(fl_regex_match_test("(a|b?)", ""));
	flut_assert_is_false(fl_regex_match_test("(x+x+)+y", "xx"));
	flut_assert_is_true(fl_regex_match_test("(x+)+y", "xxy"));
	flut_assert_is_true(fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy"));
	flut_assert_is_false(fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	flut_assert_is_true(fl_regex_match_test("a", "a"));
	flut_assert_is_true(fl_regex_match_test("ab", "ab"));
	flut_assert_is_false(fl_regex_match_test("ab", "ac"));
	flut_assert_is_true(fl_regex_match_test("abc", "abcd"));
	flut_assert_is_true(fl_regex_match_test("abc", "1abcd"));
	flut_assert_is_true(fl_regex_match_test("a|b", "a"));
	flut_assert_is_true(fl_regex_match_test("a|b", "b"));
	flut_assert_is_false(fl_regex_match_test("a|b", "c"));
	flut_assert_is_true(fl_regex_match_test("a|b", "cad"));
	flut_assert_is_true(fl_regex_match_test("[a]", "a"));
	flut_assert_is_false(fl_regex_match_test("[a]", "b"));
	flut_assert_is_true(fl_regex_match_test("[abcde]", "c"));
	flut_assert_is_true(fl_regex_match_test("[abcde]", "e"));
	flut_assert_is_false(fl_regex_match_test("[abcde]", "1"));
	flut_assert_is_false(fl_regex_match_test("[^a]", "a"));
	flut_assert_is_true(fl_regex_match_test("[^a]", "b"));
	flut_assert_is_false(fl_regex_match_test("[^abcdef]", "c"));
	flut_assert_is_false(fl_regex_match_test("[^abcdef]", "f"));
	flut_assert_is_false(fl_regex_match_test("[^abcdef]", "b"));
	flut_assert_is_true(fl_regex_match_test("[^abcdef]", "z"));
	flut_assert_is_true(fl_regex_match_test("[^abcdef]", "x"));
	flut_assert_is_true(fl_regex_match_test("[^abcdef]", "%%"));
	flut_assert_is_true(fl_regex_match_test("[1-6]", "2"));
	flut_assert_is_true(fl_regex_match_test("[1-6]", "6"));
	flut_assert_is_false(fl_regex_match_test("[1-6]", "8"));
	flut_assert_is_true(fl_regex_match_test("[l-p]", "n"));
	flut_assert_is_false(fl_regex_match_test("[l-p]", "z"));
	flut_assert_is_true(fl_regex_match_test("[5-c]", "a"));
	flut_assert_is_true(fl_regex_match_test("[5-c]", "8"));
	flut_assert_is_false(fl_regex_match_test("[5-c]", "j"));
	flut_assert_is_true(fl_regex_match_test("(a)", "a"));
	flut_assert_is_true(fl_regex_match_test("(a)(b)", "ab"));
	flut_assert_is_true(fl_regex_match_test("a*", "a"));
	flut_assert_is_true(fl_regex_match_test("a*", ""));
	flut_assert_is_true(fl_regex_match_test("a*a", "aaaa"));
	flut_assert_is_true(fl_regex_match_test("(a*)(a)", "aa"));
	flut_assert_is_false(fl_regex_match_test("(a*)b", "a"));
	flut_assert_is_true(fl_regex_match_test("(a*)b", "b"));
	flut_assert_is_false(fl_regex_match_test("b(a*)", "a"));
	flut_assert_is_true(fl_regex_match_test("b(a*)", "b"));
	flut_assert_is_true(fl_regex_match_test("b(a*)", "ab"));
	flut_assert_is_true(fl_regex_match_test("(a*a*)*y", "y"));
	flut_assert_is_true(fl_regex_match_test("a*", "b"));
	flut_assert_is_false(fl_regex_match_test("ba*", "a"));
	flut_assert_is_true(fl_regex_match_test("ba*", "b"));
	flut_assert_is_true(fl_regex_match_test("ba*", "ba"));
	flut_assert_is_true(fl_regex_match_test("ba*", "ab"));
	flut_assert_is_true(fl_regex_match_test("a+", "a"));
	flut_assert_is_false(fl_regex_match_test("a+", ""));
	flut_assert_is_false(fl_regex_match_test("a+", "b"));
	flut_assert_is_false(fl_regex_match_test("ba+", "a"));
	flut_assert_is_false(fl_regex_match_test("ba+", "b"));
	flut_assert_is_true(fl_regex_match_test("ba+", "ba"));
	flut_assert_is_true(fl_regex_match_test("ba+", "baaaaa"));
	flut_assert_is_true(fl_regex_match_test("a?", "a"));
	flut_assert_is_true(fl_regex_match_test("a?", ""));
	flut_assert_is_true(fl_regex_match_test("a?", "b"));
	flut_assert_is_false(fl_regex_match_test("ba?", "a"));
	flut_assert_is_true(fl_regex_match_test("ba?", "b"));
	flut_assert_is_true(fl_regex_match_test("ba?", "ba"));
	flut_assert_is_true(fl_regex_match_test("ba?", "ab"));
	flut_assert_is_true(fl_regex_match_test("(hello|hi) world[!.]", "hello world!"));
	flut_assert_is_true(fl_regex_match_test("(hello|hi) world[!.]", "hello world."));
	flut_assert_is_true(fl_regex_match_test("(hello|hi) world[!.]", "hi world!"));
	flut_assert_is_true(fl_regex_match_test("(hello|hi) world[!.]", "hi world."));
	flut_assert_is_true(fl_regex_match_test("(a+)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("([a-zA-Z]+)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("([a-zA-Z]+)*", "abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD"));
	flut_assert_is_true(fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_assert_is_true(fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa"));
	flut_assert_is_true(fl_regex_match_test("(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]", "hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world."));
	flut_assert_is_true(fl_regex_match_test("he0|heo", "heo"));
	flut_assert_is_true(fl_regex_match_test("he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo", "heo"));
	flut_assert_is_true(fl_regex_match_test("(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)", "heoz"));
	flut_assert_is_true(fl_regex_match_test("(ab|ac)*(ab)", "acabacab"));
	flut_assert_is_true(fl_regex_match_test("(ab)*(ab)", "abab"));
	flut_assert_is_true(fl_regex_match_test("(ab|ac)*(ab)", "acabacac"));
	flut_assert_is_true(fl_regex_match_test("((a)*)", "aaa"));
	flut_assert_is_true(fl_regex_match_test("(x*)(x*)(x)", "xxx"));
	flut_assert_is_true(fl_regex_match_test("a(b|c)(d|e)*", "abc"));
	flut_assert_is_true(fl_regex_match_test("(a|b)(a|b)*(a|b)+", "abab"));
	flut_assert_is_true(fl_regex_match_test("a(b|d)*c+", "abdddbddbbcc"));
	flut_assert_is_true(fl_regex_match_test("\\*", "*"));
	flut_assert_is_false(fl_regex_match_test("*", "*"));
	flut_assert_is_true(fl_regex_match_test("\\*\\{\\}\\[\\]", "*{}[]"));
	flut_assert_is_true(fl_regex_match_test(".", "."));
	flut_assert_is_true(fl_regex_match_test(".", "a"));
	flut_assert_is_true(fl_regex_match_test(".", "1"));
	flut_assert_is_true(fl_regex_match_test(".", "{"));
	flut_assert_is_true(fl_regex_match_test(".", "\\\\"));
	flut_assert_is_true(fl_regex_match_test(".", "\r"));
	flut_assert_is_false(fl_regex_match_test(".", "\n"));
	flut_assert_is_true(fl_regex_match_test(".+", "abcdefg"));
	flut_assert_is_false(fl_regex_match_test("*", "*"));
	flut_assert_is_false(fl_regex_match_test("\\h", "h"));
	flut_assert_is_true(fl_regex_match_test("[()]", "()"));
	flut_assert_is_true(fl_regex_match_test("[\\(\\)]", "()"));
	flut_assert_is_true(fl_regex_match_test("nes/(opcode|rom)\\.c", "nes/rom.c"));
	flut_assert_is_false(fl_regex_match_test("nes/(opcode|rom)\\.c", "nes/nesrom.c"));
	flut_assert_is_false(fl_regex_match_test("[^/]", "/"));
	flut_assert_is_false(fl_regex_match_test("[.]", "a"));
	flut_assert_is_true(fl_regex_match_test("[.]", "."));
	flut_assert_is_true(fl_regex_match_test("[^.]", "a"));
}

static FlutAssertResult* assert_tokenize(char *expect[], char *regex) {
	FlutAssertResult *result = flut_assert_result_new();

	RegexFlags flags;
	FlVector *tokens = parse_regex(regex, &flags);

	if (tokens == NULL) {
		flut_set_assert_result(result, true, "Regex expected to not tokenize");
		return result;
	}

	if (expect == NULL) {
		flut_set_assert_result(result, false, "Parsing of the regex should not have succeed");
		return result;
	}

	size_t expect_length = 0;
	for (size_t i=0; expect[i] != NULL; i++) {
		expect_length++;
	}
	
    size_t tokens_length = fl_vector_length(tokens);
    bool pass = true;
	for(size_t i=0; i < tokens_length; i++)
	{
		char* token = flm_vector_get(tokens, char*, i);
		if (i < expect_length) {
            if (fl_equals(expect[i], token, strlen(expect[i]))) {
                continue;
			}
            pass = false;
            break;
        }
	}

    char *tokensstr = fl_cstring_join(tokens, "");
    if (expect_length > 0)
    {
        char *expectstr = fl_cstring_dup("");
        for(size_t i=0; expect[i] != 0x0; i++) {
            fl_cstring_append(&expectstr, expect[i]);
		}

		flut_set_assert_resultv(result, pass, "Expected tokens '%s', received '%s'", expectstr, tokensstr);
        
        fl_cstring_free(expectstr);
    }
    else
    {
		flut_set_assert_resultv(result, pass, "No expectations, received '%s'", tokensstr);
    }
    fl_cstring_free(tokensstr);
	fl_vector_free(tokens);

	return result;
}
