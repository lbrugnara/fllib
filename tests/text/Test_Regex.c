#include "../Test.h"
#include "Test_Regex.h"

#include "../../src/text/Regex.c"

void tokenize(char *regex, char *expect[])
{
	RegexFlags flags;
	FlVector tokens = parse_regex(regex, &flags);

	if (tokens == NULL)
		return;

    size_t l = fl_vector_length(tokens);
    bool pass = true;
	for(size_t i=0; i < l; i++)
	{
		char* c = (char*)fl_vector_get(tokens, i);
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
        fl_vexpect(pass, "Expected tokens '%s', received '%s'", expectstr, tokensstr);
        
        fl_cstring_free(expectstr);
    }
    else
    {
        fl_vexpect(pass, "No expectations still received '%s'", tokensstr);
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
    fl_expect("Regex [3-1] must not compile because it is an invalid range", fl_equals(err.message, "Invalid range", 13));

    tokenize("[x-j]", NULL);
	err = fl_error_last();
    fl_expect("Regex [x-j] must not compile because it is an invalid range", fl_equals(err.message, "Invalid range", 13));

    tokenize("[j-x]", expect("[", "j", "-", "x", "]"));
    tokenize("[9-ao-zqwe]", expect("[", "9", "-", "a", "o", "-", "z", "q", "w", "e", "]"));
    tokenize("(ab|cd)", expect("(", "a", "&", "b", "|", "c", "&", "d", ")"));
    tokenize("([0-9]|[a-z]+)", expect("(", "[", "0", "-", "9", "]", "|", "[", "a", "-", "z", "]", "+", ")"));
    #undef expect
}

bool fl_regex_match_test(char *pattern, char *input)
{
	FlRegex regex = fl_regex_compile(pattern);

	if (regex == NULL)
		return false;

	bool res = fl_regex_match(regex, input);
	fl_regex_free(regex);
	return res;
}

void test_fl_regex_match()
{
    fl_expect("Pattern /ab(c|d)/ to match 'abc'", fl_regex_match_test("ab(c|d)", "abc"));
	fl_expect("Pattern /ab(c|d)/ to match 'abd'", fl_regex_match_test("ab(c|d)", "abd"));
	fl_expect("Pattern /ab(c|d)/ to not match 'abe'", !fl_regex_match_test("ab(c|d)", "abe"));
	fl_expect("Pattern /^ab(c|d)/ to not match 'zabc'", !fl_regex_match_test("^ab(c|d)", "zabc"));
	fl_expect("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	fl_expect("Pattern /(a*)*/ to match 'a'", fl_regex_match_test("(a*)*", "a"));
	fl_expect("Pattern /[a]/ to match 'a'", fl_regex_match_test("[a]", "a"));
	fl_expect("Pattern /a(bc)(de)/ to match 'abcde'", fl_regex_match_test("a(bc)(de)", "abcde"));
	fl_expect("Pattern /a((bc)(de))/ to match 'abcde'", fl_regex_match_test("a((bc)(de))", "abcde"));
	fl_expect("Pattern /(a|b)c/ to match 'ac'", fl_regex_match_test("(a|b)c", "ac"));
	fl_expect("Pattern /(a|b|c|d)e/ to match 'ae'", fl_regex_match_test("(a|b|c|d)e", "ae"));
	fl_expect("Pattern /a+/ to match 'a'", fl_regex_match_test("a+", "a"));
	fl_expect("Pattern /(ab)+/ to match 'ab'", fl_regex_match_test("(ab)+", "ab"));
	fl_expect("Pattern /(ab)+(c|d)/ to match 'abc'", fl_regex_match_test("(ab)+(c|d)", "abc"));
	fl_expect("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	fl_expect("Pattern /(abc|def)*/ to match 'abc'", fl_regex_match_test("(abc|def)*", "abc"));
	fl_expect("Pattern /(a*)*/ to match 'a'", fl_regex_match_test("(a*)*", "a"));
	fl_expect("Pattern /a/ to match 'a'", fl_regex_match_test("a", "a"));
	fl_expect("Pattern /abc/ to not match 'b'", !fl_regex_match_test("abc", "b"));
	fl_expect("Pattern /^abc$/ to match 'abc'", fl_regex_match_test("^abc$", "abc"));
	fl_expect("Pattern /^ab/ to not match 'cab'", !fl_regex_match_test("^ab", "cab"));
	fl_expect("Pattern /^ab/ to not match 'ac'", !fl_regex_match_test("^ab", "ac"));
	fl_expect("Pattern /^ab/ to match 'ab'", fl_regex_match_test("^ab", "ab"));
	fl_expect("Pattern /1[abc]9/ to match '1b9'", fl_regex_match_test("1[abc]9", "1b9"));
	fl_expect("Pattern /ab$/ to match 'ab'", fl_regex_match_test("ab$", "ab"));
	fl_expect("Pattern /ab$/ to not match 'abc'", !fl_regex_match_test("ab$", "abc"));
	fl_expect("Pattern /(a(b)c((d)))((e)f)/ to match 'abcdef'", fl_regex_match_test("(a(b)c((d)))((e)f)", "abcdef"));
	fl_expect("Pattern /((a)*)/ to match 'aaa'", fl_regex_match_test("((a)*)", "aaa"));
	fl_expect("Pattern /(abc)+(def)/ to match 'abcabcabcdef'", fl_regex_match_test("(abc)+(def)", "abcabcabcdef"));
	fl_expect("Pattern /(abc)+(def)/ to not match 'abcabcabdef'", !fl_regex_match_test("(abc)+(def)", "abcabcabdef"));
	fl_expect("Pattern /c/ to match 'abcd'", fl_regex_match_test("c", "abcd"));
	fl_expect("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	fl_expect("Pattern /a+/ to not match ''", !fl_regex_match_test("a+", ""));
	fl_expect("Pattern /a*a/ to match 'aaaa'", fl_regex_match_test("a*a", "aaaa"));
	fl_expect("Pattern /a/ to match 'aaa'", fl_regex_match_test("a", "aaa"));
	fl_expect("Pattern /a/ to not match 'b'", !fl_regex_match_test("a", "b"));
	fl_expect("Pattern /a/ to match 'ba'", fl_regex_match_test("a", "ba"));
	fl_expect("Pattern /ab/ to match 'ab'", fl_regex_match_test("ab", "ab"));
	fl_expect("Pattern /a|b/ to match 'a'", fl_regex_match_test("a|b", "a"));
	fl_expect("Pattern /a|b/ to match 'b'", fl_regex_match_test("a|b", "b"));
	fl_expect("Pattern /a|b/ to not match 'c'", !fl_regex_match_test("a|b", "c"));
	fl_expect("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	fl_expect("Pattern /a*/ to match 'bbbaacc'", fl_regex_match_test("a*", "bbbaacc"));
	fl_expect("Pattern /a+c*d+/ to match 'bbbaaccd'", fl_regex_match_test("a+c*d+", "bbbaaccd"));
	fl_expect("Pattern /a*b+/ to match 'bbbaacc'", fl_regex_match_test("a*b+", "bbbaacc"));
	fl_expect("Pattern /a*b+/ to not match 'aacc'", !fl_regex_match_test("a*b+", "aacc"));
	fl_expect("Pattern /b+/ to not match ''", !fl_regex_match_test("b+", ""));
	fl_expect("Pattern /[012345]/ to match '123321'", fl_regex_match_test("[012345]", "123321"));
	fl_expect("Pattern /[012345]/ to not match '99'", !fl_regex_match_test("[012345]", "99"));
	fl_expect("Pattern /[^012345]/ to match '789987'", fl_regex_match_test("[^012345]", "789987"));
	fl_expect("Pattern /[^01]/ to not match '0'", !fl_regex_match_test("[^01]", "0"));
	fl_expect("Pattern /[^0]/ to match '9'", fl_regex_match_test("[^0]", "9"));
	fl_expect("Pattern /[0-5]/ to match '2'", fl_regex_match_test("[0-5]", "2"));
	fl_expect("Pattern /[0-5]/ to not match '6'", !fl_regex_match_test("[0-5]", "6"));
	fl_expect("Pattern /[^0-5]/ to not match '2'", !fl_regex_match_test("[^0-5]", "2"));
	fl_expect("Pattern /[^0-5]/ to match '6'", fl_regex_match_test("[^0-5]", "6"));
	fl_expect("Pattern /a?/ to match ''", fl_regex_match_test("a?", ""));
	fl_expect("Pattern /a?/ to match 'a'", fl_regex_match_test("a?", "a"));
	fl_expect("Pattern /a?/ to match 'b'", fl_regex_match_test("a?", "b"));
	fl_expect("Pattern /a?b/ to not match 'c'", !fl_regex_match_test("a?b", "c"));
	fl_expect("Pattern /(a)?/ to match ''", fl_regex_match_test("(a)?", ""));
	fl_expect("Pattern /(a?)/ to match ''", fl_regex_match_test("(a?)", ""));
	fl_expect("Pattern /(a|b?)/ to match 'a'", fl_regex_match_test("(a|b?)", "a"));
	fl_expect("Pattern /(a|b?)/ to match 'b'", fl_regex_match_test("(a|b?)", "b"));
	fl_expect("Pattern /(a|b?)/ to match ''", fl_regex_match_test("(a|b?)", ""));
	fl_expect("Pattern /(x+x+)+y/ to not match 'xx'", !fl_regex_match_test("(x+x+)+y", "xx"));
	fl_expect("Pattern /(x+)+y/ to match 'xxy'", fl_regex_match_test("(x+)+y", "xxy"));
	fl_expect("Pattern /(x+x+)+y/ to match 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy'", fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy"));
	fl_expect("Pattern /(x+x+)+y/ to not match 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'", !fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	fl_expect("Pattern /a/ to match 'a'", fl_regex_match_test("a", "a"));
	fl_expect("Pattern /ab/ to match 'ab'", fl_regex_match_test("ab", "ab"));
	fl_expect("Pattern /ab/ to not match 'ac'", !fl_regex_match_test("ab", "ac"));
	fl_expect("Pattern /abc/ to match 'abcd'", fl_regex_match_test("abc", "abcd"));
	fl_expect("Pattern /abc/ to match '1abcd'", fl_regex_match_test("abc", "1abcd"));
	fl_expect("Pattern /a|b/ to match 'a'", fl_regex_match_test("a|b", "a"));
	fl_expect("Pattern /a|b/ to match 'b'", fl_regex_match_test("a|b", "b"));
	fl_expect("Pattern /a|b/ to not match 'c'", !fl_regex_match_test("a|b", "c"));
	fl_expect("Pattern /a|b/ to match 'cad'", fl_regex_match_test("a|b", "cad"));
	fl_expect("Pattern /[a]/ to match 'a'", fl_regex_match_test("[a]", "a"));
	fl_expect("Pattern /[a]/ to not match 'b'", !fl_regex_match_test("[a]", "b"));
	fl_expect("Pattern /[abcde]/ to match 'c'", fl_regex_match_test("[abcde]", "c"));
	fl_expect("Pattern /[abcde]/ to match 'e'", fl_regex_match_test("[abcde]", "e"));
	fl_expect("Pattern /[abcde]/ to not match '1'", !fl_regex_match_test("[abcde]", "1"));
	fl_expect("Pattern /[^a]/ to not match 'a'", !fl_regex_match_test("[^a]", "a"));
	fl_expect("Pattern /[^a]/ to match 'b'", fl_regex_match_test("[^a]", "b"));
	fl_expect("Pattern /[^abcdef]/ to not match 'c'", !fl_regex_match_test("[^abcdef]", "c"));
	fl_expect("Pattern /[^abcdef]/ to not match 'f'", !fl_regex_match_test("[^abcdef]", "f"));
	fl_expect("Pattern /[^abcdef]/ to not match 'b'", !fl_regex_match_test("[^abcdef]", "b"));
	fl_expect("Pattern /[^abcdef]/ to match 'z'", fl_regex_match_test("[^abcdef]", "z"));
	fl_expect("Pattern /[^abcdef]/ to match 'x'", fl_regex_match_test("[^abcdef]", "x"));
	fl_expect("Pattern /[^abcdef]/ to match '%%'", fl_regex_match_test("[^abcdef]", "%%"));
	fl_expect("Pattern /[1-6]/ to match '2'", fl_regex_match_test("[1-6]", "2"));
	fl_expect("Pattern /[1-6]/ to match '6'", fl_regex_match_test("[1-6]", "6"));
	fl_expect("Pattern /[1-6]/ to not match '8'", !fl_regex_match_test("[1-6]", "8"));
	fl_expect("Pattern /[l-p]/ to match 'n'", fl_regex_match_test("[l-p]", "n"));
	fl_expect("Pattern /[l-p]/ to not match 'z'", !fl_regex_match_test("[l-p]", "z"));
	fl_expect("Pattern /[5-c]/ to match 'a'", fl_regex_match_test("[5-c]", "a"));
	fl_expect("Pattern /[5-c]/ to match '8'", fl_regex_match_test("[5-c]", "8"));
	fl_expect("Pattern /[5-c]/ to not match 'j'", !fl_regex_match_test("[5-c]", "j"));
	fl_expect("Pattern /(a)/ to match 'a'", fl_regex_match_test("(a)", "a"));
	fl_expect("Pattern /(a)(b)/ to match 'ab'", fl_regex_match_test("(a)(b)", "ab"));
	fl_expect("Pattern /a*/ to match 'a'", fl_regex_match_test("a*", "a"));
	fl_expect("Pattern /a*/ to match ''", fl_regex_match_test("a*", ""));
	fl_expect("Pattern /a*a/ to match 'aaaa'", fl_regex_match_test("a*a", "aaaa"));
	fl_expect("Pattern /(a*)(a)/ to match 'aa'", fl_regex_match_test("(a*)(a)", "aa"));
	fl_expect("Pattern /(a*)b/ to not match 'a'", !fl_regex_match_test("(a*)b", "a"));
	fl_expect("Pattern /(a*)b/ to match 'b'", fl_regex_match_test("(a*)b", "b"));
	fl_expect("Pattern /b(a*)/ to not match 'a'", !fl_regex_match_test("b(a*)", "a"));
	fl_expect("Pattern /b(a*)/ to match 'b'", fl_regex_match_test("b(a*)", "b"));
	fl_expect("Pattern /b(a*)/ to match 'ab'", fl_regex_match_test("b(a*)", "ab"));
	fl_expect("Pattern /(a*a*)*y/ to match 'y'", fl_regex_match_test("(a*a*)*y", "y"));
	fl_expect("Pattern /a*/ to match 'b'", fl_regex_match_test("a*", "b"));
	fl_expect("Pattern /ba*/ to not match 'a'", !fl_regex_match_test("ba*", "a"));
	fl_expect("Pattern /ba*/ to match 'b'", fl_regex_match_test("ba*", "b"));
	fl_expect("Pattern /ba*/ to match 'ba'", fl_regex_match_test("ba*", "ba"));
	fl_expect("Pattern /ba*/ to match 'ab'", fl_regex_match_test("ba*", "ab"));
	fl_expect("Pattern /a+/ to match 'a'", fl_regex_match_test("a+", "a"));
	fl_expect("Pattern /a+/ to not match ''", !fl_regex_match_test("a+", ""));
	fl_expect("Pattern /a+/ to not match 'b'", !fl_regex_match_test("a+", "b"));
	fl_expect("Pattern /ba+/ to not match 'a'", !fl_regex_match_test("ba+", "a"));
	fl_expect("Pattern /ba+/ to not match 'b'", !fl_regex_match_test("ba+", "b"));
	fl_expect("Pattern /ba+/ to match 'ba'", fl_regex_match_test("ba+", "ba"));
	fl_expect("Pattern /ba+/ to match 'baaaaa'", fl_regex_match_test("ba+", "baaaaa"));
	fl_expect("Pattern /a?/ to match 'a'", fl_regex_match_test("a?", "a"));
	fl_expect("Pattern /a?/ to match ''", fl_regex_match_test("a?", ""));
	fl_expect("Pattern /a?/ to match 'b'", fl_regex_match_test("a?", "b"));
	fl_expect("Pattern /ba?/ to not match 'a'", !fl_regex_match_test("ba?", "a"));
	fl_expect("Pattern /ba?/ to match 'b'", fl_regex_match_test("ba?", "b"));
	fl_expect("Pattern /ba?/ to match 'ba'", fl_regex_match_test("ba?", "ba"));
	fl_expect("Pattern /ba?/ to match 'ab'", fl_regex_match_test("ba?", "ab"));
	fl_expect("Pattern /(hello|hi) world[!.]/ to match 'hello world!'", fl_regex_match_test("(hello|hi) world[!.]", "hello world!"));
	fl_expect("Pattern /(hello|hi) world[!.]/ to match 'hello world.'", fl_regex_match_test("(hello|hi) world[!.]", "hello world."));
	fl_expect("Pattern /(hello|hi) world[!.]/ to match 'hi world!'", fl_regex_match_test("(hello|hi) world[!.]", "hi world!"));
	fl_expect("Pattern /(hello|hi) world[!.]/ to match 'hi world.'", fl_regex_match_test("(hello|hi) world[!.]", "hi world."));
	fl_expect("Pattern /(a+)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a+)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|aa)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|a?)+/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /([a-zA-Z]+)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("([a-zA-Z]+)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /([a-zA-Z]+)*/ to match 'abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD'", fl_regex_match_test("([a-zA-Z]+)*", "abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD"));
	fl_expect("Pattern /(a|a?)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa!'", fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	fl_expect("Pattern /(a|a?)*/ to match 'aaaaaaaaaaaaaaaaaaaaaaaa'", fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa"));
	fl_expect("Pattern /(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]/ to match 'hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.'", fl_regex_match_test("(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]", "hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world."));
	fl_expect("Pattern /he0|heo/ to match 'heo'", fl_regex_match_test("he0|heo", "heo"));
	fl_expect("Pattern /he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo/ to match 'heo'", fl_regex_match_test("he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo", "heo"));
	fl_expect("Pattern /(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)/ to match 'heoz'", fl_regex_match_test("(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)", "heoz"));
	fl_expect("Pattern /(ab|ac)*(ab)/ to match 'acabacab'", fl_regex_match_test("(ab|ac)*(ab)", "acabacab"));
	fl_expect("Pattern /(ab)*(ab)/ to match 'abab'", fl_regex_match_test("(ab)*(ab)", "abab"));
	fl_expect("Pattern /(ab|ac)*(ab)/ to match 'acabacac'", fl_regex_match_test("(ab|ac)*(ab)", "acabacac"));
	fl_expect("Pattern /((a)*)/ to match 'aaa'", fl_regex_match_test("((a)*)", "aaa"));
	fl_expect("Pattern /(x*)(x*)(x)/ to match 'xxx'", fl_regex_match_test("(x*)(x*)(x)", "xxx"));
	fl_expect("Pattern /a(b|c)(d|e)*/ to match 'abc'", fl_regex_match_test("a(b|c)(d|e)*", "abc"));
	fl_expect("Pattern /(a|b)(a|b)*(a|b)+/ to match 'abab'", fl_regex_match_test("(a|b)(a|b)*(a|b)+", "abab"));
	fl_expect("Pattern /a(b|d)*c+/ to match 'abdddbddbbcc'", fl_regex_match_test("a(b|d)*c+", "abdddbddbbcc"));
	fl_expect("Pattern /\\*/ to match '*'", fl_regex_match_test("\\*", "*"));
	fl_expect("Pattern /*/ to not match '*'", !fl_regex_match_test("*", "*"));
	fl_expect("Pattern /\\*\\{\\}\\[\\]/ to match '*{}[]'", fl_regex_match_test("\\*\\{\\}\\[\\]", "*{}[]"));
	fl_expect("Pattern /./ to match '.'", fl_regex_match_test(".", "."));
	fl_expect("Pattern /./ to match 'a'", fl_regex_match_test(".", "a"));
	fl_expect("Pattern /./ to match '1'", fl_regex_match_test(".", "1"));
	fl_expect("Pattern /./ to match '{'", fl_regex_match_test(".", "{"));
	fl_expect("Pattern /./ to match '\\\\'", fl_regex_match_test(".", "\\\\"));
	fl_expect("Pattern /./ to match '\r'", fl_regex_match_test(".", "\r"));
	fl_expect("Pattern /./ to not match '\n'", !fl_regex_match_test(".", "\n"));
	fl_expect("Pattern /.+/ to match 'abcdefg'", fl_regex_match_test(".+", "abcdefg"));
	fl_expect("Pattern /*/ to not match '*'", !fl_regex_match_test("*", "*"));
	fl_expect("Pattern /\\h/ to not match 'h'", !fl_regex_match_test("\\h", "h"));
	fl_expect("Pattern /[()]/ to match '()'", fl_regex_match_test("[()]", "()"));
	fl_expect("Pattern /[\\(\\)]/ to match '()'", fl_regex_match_test("[\\(\\)]", "()"));
}
