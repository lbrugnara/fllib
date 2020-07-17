#include <stdbool.h>
#include <flut/flut.h>
#include "Test_Regex.h"

#include "../../src/text/Regex.c"

void tokenize(char *regex, char *expect[])
{
	RegexFlags flags;
	FlVector *tokens = parse_regex(regex, &flags);

	if (tokens == NULL)
		return;

    size_t l = fl_vector_length(tokens);
    bool pass = true;
	for(size_t i=0; i < l; i++)
	{
		char* c = *(char**) fl_vector_ref_get(tokens, i);
		if (expect != NULL)
        {
            int expectl = strlen(expect[i]);
            if (fl_equals(expect[i], c, expectl))
                continue;
            pass = false;
            break;
        }
	}

    char *tokensstr = fl_cstring_join(tokens, "");
    if (expect != NULL)
    {
        char *expectstr = fl_cstring_dup("");
        for(size_t i=0; expect[i] != 0x0; i++)
            fl_cstring_append(&expectstr, expect[i]);
        flut_vexpect_compat(pass, "Expected tokens '%s', received '%s'", expectstr, tokensstr);
        
        fl_cstring_free(expectstr);
    }
    else
    {
        flut_vexpect_compat(pass, "No expectations still received '%s'", tokensstr);
    }
    fl_cstring_free(tokensstr);
	fl_vector_free(tokens);
}

void test_fl_regex_tokenize()
{
    #define expect(...) (char*[]){__VA_ARGS__,0x0}
    tokenize("abc|&def", expect("a", "&", "b", "&", "c", "|", "\\&", "&", "d", "&", "e", "&", "f"));
    tokenize("[abc]", expect("[", "a", "b", "c", "]"));
    tokenize("123[abc]", expect("1", "&", "2", "&", "3", "&", "[", "a", "b", "c", "]"));
    tokenize("[abc]456", expect("[", "a", "b", "c", "]", "&", "4", "&", "5", "&", "6"));
    tokenize("[^abc]", expect("[", "^", "a", "b", "c", "]"));
    tokenize("[ab^cd]", expect("[", "a", "b", "\\^", "c", "d", "]"));
	tokenize("[^^]", expect("[", "^", "\\^", "]"));
	tokenize("[a-z]", expect("[", "a", "-", "z", "]"));
    tokenize("[1-9]", expect("[", "1", "-", "9", "]"));
    
    tokenize("[3-1]", NULL);
    struct FlError err = fl_error_last();
    flut_expect_compat("Regex [3-1] must not compile because it is an invalid range", fl_equals(err.message, "Invalid range", 13));

    tokenize("[x-j]", NULL);
	err = fl_error_last();
    flut_expect_compat("Regex [x-j] must not compile because it is an invalid range", fl_equals(err.message, "Invalid range", 13));

    tokenize("[j-x]", expect("[", "j", "-", "x", "]"));
    tokenize("[9-ao-zqwe]", expect("[", "9", "-", "a", "o", "-", "z", "q", "w", "e", "]"));
    tokenize("(ab|cd)", expect("(", "a", "&", "b", "|", "c", "&", "d", ")"));
    tokenize("([0-9]|[a-z]+)", expect("(", "[", "0", "-", "9", "]", "|", "[", "a", "-", "z", "]", "+", ")"));
    #undef expect
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

void test_fl_regex_match()
{
    flut_expect_compat("Pattern /ab(c|d)/ to match 'abc'", fl_regex_match_test("ab(c|d)", "abc"));
	flut_expect_compat("Pattern /ab(c|d)/ to match 'abd'", fl_regex_match_test("ab(c|d)", "abd"));
	flut_expect_compat("Pattern /ab(c|d)/ to not match 'abe'", !fl_regex_match_test("ab(c|d)", "abe"));
	flut_expect_compat("Pattern /^ab(c|d)/ to not match 'zabc'", !fl_regex_match_test("^ab(c|d)", "zabc"));
	flut_expect_compat("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	flut_expect_compat("Pattern /(a*)*/ to match 'a'", fl_regex_match_test("(a*)*", "a"));
	flut_expect_compat("Pattern /[a]/ to match 'a'", fl_regex_match_test("[a]", "a"));
	flut_expect_compat("Pattern /a(bc)(de)/ to match 'abcde'", fl_regex_match_test("a(bc)(de)", "abcde"));
	flut_expect_compat("Pattern /a((bc)(de))/ to match 'abcde'", fl_regex_match_test("a((bc)(de))", "abcde"));
	flut_expect_compat("Pattern /(a|b)c/ to match 'ac'", fl_regex_match_test("(a|b)c", "ac"));
	flut_expect_compat("Pattern /(a|b|c|d)e/ to match 'ae'", fl_regex_match_test("(a|b|c|d)e", "ae"));
	flut_expect_compat("Pattern /a+/ to match 'a'", fl_regex_match_test("a+", "a"));
	flut_expect_compat("Pattern /(ab)+/ to match 'ab'", fl_regex_match_test("(ab)+", "ab"));
	flut_expect_compat("Pattern /(ab)+(c|d)/ to match 'abc'", fl_regex_match_test("(ab)+(c|d)", "abc"));
	flut_expect_compat("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	flut_expect_compat("Pattern /(abc|def)*/ to match 'abc'", fl_regex_match_test("(abc|def)*", "abc"));
	flut_expect_compat("Pattern /(a*)*/ to match 'a'", fl_regex_match_test("(a*)*", "a"));
	flut_expect_compat("Pattern /a/ to match 'a'", fl_regex_match_test("a", "a"));
	flut_expect_compat("Pattern /abc/ to not match 'b'", !fl_regex_match_test("abc", "b"));
	flut_expect_compat("Pattern /^abc$/ to match 'abc'", fl_regex_match_test("^abc$", "abc"));
	flut_expect_compat("Pattern /^ab/ to not match 'cab'", !fl_regex_match_test("^ab", "cab"));
	flut_expect_compat("Pattern /^ab/ to not match 'ac'", !fl_regex_match_test("^ab", "ac"));
	flut_expect_compat("Pattern /^ab/ to match 'ab'", fl_regex_match_test("^ab", "ab"));
	flut_expect_compat("Pattern /1[abc]9/ to match '1b9'", fl_regex_match_test("1[abc]9", "1b9"));
	flut_expect_compat("Pattern /ab$/ to match 'ab'", fl_regex_match_test("ab$", "ab"));
	flut_expect_compat("Pattern /ab$/ to not match 'abc'", !fl_regex_match_test("ab$", "abc"));
	flut_expect_compat("Pattern /(a(b)c((d)))((e)f)/ to match 'abcdef'", fl_regex_match_test("(a(b)c((d)))((e)f)", "abcdef"));
	flut_expect_compat("Pattern /((a)*)/ to match 'aaa'", fl_regex_match_test("((a)*)", "aaa"));
	flut_expect_compat("Pattern /(abc)+(def)/ to match 'abcabcabcdef'", fl_regex_match_test("(abc)+(def)", "abcabcabcdef"));
	flut_expect_compat("Pattern /(abc)+(def)/ to not match 'abcabcabdef'", !fl_regex_match_test("(abc)+(def)", "abcabcabdef"));
	flut_expect_compat("Pattern /c/ to match 'abcd'", fl_regex_match_test("c", "abcd"));
	flut_expect_compat("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	flut_expect_compat("Pattern /a+/ to not match ''", !fl_regex_match_test("a+", ""));
	flut_expect_compat("Pattern /a*a/ to match 'aaaa'", fl_regex_match_test("a*a", "aaaa"));
	flut_expect_compat("Pattern /a/ to match 'aaa'", fl_regex_match_test("a", "aaa"));
	flut_expect_compat("Pattern /a/ to not match 'b'", !fl_regex_match_test("a", "b"));
	flut_expect_compat("Pattern /a/ to match 'ba'", fl_regex_match_test("a", "ba"));
	flut_expect_compat("Pattern /ab/ to match 'ab'", fl_regex_match_test("ab", "ab"));
	flut_expect_compat("Pattern /a|b/ to match 'a'", fl_regex_match_test("a|b", "a"));
	flut_expect_compat("Pattern /a|b/ to match 'b'", fl_regex_match_test("a|b", "b"));
	flut_expect_compat("Pattern /a|b/ to not match 'c'", !fl_regex_match_test("a|b", "c"));
	flut_expect_compat("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	flut_expect_compat("Pattern /a*/ to match 'bbbaacc'", fl_regex_match_test("a*", "bbbaacc"));
	flut_expect_compat("Pattern /a+c*d+/ to match 'bbbaaccd'", fl_regex_match_test("a+c*d+", "bbbaaccd"));
	flut_expect_compat("Pattern /a*b+/ to match 'bbbaacc'", fl_regex_match_test("a*b+", "bbbaacc"));
	flut_expect_compat("Pattern /a*b+/ to not match 'aacc'", !fl_regex_match_test("a*b+", "aacc"));
	flut_expect_compat("Pattern /b+/ to not match ''", !fl_regex_match_test("b+", ""));
	flut_expect_compat("Pattern /[012345]/ to match '123321'", fl_regex_match_test("[012345]", "123321"));
	flut_expect_compat("Pattern /[012345]/ to not match '99'", !fl_regex_match_test("[012345]", "99"));
	flut_expect_compat("Pattern /[^012345]/ to match '789987'", fl_regex_match_test("[^012345]", "789987"));
	flut_expect_compat("Pattern /[^01]/ to not match '0'", !fl_regex_match_test("[^01]", "0"));
	flut_expect_compat("Pattern /[^0]/ to match '9'", fl_regex_match_test("[^0]", "9"));
	flut_expect_compat("Pattern /[0-5]/ to match '2'", fl_regex_match_test("[0-5]", "2"));
	flut_expect_compat("Pattern /[0-5]/ to not match '6'", !fl_regex_match_test("[0-5]", "6"));
	flut_expect_compat("Pattern /[^0-5]/ to not match '2'", !fl_regex_match_test("[^0-5]", "2"));
	flut_expect_compat("Pattern /[^0-5]/ to match '6'", fl_regex_match_test("[^0-5]", "6"));
	flut_expect_compat("Pattern /a?/ to match ''", fl_regex_match_test("a?", ""));
	flut_expect_compat("Pattern /a?/ to match 'a'", fl_regex_match_test("a?", "a"));
	flut_expect_compat("Pattern /a?/ to match 'b'", fl_regex_match_test("a?", "b"));
	flut_expect_compat("Pattern /a?b/ to not match 'c'", !fl_regex_match_test("a?b", "c"));
	flut_expect_compat("Pattern /(a)?/ to match ''", fl_regex_match_test("(a)?", ""));
	flut_expect_compat("Pattern /(a?)/ to match ''", fl_regex_match_test("(a?)", ""));
	flut_expect_compat("Pattern /(a|b?)/ to match 'a'", fl_regex_match_test("(a|b?)", "a"));
	flut_expect_compat("Pattern /(a|b?)/ to match 'b'", fl_regex_match_test("(a|b?)", "b"));
	flut_expect_compat("Pattern /(a|b?)/ to match ''", fl_regex_match_test("(a|b?)", ""));
	flut_expect_compat("Pattern /(x+x+)+y/ to not match 'xx'", !fl_regex_match_test("(x+x+)+y", "xx"));
	flut_expect_compat("Pattern /(x+)+y/ to match 'xxy'", fl_regex_match_test("(x+)+y", "xxy"));
	flut_expect_compat("Pattern /(x+x+)+y/ to match 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy'", fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy"));
	flut_expect_compat("Pattern /(x+x+)+y/ to not match 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'", !fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	flut_expect_compat("Pattern /a/ to match 'a'", fl_regex_match_test("a", "a"));
	flut_expect_compat("Pattern /ab/ to match 'ab'", fl_regex_match_test("ab", "ab"));
	flut_expect_compat("Pattern /ab/ to not match 'ac'", !fl_regex_match_test("ab", "ac"));
	flut_expect_compat("Pattern /abc/ to match 'abcd'", fl_regex_match_test("abc", "abcd"));
	flut_expect_compat("Pattern /abc/ to match '1abcd'", fl_regex_match_test("abc", "1abcd"));
	flut_expect_compat("Pattern /a|b/ to match 'a'", fl_regex_match_test("a|b", "a"));
	flut_expect_compat("Pattern /a|b/ to match 'b'", fl_regex_match_test("a|b", "b"));
	flut_expect_compat("Pattern /a|b/ to not match 'c'", !fl_regex_match_test("a|b", "c"));
	flut_expect_compat("Pattern /a|b/ to match 'cad'", fl_regex_match_test("a|b", "cad"));
	flut_expect_compat("Pattern /[a]/ to match 'a'", fl_regex_match_test("[a]", "a"));
	flut_expect_compat("Pattern /[a]/ to not match 'b'", !fl_regex_match_test("[a]", "b"));
	flut_expect_compat("Pattern /[abcde]/ to match 'c'", fl_regex_match_test("[abcde]", "c"));
	flut_expect_compat("Pattern /[abcde]/ to match 'e'", fl_regex_match_test("[abcde]", "e"));
	flut_expect_compat("Pattern /[abcde]/ to not match '1'", !fl_regex_match_test("[abcde]", "1"));
	flut_expect_compat("Pattern /[^a]/ to not match 'a'", !fl_regex_match_test("[^a]", "a"));
	flut_expect_compat("Pattern /[^a]/ to match 'b'", fl_regex_match_test("[^a]", "b"));
	flut_expect_compat("Pattern /[^abcdef]/ to not match 'c'", !fl_regex_match_test("[^abcdef]", "c"));
	flut_expect_compat("Pattern /[^abcdef]/ to not match 'f'", !fl_regex_match_test("[^abcdef]", "f"));
	flut_expect_compat("Pattern /[^abcdef]/ to not match 'b'", !fl_regex_match_test("[^abcdef]", "b"));
	flut_expect_compat("Pattern /[^abcdef]/ to match 'z'", fl_regex_match_test("[^abcdef]", "z"));
	flut_expect_compat("Pattern /[^abcdef]/ to match 'x'", fl_regex_match_test("[^abcdef]", "x"));
	flut_expect_compat("Pattern /[^abcdef]/ to match '%%'", fl_regex_match_test("[^abcdef]", "%%"));
	flut_expect_compat("Pattern /[1-6]/ to match '2'", fl_regex_match_test("[1-6]", "2"));
	flut_expect_compat("Pattern /[1-6]/ to match '6'", fl_regex_match_test("[1-6]", "6"));
	flut_expect_compat("Pattern /[1-6]/ to not match '8'", !fl_regex_match_test("[1-6]", "8"));
	flut_expect_compat("Pattern /[l-p]/ to match 'n'", fl_regex_match_test("[l-p]", "n"));
	flut_expect_compat("Pattern /[l-p]/ to not match 'z'", !fl_regex_match_test("[l-p]", "z"));
	flut_expect_compat("Pattern /[5-c]/ to match 'a'", fl_regex_match_test("[5-c]", "a"));
	flut_expect_compat("Pattern /[5-c]/ to match '8'", fl_regex_match_test("[5-c]", "8"));
	flut_expect_compat("Pattern /[5-c]/ to not match 'j'", !fl_regex_match_test("[5-c]", "j"));
	flut_expect_compat("Pattern /(a)/ to match 'a'", fl_regex_match_test("(a)", "a"));
	flut_expect_compat("Pattern /(a)(b)/ to match 'ab'", fl_regex_match_test("(a)(b)", "ab"));
	flut_expect_compat("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	flut_expect_compat("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	flut_expect_compat("Pattern /a*a/ to match 'aaaa'", fl_regex_match_test("a*a", "aaaa"));
	flut_expect_compat("Pattern /(a*)(a)/ to match 'aa'", fl_regex_match_test("(a*)(a)", "aa"));
	flut_expect_compat("Pattern /(a*)b/ to not match 'a'", !fl_regex_match_test("(a*)b", "a"));
	flut_expect_compat("Pattern /(a*)b/ to match 'b'", fl_regex_match_test("(a*)b", "b"));
	flut_expect_compat("Pattern /b(a*)/ to not match 'a'", !fl_regex_match_test("b(a*)", "a"));
	flut_expect_compat("Pattern /b(a*)/ to match 'b'", fl_regex_match_test("b(a*)", "b"));
	flut_expect_compat("Pattern /b(a*)/ to match 'ab'", fl_regex_match_test("b(a*)", "ab"));
	flut_expect_compat("Pattern /(a*a*)*y/ to match 'y'", fl_regex_match_test("(a*a*)*y", "y"));
	flut_expect_compat("Pattern /a*/ to match 'b'", fl_regex_match_test("a*", "b"));
	flut_expect_compat("Pattern /ba*/ to not match 'a'", !fl_regex_match_test("ba*", "a"));
	flut_expect_compat("Pattern /ba*/ to match 'b'", fl_regex_match_test("ba*", "b"));
	flut_expect_compat("Pattern /ba*/ to match 'ba'", fl_regex_match_test("ba*", "ba"));
	flut_expect_compat("Pattern /ba*/ to match 'ab'", fl_regex_match_test("ba*", "ab"));
	flut_expect_compat("Pattern /a+/ to match 'a'", fl_regex_match_test("a+", "a"));
	flut_expect_compat("Pattern /a+/ to not match ''", !fl_regex_match_test("a+", ""));
	flut_expect_compat("Pattern /a+/ to not match 'b'", !fl_regex_match_test("a+", "b"));
	flut_expect_compat("Pattern /ba+/ to not match 'a'", !fl_regex_match_test("ba+", "a"));
	flut_expect_compat("Pattern /ba+/ to not match 'b'", !fl_regex_match_test("ba+", "b"));
	flut_expect_compat("Pattern /ba+/ to match 'ba'", fl_regex_match_test("ba+", "ba"));
	flut_expect_compat("Pattern /ba+/ to match 'baaaaa'", fl_regex_match_test("ba+", "baaaaa"));
	flut_expect_compat("Pattern /a?/ to match 'a'", fl_regex_match_test("a?", "a"));
	flut_expect_compat("Pattern /a?/ to match ''", fl_regex_match_test("a?", ""));
	flut_expect_compat("Pattern /a?/ to match 'b'", fl_regex_match_test("a?", "b"));
	flut_expect_compat("Pattern /ba?/ to not match 'a'", !fl_regex_match_test("ba?", "a"));
	flut_expect_compat("Pattern /ba?/ to match 'b'", fl_regex_match_test("ba?", "b"));
	flut_expect_compat("Pattern /ba?/ to match 'ba'", fl_regex_match_test("ba?", "ba"));
	flut_expect_compat("Pattern /ba?/ to match 'ab'", fl_regex_match_test("ba?", "ab"));
	flut_expect_compat("Pattern /(hello|hi) world[!.]/ to match 'hello world!'", fl_regex_match_test("(hello|hi) world[!.]", "hello world!"));
	flut_expect_compat("Pattern /(hello|hi) world[!.]/ to match 'hello world.'", fl_regex_match_test("(hello|hi) world[!.]", "hello world."));
	flut_expect_compat("Pattern /(hello|hi) world[!.]/ to match 'hi world!'", fl_regex_match_test("(hello|hi) world[!.]", "hi world!"));
	flut_expect_compat("Pattern /(hello|hi) world[!.]/ to match 'hi world.'", fl_regex_match_test("(hello|hi) world[!.]", "hi world."));
	flut_expect_compat("Pattern /(a+)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a+)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /([a-zA-Z]+)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("([a-zA-Z]+)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /([a-zA-Z]+)*/ to match 'abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD'", fl_regex_match_test("([a-zA-Z]+)*", "abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD"));
	flut_expect_compat("Pattern /(a|a?)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flut_expect_compat("Pattern /(a|a?)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa'", fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa"));
	flut_expect_compat("Pattern /(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]/ to match 'hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.'", fl_regex_match_test("(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]", "hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world."));
	flut_expect_compat("Pattern /he0|heo/ to match 'heo'", fl_regex_match_test("he0|heo", "heo"));
	flut_expect_compat("Pattern /he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo/ to match 'heo'", fl_regex_match_test("he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo", "heo"));
	flut_expect_compat("Pattern /(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)/ to match 'heoz'", fl_regex_match_test("(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)", "heoz"));
	flut_expect_compat("Pattern /(ab|ac)*(ab)/ to match 'acabacab'", fl_regex_match_test("(ab|ac)*(ab)", "acabacab"));
	flut_expect_compat("Pattern /(ab)*(ab)/ to match 'abab'", fl_regex_match_test("(ab)*(ab)", "abab"));
	flut_expect_compat("Pattern /(ab|ac)*(ab)/ to match 'acabacac'", fl_regex_match_test("(ab|ac)*(ab)", "acabacac"));
	flut_expect_compat("Pattern /((a)*)/ to match 'aaa'", fl_regex_match_test("((a)*)", "aaa"));
	flut_expect_compat("Pattern /(x*)(x*)(x)/ to match 'xxx'", fl_regex_match_test("(x*)(x*)(x)", "xxx"));
	flut_expect_compat("Pattern /a(b|c)(d|e)*/ to match 'abc'", fl_regex_match_test("a(b|c)(d|e)*", "abc"));
	flut_expect_compat("Pattern /(a|b)(a|b)*(a|b)+/ to match 'abab'", fl_regex_match_test("(a|b)(a|b)*(a|b)+", "abab"));
	flut_expect_compat("Pattern /a(b|d)*c+/ to match 'abdddbddbbcc'", fl_regex_match_test("a(b|d)*c+", "abdddbddbbcc"));
	flut_expect_compat("Pattern /\\*/ to match '*'", fl_regex_match_test("\\*", "*"));
	flut_expect_compat("Pattern /*/ to not match '*'", !fl_regex_match_test("*", "*"));
	flut_expect_compat("Pattern /\\*\\{\\}\\[\\]/ to match '*{}[]'", fl_regex_match_test("\\*\\{\\}\\[\\]", "*{}[]"));
	flut_expect_compat("Pattern /./ to match '.'", fl_regex_match_test(".", "."));
	flut_expect_compat("Pattern /./ to match 'a'", fl_regex_match_test(".", "a"));
	flut_expect_compat("Pattern /./ to match '1'", fl_regex_match_test(".", "1"));
	flut_expect_compat("Pattern /./ to match '{'", fl_regex_match_test(".", "{"));
	flut_expect_compat("Pattern /./ to match '\\\\'", fl_regex_match_test(".", "\\\\"));
	flut_expect_compat("Pattern /./ to match '\r'", fl_regex_match_test(".", "\r"));
	flut_expect_compat("Pattern /./ to not match '\n'", !fl_regex_match_test(".", "\n"));
	flut_expect_compat("Pattern /.+/ to match 'abcdefg'", fl_regex_match_test(".+", "abcdefg"));
	flut_expect_compat("Pattern /*/ to not match '*'", !fl_regex_match_test("*", "*"));
	flut_expect_compat("Pattern /\\h/ to not match 'h'", !fl_regex_match_test("\\h", "h"));
	flut_expect_compat("Pattern /[()]/ to match '()'", fl_regex_match_test("[()]", "()"));
	flut_expect_compat("Pattern /[\\(\\)]/ to match '()'", fl_regex_match_test("[\\(\\)]", "()"));
	flut_expect_compat("Pattern /nes/(opcode|rom)\\.c/ to match 'nes/rom.c'", fl_regex_match_test("nes/(opcode|rom)\\.c", "nes/rom.c"));
	flut_expect_compat("Pattern /nes/(opcode|rom)\\.c/ to not match 'nes/nesrom.c'", !fl_regex_match_test("nes/(opcode|rom)\\.c", "nes/nesrom.c"));
	flut_expect_compat("Pattern /[^/]/ to not match '/'", !fl_regex_match_test("[^/]", "/"));
	flut_expect_compat("Pattern /[.]/ to not match 'a'", !fl_regex_match_test("[.]", "a"));
	flut_expect_compat("Pattern /[.]/ to match '.'", fl_regex_match_test("[.]", "."));
	flut_expect_compat("Pattern /[^.]/ to match 'a'", fl_regex_match_test("[^.]", "a"));
}
