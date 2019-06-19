#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../fl_test_utils.h"
#include "../../src/text/Regex.h"
#include "../../src/text/Regex.c"
#include "../../src/Cstring.h"

bool fl_regex_match_test(FlCstr pattern, FlCstr input)
{
	struct FlError *error = NULL;
	FlRegex *regex = fl_regex_compile(pattern, &error);
	if (error != NULL)
	{
		printf("%s in test ", fl_error_get_message(error));
		fl_error_delete(error);
		return false;
	}
	bool res = fl_regex_match(regex, input);
	fl_regex_delete(regex);
	return res;
}

void
fl_regex_test_tokenize(const FlCstr regex, FlCstr expect[])
{
	printf("Tokenize: /%s/ => ", regex);
	RegexFlags flags;
	struct FlError *error = NULL;
	FlVector* tokens = parse_regex(regex, &flags, &error);
	if (error != NULL)
	{
		printf("%s\n", fl_error_get_message(error));
		fl_error_delete(error);
		return;
	}

	FlCstr result = fl_cstring_join(tokens, ", ");
	printf("%s\n", result);
	for(int i=0; i < fl_vector_length(tokens); i++)
	{
		FlCstr c = flm_vector_get(tokens, FlCstr, i);
		if (expect != NULL)
			flm_test(flm_cstring_equals(expect[i], c));
	}
	printf("\n");
	fl_cstring_delete(result);
	fl_vector_delete_ptrs(tokens);
}

void
fl_regex_tests() 
{
	printf("\n==============\n");
	printf("Test: fl_regex\n");

	if (1)
	{
		#define expect(...) (FlCstr[]){__VA_ARGS__}
		fl_regex_test_tokenize("abc|&def", expect("a", "&", "b", "&", "c", "|", "\\&", "&", "d", "&", "e", "&", "f"));
		fl_regex_test_tokenize("[abc]", expect("[", "a", "b", "c", "]"));
		fl_regex_test_tokenize("123[abc]", expect("1", "&", "2", "&", "3", "&", "[", "a", "b", "c", "]"));
		fl_regex_test_tokenize("[abc]456", NULL);
		fl_regex_test_tokenize("[^abc]", NULL);
		fl_regex_test_tokenize("[ab^cd]", NULL);
		fl_regex_test_tokenize("[^^]", NULL);
		fl_regex_test_tokenize("[a-z]", NULL);
		fl_regex_test_tokenize("[1-9]", NULL);
		fl_regex_test_tokenize("[3-1]", NULL);
		fl_regex_test_tokenize("[x-j]", NULL);
		fl_regex_test_tokenize("[j-x]", NULL);
		fl_regex_test_tokenize("[9-ao-zqwe]", NULL);
		fl_regex_test_tokenize("(ab|cd)", NULL);
		fl_regex_test_tokenize("([0-9]|[a-z]+)", NULL);
		#undef expect
	}

	if (1)
	{
		RegexFlags flags;
		FlVector* postfixv = parse_regex_postfix("abc&def[1-6](123|456)", &flags, NULL);
		for(int i=0; i < fl_vector_length(postfixv); i++)
			printf("%s ", flm_vector_get(postfixv, FlCstr, i));
		printf("\n");
		fl_vector_delete_ptrs(postfixv);
	}

	FlRegex *nfa = fl_regex_compile("^ab*(c|d)z+[0-9\\]]x*(asd)?[^a-z]$", NULL);
	fl_regex_delete(nfa);

	flm_test(fl_regex_match_test("ab(c|d)", "abc"));
	flm_test(fl_regex_match_test("ab(c|d)", "abd"));
	flm_test(!fl_regex_match_test("ab(c|d)", "abe"));
	flm_test(!fl_regex_match_test("^ab(c|d)", "zabc"));
	flm_test(fl_regex_match_test("a*", "a"));
	flm_test(fl_regex_match_test("(a*)*", "a"));
	flm_test(fl_regex_match_test("[a]", "a"));
	flm_test(fl_regex_match_test("a(bc)(de)", "abcde"));
	flm_test(fl_regex_match_test("a((bc)(de))", "abcde"));
	flm_test(fl_regex_match_test("(a|b)c", "ac"));
	flm_test(fl_regex_match_test("(a|b|c|d)e", "ae"));
	flm_test(fl_regex_match_test("a+", "a"));
	flm_test(fl_regex_match_test("(ab)+", "ab"));
	flm_test(fl_regex_match_test("(ab)+(c|d)", "abc"));
	flm_test(fl_regex_match_test("a*", "a"));
	flm_test(fl_regex_match_test("(abc|def)*", "abc"));
	flm_test(fl_regex_match_test("(a*)*", "a"));
	flm_test(fl_regex_match_test("a", "a"));
	flm_test(!fl_regex_match_test("abc", "b"));
	flm_test(fl_regex_match_test("^abc$", "abc"));
	flm_test(!fl_regex_match_test("^ab", "cab"));
	flm_test(!fl_regex_match_test("^ab", "ac"));
	flm_test(fl_regex_match_test("^ab", "ab"));
	flm_test(fl_regex_match_test("1[abc]9", "1b9"));
	flm_test(fl_regex_match_test("ab$", "ab"));
	flm_test(!fl_regex_match_test("ab$", "abc"));
	flm_test(fl_regex_match_test("(a(b)c((d)))((e)f)", "abcdef"));
	flm_test(fl_regex_match_test("((a)*)", "aaa"));
	flm_test(fl_regex_match_test("(abc)+(def)", "abcabcabcdef"));
	flm_test(!fl_regex_match_test("(abc)+(def)", "abcabcabdef"));
	flm_test(fl_regex_match_test("c", "abcd"));
	flm_test(fl_regex_match_test("a*", ""));
	flm_test(!fl_regex_match_test("a+", ""));
	flm_test(fl_regex_match_test("a*a", "aaaa"));
	flm_test(fl_regex_match_test("a", "aaa"));
	flm_test(!fl_regex_match_test("a", "b"));
	flm_test(fl_regex_match_test("a", "ba"));
	flm_test(fl_regex_match_test("ab", "ab"));
	flm_test(fl_regex_match_test("a|b", "a"));
	flm_test(fl_regex_match_test("a|b", "b"));
	flm_test(!fl_regex_match_test("a|b", "c"));
	flm_test(fl_regex_match_test("a*", ""));
	flm_test(fl_regex_match_test("a*", "bbbaacc"));
	flm_test(fl_regex_match_test("a+c*d+", "bbbaaccd"));
	flm_test(fl_regex_match_test("a*b+", "bbbaacc"));
	flm_test(!fl_regex_match_test("a*b+", "aacc"));
	flm_test(!fl_regex_match_test("b+", ""));
	flm_test(fl_regex_match_test("[012345]", "123321"));
	flm_test(!fl_regex_match_test("[012345]", "99"));
	flm_test(fl_regex_match_test("[^012345]", "789987"));
	flm_test(!fl_regex_match_test("[^01]", "0"));
	flm_test(fl_regex_match_test("[^0]", "9"));
	flm_test(fl_regex_match_test("[0-5]", "2"));
	flm_test(!fl_regex_match_test("[0-5]", "6"));
	flm_test(!fl_regex_match_test("[^0-5]", "2"));
	flm_test(fl_regex_match_test("[^0-5]", "6"));
	flm_test(fl_regex_match_test("a?", ""));
	flm_test(fl_regex_match_test("a?", "a"));
	flm_test(fl_regex_match_test("a?", "b"));
	flm_test(!fl_regex_match_test("a?b", "c"));
	flm_test(fl_regex_match_test("(a)?", ""));
	flm_test(fl_regex_match_test("(a?)", ""));
	flm_test(fl_regex_match_test("(a|b?)", "a"));
	flm_test(fl_regex_match_test("(a|b?)", "b"));
	flm_test(fl_regex_match_test("(a|b?)", ""));
	flm_test(!fl_regex_match_test("(x+x+)+y", "xx"));
	flm_test(fl_regex_match_test("(x+)+y", "xxy"));
	flm_test(fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxy"));
	flm_test(!fl_regex_match_test("(x+x+)+y", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"));
	flm_test(fl_regex_match_test("a", "a"));
	flm_test(fl_regex_match_test("ab", "ab"));
	flm_test(!fl_regex_match_test("ab", "ac"));
	flm_test(fl_regex_match_test("abc", "abcd"));
	flm_test(fl_regex_match_test("abc", "1abcd"));
	flm_test(fl_regex_match_test("a|b", "a"));
	flm_test(fl_regex_match_test("a|b", "b"));
	flm_test(!fl_regex_match_test("a|b", "c"));
	flm_test(fl_regex_match_test("a|b", "cad"));
	flm_test(fl_regex_match_test("[a]", "a"));
	flm_test(!fl_regex_match_test("[a]", "b"));
	flm_test(fl_regex_match_test("[abcde]", "c"));
	flm_test(fl_regex_match_test("[abcde]", "e"));
	flm_test(!fl_regex_match_test("[abcde]", "1"));
	flm_test(!fl_regex_match_test("[^a]", "a"));
	flm_test(fl_regex_match_test("[^a]", "b"));
	flm_test(!fl_regex_match_test("[^abcdef]", "c"));
	flm_test(!fl_regex_match_test("[^abcdef]", "f"));
	flm_test(!fl_regex_match_test("[^abcdef]", "b"));
	flm_test(fl_regex_match_test("[^abcdef]", "z"));
	flm_test(fl_regex_match_test("[^abcdef]", "x"));
	flm_test(fl_regex_match_test("[^abcdef]", "%%"));
	flm_test(fl_regex_match_test("[1-6]", "2"));
	flm_test(fl_regex_match_test("[1-6]", "6"));
	flm_test(!fl_regex_match_test("[1-6]", "8"));
	flm_test(fl_regex_match_test("[l-p]", "n"));
	flm_test(!fl_regex_match_test("[l-p]", "z"));
	flm_test(fl_regex_match_test("[5-c]", "a"));
	flm_test(fl_regex_match_test("[5-c]", "8"));
	flm_test(!fl_regex_match_test("[5-c]", "j"));
	flm_test(fl_regex_match_test("(a)", "a"));
	flm_test(fl_regex_match_test("(a)(b)", "ab"));
	flm_test(fl_regex_match_test("a*", "a"));
	flm_test(fl_regex_match_test("a*", ""));
	flm_test(fl_regex_match_test("a*a", "aaaa"));
	flm_test(fl_regex_match_test("(a*)(a)", "aa"));
	flm_test(!fl_regex_match_test("(a*)b", "a"));
	flm_test(fl_regex_match_test("(a*)b", "b"));
	flm_test(!fl_regex_match_test("b(a*)", "a"));
	flm_test(fl_regex_match_test("b(a*)", "b"));
	flm_test(fl_regex_match_test("b(a*)", "ab"));
	flm_test(fl_regex_match_test("(a*a*)*y", "y"));
	flm_test(fl_regex_match_test("a*", "b"));
	flm_test(!fl_regex_match_test("ba*", "a"));
	flm_test(fl_regex_match_test("ba*", "b"));
	flm_test(fl_regex_match_test("ba*", "ba"));
	flm_test(fl_regex_match_test("ba*", "ab"));
	flm_test(fl_regex_match_test("a+", "a"));
	flm_test(!fl_regex_match_test("a+", ""));
	flm_test(!fl_regex_match_test("a+", "b"));
	flm_test(!fl_regex_match_test("ba+", "a"));
	flm_test(!fl_regex_match_test("ba+", "b"));
	flm_test(fl_regex_match_test("ba+", "ba"));
	flm_test(fl_regex_match_test("ba+", "baaaaa"));
	flm_test(fl_regex_match_test("a?", "a"));
	flm_test(fl_regex_match_test("a?", ""));
	flm_test(fl_regex_match_test("a?", "b"));
	flm_test(!fl_regex_match_test("ba?", "a"));
	flm_test(fl_regex_match_test("ba?", "b"));
	flm_test(fl_regex_match_test("ba?", "ba"));
	flm_test(fl_regex_match_test("ba?", "ab"));
	flm_test(fl_regex_match_test("(hello|hi) world[!.]", "hello world!"));
	flm_test(fl_regex_match_test("(hello|hi) world[!.]", "hello world."));
	flm_test(fl_regex_match_test("(hello|hi) world[!.]", "hi world!"));
	flm_test(fl_regex_match_test("(hello|hi) world[!.]", "hi world."));
	flm_test(fl_regex_match_test("(a+)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|aa)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|a?)+", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("([a-zA-Z]+)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("([a-zA-Z]+)*", "abopiasdnmLASDLnlmkasdmlkLSDANDOIQmnlASDnlzNfnLLASDLD"));
	flm_test(fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa!"));
	flm_test(fl_regex_match_test("(a|a?)*", "aaaaaaaaaaaaaaaaaaaaaaaa"));
	flm_test(fl_regex_match_test("(hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.](hello|hi) world[!.]", "hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world.hi world."));
	flm_test(fl_regex_match_test("he0|heo", "heo"));
	flm_test(fl_regex_match_test("he0|hea|heb|hec|hed|hee|hef|heg|heh|hei|hej|hek|heo", "heo"));
	flm_test(fl_regex_match_test("(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(e)|(heo)(a)|(heo)(b)|(heo)(c)|(heo)(d)|(heo)(z)", "heoz"));
	flm_test(fl_regex_match_test("(ab|ac)*(ab)", "acabacab"));
	flm_test(fl_regex_match_test("(ab)*(ab)", "abab"));
	flm_test(fl_regex_match_test("(ab|ac)*(ab)", "acabacac"));
	flm_test(fl_regex_match_test("((a)*)", "aaa"));
	flm_test(fl_regex_match_test("(x*)(x*)(x)", "xxx"));
	flm_test(fl_regex_match_test("a(b|c)(d|e)*", "abc"));
	flm_test(fl_regex_match_test("(a|b)(a|b)*(a|b)+", "abab"));
	flm_test(fl_regex_match_test("a(b|d)*c+", "abdddbddbbcc"));
	flm_test(fl_regex_match_test("\\*", "*"));
	flm_test(!fl_regex_match_test("*", "*")); // Regex compilation error
	flm_test(fl_regex_match_test("\\*\\{\\}\\[\\]", "*{}[]"));
	flm_test(fl_regex_match_test(".", "."));
	flm_test(fl_regex_match_test(".", "a"));
	flm_test(fl_regex_match_test(".", "1"));
	flm_test(fl_regex_match_test(".", "{"));
	flm_test(fl_regex_match_test(".", "\\\\"));
	flm_test(fl_regex_match_test(".", "\r"));
	flm_test(!fl_regex_match_test(".", "\n"));
	flm_test(fl_regex_match_test(".+", "abcdefg"));
	flm_test(!fl_regex_match_test("*", "*")); // Regex compilation error
	flm_test(!fl_regex_match_test("\\h", "h")); // Regex compilation error
	flm_test(fl_regex_match_test("[()]", "()"));	// Operators inside charclass without escape should be automatically escaped
	flm_test(fl_regex_match_test("[\\(\\)]", "()")); // Escaped operators inside charclass should work like operators without escape seq

	printf("\nfl_regex -> Ok\n");
	printf("Max used mem: %.2f kbytes\n", fl_memtrack_get()/1024.0);
}