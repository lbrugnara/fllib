/* =============================================================
 * {module: Regex}
 * =============================================================
 * This module compiles patterns into regular expressions through
 * the creation of an NFA.
 * The steps involved in the process are:
 *	- Analysis of the pattern
 *	- Parsing. It includes tokenization of the pattern and its transformation into a postifx notation
 *	- Compilation. The process that creates the NFA
 * This module currently support matching functions.
 *-------------------------------------------------------------
 * {todo: Capturing groups}
 * -------------------------------------------------------------
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "Regex.h"
#include "../Cstr.h"
#include "../Array.h"
#include "../Mem.h"

/* -------------------------------------------------------------
 * {datatype: unsigned char RegexFlags}
 * -------------------------------------------------------------
 * Use bitwise operators to track pattern flags.
 * Current flags:
 * 	Bit 0 - Start: 1 if pattern starts with ^
 *	Bit 1 - End: 1 if pattern ends with $
 * -------------------------------------------------------------
 */
typedef unsigned char RegexFlags;
#define FLAG_START 			FLBIT(0)
#define FLAG_END 			FLBIT(1)
#define SET_FLAG_START(s)	FLBIT_ON(s,FLAG_START)
#define SET_FLAG_END(s)		FLBIT_ON(s,FLAG_END)
#define HAS_FLAG_START(s)	FLBIT_IS_ON(s,FLAG_START)
#define HAS_FLAG_END(s)		FLBIT_IS_ON(s,FLAG_END)

/* -------------------------------------------------------------
 * {datatype: enum NfaStateType}
 * -------------------------------------------------------------
 * Type of NFA state. Current types:
 * 	NFA_STATE: Represents a simple state with a given value like S(a)
 * 	NFA_CHARCLASS_STATE: Represents a charclass state like S([0-9])
 * -------------------------------------------------------------
 */
typedef enum
{
	NFA_STATE,
	NFA_CHARCLASS_STATE
} NfaStateType;

/* -------------------------------------------------------------
 * {datatype: unsigned char StateFlags}
 * -------------------------------------------------------------
 * Use bitwise operators to track state flags. 
 * Current flags:
 * 	Bit 0 - Initial: 1 when state is the initial state of the NFA
 *	Bit 1 - Final: 1 when state is a final state of the NFA
 * -------------------------------------------------------------
 */
typedef unsigned char StateFlags;
#define STATE_INITIAL 			FLBIT(0)
#define STATE_FINAL 			FLBIT(1)
#define STATE_SET_INITIAL(s)	FLBIT_ON(s->flags,STATE_INITIAL)
#define STATE_SET_FINAL(s)		FLBIT_ON(s->flags,STATE_FINAL)
#define STATE_UNSET_INITIAL(s)	FLBIT_OFF(s->flags,STATE_INITIAL)
#define STATE_UNSET_FINAL(s)	FLBIT_OFF(s->flags,STATE_FINAL)
#define STATE_IS_INITIAL(s)		FLBIT_IS_ON(s->flags,STATE_INITIAL)
#define STATE_IS_FINAL(s)		FLBIT_IS_ON(s->flags,STATE_FINAL)

/* -------------------------------------------------------------
 * {datatype: struct NfaState}
 * -------------------------------------------------------------
 * Represents an state of an NFA
 * -------------------------------------------------------------
 * {member: int id} State ID (starts from 0). It us used as an array index to run the NFA 
 * {member: FlCstr value} Node value to be matched (or a pretty representation like in NfaStateCharClass)
 * {member: FlVector to*} Vector of states reachables from current state
 * {member: StateFlags flags} Set of flags to indicate initial and final states
 * {member: NfaStateType type} Type of state
 * -------------------------------------------------------------
 */
#define STATE_BASE_DEF() 	\
int id;						\
FlCstr value;				\
FlVector *to;				\
StateFlags flags;			\
NfaStateType type;

typedef struct
{
	STATE_BASE_DEF()
} NfaState;

/* -------------------------------------------------------------
 * {datatype: struct NfaStateCharClass}
 * -------------------------------------------------------------
 * {member: int map} Represents 256 ASCII characters. Valid characters are set to 1. State matches with all the values that are 1 in map (or 0 based on {negated})
 * {member: bool negated} True when the char class state is a negated set
 * -------------------------------------------------------------
 */
typedef struct
{
	STATE_BASE_DEF()
	int map[256];
	bool negated;
} NfaStateCharClass;

/* -------------------------------------------------------------
 * {datatype: struct FlRegex}
 * -------------------------------------------------------------
 * {member: FlCstr pattern} Pattern used to compile the {FlRegex}
 * {member: NfaState states} Array of {NfaState} pointers (represent the NFA)
 * {member: RegexFlags flags} Flags parsed during compilation
 * -------------------------------------------------------------
 */
struct FlRegex
{
	FlCstr pattern;
	NfaState **states;
	RegexFlags flags;
};

/* -------------------------------------------------------------
 * {datatype: struct RegexOperatorInfo}
 * -------------------------------------------------------------
 * Represents the information related to an operator
 * {member: short precedence} Operator precedence
 * {member: char assoc} Operator associativity ('r' for right and 'l' for left)
 * {member: char arity} Operator arity ('u' for unary, 'b' for binary)
 * -------------------------------------------------------------
 */
typedef struct 
{
    short precedence;
    char assoc;
    char arity;
} RegexOperatorInfo;

/* -------------------------------------------------------------
 * {datatype: struct RegexOperator}
 * -------------------------------------------------------------
 * Represents a regex operator and its information
 * {member: char operator} Character representation of the operator
 * {member: RegexOperatorInfo data} Operator info 
 * -------------------------------------------------------------
 */
typedef struct 
{
	char operator;
	RegexOperatorInfo data;    
} RegexOperator;

/* -------------------------------------------------------------
 * {datatype: struct NfaStepResult}
 * -------------------------------------------------------------
 * Return type of the nfa_step function. Contains information about a step of the NFA for a given input
 * -------------------------------------------------------------
 * {member: bool anyMatch} True if we can reach another state in the step
 * {member: bool anyFinal} True if a reached state in the step is a final step
 * -------------------------------------------------------------
 */
typedef struct
{
	bool anyMatch;
	bool anyFinal;
} NfaStepResult;

/* -------------------------------------------------------------
 * {datatype: struct CurrentState}
 * -------------------------------------------------------------
 * Represents a reached state in the NFA for a given input in a certain step
 * -------------------------------------------------------------
 * {member: int id} State ID
 * {member: bool e} True when state is an Epsilon state
 * -------------------------------------------------------------
 */
typedef struct 
{
	int id;
	bool e;
} CurrentState;

/* -------------------------------------------------------------
 * {datatype: struct RegexAnalysis}
 * -------------------------------------------------------------
 * Intended to track information about a pattern. Used by {analyze_regexx}
 * -------------------------------------------------------------
 * {member: size_t patternStart} Position to start at tokenization
 * {member: size_t patternEnd} Position to end at tokenization
 * {member: size_t numTokens} Number of tokens the tokenization process will produce
 * -------------------------------------------------------------
 */
typedef struct
{
	size_t patternStart;
	size_t patternEnd;
	size_t numTokens;
} RegexAnalysis;

/* -------------------------------------------------------------
 * {variable: char[] OperatorsChars}
 * -------------------------------------------------------------
 * Contains all the operators recognized by this regex engine. & is recognized
 * as an operator, by it is used explicitly in the code, so there is no need to
 * keep it here. (Commented out to make it notorious)
 * -------------------------------------------------------------
 */
static char OperatorsChars[] = {
	'(', ')', 		/* Capturing group */
	'[', ']', 		/* Character class */
	'*', '+', '?',  /* Repetition */
	'|'				/* Alternation */
	//'&'			/* Concatenation will be added explictly */
};

/* -------------------------------------------------------------
 * {variable: char[] CompileOperators}
 * -------------------------------------------------------------
 * Operators that need special attention while compiling the pattern
 * -------------------------------------------------------------
 */
static char CompileOperators[] = {'&', '|', '*', '+', '?', '['};

/* -------------------------------------------------------------
 * {variable: char[] CharclassOperators}
 * -------------------------------------------------------------
 * Contains operators used for character class operators
 * -------------------------------------------------------------
 */
static char CharclassOperators[] = {
	'[', ']', '^', '-'
};

/* -------------------------------------------------------------
 * {variable: RegexOperator[] RgxOperators}
 * -------------------------------------------------------------
 * Contains the information of operators, used to parse the pattern and convert
 * it to postfix notation
 * -------------------------------------------------------------
 */
RegexOperator RgxOperators[] = {
	// Op	Precedence 	Assoc 	Type
	{ '*',  { 100, 		'r', 	'u' }},
	{ '+',	{ 100, 		'r', 	'u' }},
	{ '?',	{ 100, 		'r', 	'u' }},
	{ '&',	{ 90,  		'l', 	'b' }},
	{ '|',	{ 80,  		'l', 	'b' }}
};

/* -------------------------------------------------------------
 * {variable: char[] ConcatAtRightOp}
 * -------------------------------------------------------------
 * Operators that allow a concatenation operator at its right side, but not at its left side
 * -------------------------------------------------------------
 */
char ConcatAtRightOp[] = {')', ']', '*', '+', '?'};

/* -------------------------------------------------------------
 * {variable: char[] ConcatAtLeftOp}
 * -------------------------------------------------------------
 * Operators that allow a concatenation operator at its left side, but not at its right side
 * -------------------------------------------------------------
 */
char ConcatAtLeftOp[] = {'(', '['};

/* -------------------------------------------------------------
 * {variable: char[] EscapedChars}
 * -------------------------------------------------------------
 * Allowed escape sequences
 * -------------------------------------------------------------
 */
char EscapedChars[] = {
	'(', ')', '\\',
	// Operators
	'[', ']', '*', '+', '?', '|', '^', '&', '$', '{', '}',
	// Line feed, carriage return, tabs, etc
	'n', 't', 'r',
	// Digits, Words and White spaces
	'd', 'D', 'w', 'W', 's', 'S',
	// Any char
	'.'
};

/* -------------------------------------------------------------
 * Helper functions to find values inside arrays like operators, escaped chars, etc
 * -------------------------------------------------------------
 */
static inline bool is_operator(char c) 
{ 
	return fl_array_contains_n(OperatorsChars, flm_array_length(OperatorsChars), &c, sizeof(char)); 
}

static inline bool is_charclass_operator(char c) { 
	return fl_array_contains_n(CharclassOperators, flm_array_length(CharclassOperators), &c, sizeof(char)); 
}

static inline bool is_escape_seq(char c) 
{ 
	return fl_array_contains_n(EscapedChars, flm_array_length(EscapedChars), &c, sizeof(char)); 
}

#define CONCAT_LEFT 0
#define CONCAT_RIGHT 1
static inline bool allow_concat(short t, char c) 
{
	if (t == CONCAT_LEFT) 
		return fl_array_contains_n(ConcatAtLeftOp, flm_array_length(ConcatAtLeftOp), &c, sizeof(char)); 
	return fl_array_contains_n(ConcatAtRightOp, flm_array_length(ConcatAtRightOp), &c, sizeof(char)); 
}

/* -------------------------------------------------------------
 * Helper functions to retrieve precedence, assocciativity, arity, etc.
 * -------------------------------------------------------------
 */
static int RgxOpLength = flm_array_length(RgxOperators);

static inline bool regex_operator_exists(char op)
{
	for (int i=0; i < RgxOpLength; i++)
	{
		if (RgxOperators[i].operator == op)
			return true;
	}
	return false;
}

static inline int regex_operator_get_precedence(char op)
{
	for (int i=0; i < RgxOpLength; i++)
	{
		if (RgxOperators[i].operator == op)
			return RgxOperators[i].data.precedence;
	}
	return -1;
}

static inline char regex_operator_get_assoc(char op)
{
	for (int i=0; i < RgxOpLength; i++)
	{
		if (RgxOperators[i].operator == op)
			return RgxOperators[i].data.assoc;
	}
	return FL_EOS;
}

static inline char regex_operator_get_arity(char op)
{
	for (int i=0; i < RgxOpLength; i++)
	{
		if (RgxOperators[i].operator == op)
			return RgxOperators[i].data.arity;
	}
	return FL_EOS;
}

/* -------------------------------------------------------------
 * Private API used by Regex module
 * -------------------------------------------------------------
 */
FlVector *parse_regex (FlCstr regex, RegexFlags *flags, FlError **error);

FlVector *parse_regex_postfix (FlCstr regex, RegexFlags *flags, FlError **error);

void print_nfa (NfaState **states);

void print_nfa_state(NfaState *state);

NfaState* create_nfa_state(int id, FlCstr value);

NfaState* create_charclass_nfa_state(int id, FlCstr value, bool negated);

void delete_nfa(FlByte *statebytes);

void nfa_print_operands(const char *operator, NfaState **states, short s, short m, short e);

void nfa_concat(NfaState **states, short ls, short le, short rs, short re);

void nfa_repeat (NfaState **states, short ls, short le);

bool can_reach_state (NfaState *state, unsigned char value);

NfaStepResult nfa_step (NfaState *state, CurrentState nextstates[], unsigned char value, const CurrentState currentstates[]);

bool regex_match (FlRegex *regex, FlCstr text);

int compare_states(const void* v1, const void* v2);

/* -------------------------------------------------------------
 * {function: analyze_regex}
 * -------------------------------------------------------------
 * Make an analysis of the pattern to determine flags like anchors.
 * (Intended to be used in the future when more features will be added)
 * -------------------------------------------------------------
 * {param: FlCstr regex} Pattern to analyze
 * {param: RegexFlags* flags} The function will change this when find certain flags like anchors
 * {param: RegexAnalysis* analysis} Will keep metainformation of the pattern
 * {param: FlError** error} If the analysis produces an error it will be set in this parameter
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void analyze_regex(FlCstr regex, RegexFlags *flags, RegexAnalysis *analysis, FlError **error)
{
	flm_assert(analysis != NULL, "RegexAnalysis cannot be NULL");
	size_t reglength = strlen(regex);
	analysis->patternStart = 0;
	analysis->patternEnd = reglength-1;

	if (regex[0] == '^')
	{
		SET_FLAG_START(*flags);
		analysis->patternStart++; // Start from 1, 0 is the ^
	}

	if (regex[reglength-1] == '$' && regex[reglength-2] != '\\')
	{
		SET_FLAG_END(*flags);
		analysis->patternEnd--; // End before $
	}
}

/* -------------------------------------------------------------
 * {function: parse_regex}
 * -------------------------------------------------------------
 * Parses the pattern in {regex} and populates {flags}
 * with parsing information. If an error occurs, the 
 * {error} struct is populated with useful information
 * about the failure
 * -------------------------------------------------------------
 * {return: FlVector*} Vector of FlCstr that contains the tokens of the parsed regex
 * -------------------------------------------------------------
 */
FlVector* parse_regex(FlCstr regex, RegexFlags *flags, FlError **error)
{
	RegexAnalysis analysis;
	analyze_regex(regex, flags, &analysis, error);

	if (fl_has_error(error))
		return NULL;

	char *tokens = fl_cstr_split_a(regex);
	// Function output (tokens)
	FlVector *output = fl_vector_new(sizeof(FlCstr), analysis.patternEnd+1);
	
	// Contains the backslash for escaped characters
	bool bslash = false;
	bool prev_bslash = false;

	// Flag to know when we are in a character class group operator ([] or [^])
	bool isCharClass = false;
	
	// Vars for tokens
	char cur = FL_EOS;
	char nex = FL_EOS;
	char prev = FL_EOS;
	for (int i=analysis.patternStart; i <= analysis.patternEnd; i++)
	{
		bslash = false;
		cur = tokens[i];
		nex = i==analysis.patternEnd ? FL_EOS : tokens[i+1];

		// Determine {cur} token and if it is escaped. Activate/deactivate char class, validate ranges inside char class, etc
		if (cur == '\\') 
		{
			if (nex == FL_EOS)
			{
				fl_error_set(error, -1, "Incomplete escaped value");
				break;
			}

			if (!is_escape_seq(nex))
			{
				fl_error_set(error, -1, "Unknown escaped value %c%c", cur, nex);
				break;
			}
			// Set bslash to \ and update cur and nex to i+1 (and i+2)
			bslash = true;
			cur = nex;
			i++;
			nex = i == analysis.patternEnd ? FL_EOS : tokens[i+1]; 
		} 
		else if (cur == '[') 
		{
			// If we ARE NOT in a char class group, a character [ will set the flag in true
			if (!isCharClass) 
			{
				isCharClass = true;
			} 
			else 
			{ 	// If we ARE in a char class group, we allow the character [, but we have to escape it, so we assign bslash
				bslash = true;
			}
		} 
		else if (cur == ']') 
		{
			// A char ] while we ARE in a char class group, finalize the group 
			if (isCharClass) 
			{
				isCharClass = false;
			} 
			else 
			{ 
				// But, if we ARE NOT in a char class group, we allow the character ] but we have to escape it.
				// Var cur is the previous nex, and in that moment, nex was an operator (]), so
				// we need to add a concatenator operator, because now, the ] character, is a literal bracket
				if (fl_vector_length(output) > 0)
				{
					fl_vector_add_cstr(output, "&");
				}
				bslash = true;
			}
		} 
		else if (cur == '^' && isCharClass && prev != '[') 
		{
			// Escape character ^ in a char class group, when it is not following an opening bracket.
			// Add an alternation operator that wasn't added when nex was ^ (it was considered an operator,
			// but now, we know it is a literal ^). If previous token was ^ we shouldn't add the alternation
			// because it was an operator, not an operand (e.g. [^^a])
			if (prev != '^' && !prev_bslash)
			{
				fl_vector_add_cstr(output, "|");
			}
			bslash = true;
		} 
		else if (cur == '-') 
		{
			// The character - has a different meaning inside a character class.
			// It always has a prev character that is an alphnumeric character, if not, we have
			// to escape it inside the character class
			if (isCharClass && prev != FL_EOS) 
			{
				if (isalnum(prev) && isalnum(nex)) 
				{
					bool upper;
					bool lower;
					upper = isupper(prev) && isupper(nex) && (int)prev < (int)nex;
					lower = !isupper(prev) && !isupper(nex) && (int)prev < (int)nex;
					if (!lower && !upper)
					{
						fl_error_set(error, -1, "Invalid range %c%c%c", (prev == FL_EOS ? '-' : prev), cur, (nex == FL_EOS ? '-' : nex));
						break;
					}
				} 
				else if (isdigit(prev) && isdigit(nex)) 
				{
					if ((int)prev >= (int)nex)
					{
						fl_error_set(error, -1, "Invalid range %c%c%c", (prev == FL_EOS ? '-' : prev), cur, (nex == FL_EOS ? '-' : nex));
						break;
					}
				} 
				else 
				{
					bslash = true;
				}
			} 
			else 
			{
				// If we are not in a character class group, or we ARE in a character class group
				// but the first character is the -, we have to escape it
				bslash = true;
			}
		}
		else if (cur == '&')
		{
			// Escape this when it is a user input. We'll use this char as a concat. operator
			bslash = true;
		}

		// If we are inside a character class, we need to escape the operators not used inside a character class
		bool curIsOperator = !bslash && is_operator(cur);
		bool isCharClassOperator = is_charclass_operator(cur);
		if (isCharClass && !isCharClassOperator && curIsOperator)
		{
			bslash = true;
		}

		// Prepare new token
		int length = bslash ? 2 : 1;
		FlCstr tmpstr = fl_cstr_new(length);
		if (length == 2) {
			tmpstr[0] = '\\';
			tmpstr[1] = cur;
		} else {
			tmpstr[0] = cur;
		}
		fl_vector_add(output, &tmpstr);

		// Save current token as previous for next iteration (save also prev_blash)
		prev = cur;
		prev_bslash = bslash;

		// If nex is null, we've reached the end of the input
		if (nex == FL_EOS)
			continue;

		// If we are in a char class, we don't need to concatenate the operands inside the char class
		if (isCharClass) 
			continue;

		// Check if we need to add a & operator between two literals, or an expression and a literal
		bool curAllowConcatRight = curIsOperator && allow_concat(CONCAT_RIGHT, cur);

		bool nexIsOp = !bslash && is_operator(nex);
		bool nexAllowConcatLeft = nexIsOp && allow_concat(CONCAT_LEFT, nex);
		if ((!curIsOperator || curAllowConcatRight) && (!nexIsOp || nexAllowConcatLeft))
		{
			fl_vector_add_cstr(output, "&");
		}
	}

	if (tokens)
		fl_array_delete(tokens);
	
	if (fl_has_error(error))
	{
		fl_vector_delete_ptrs(output);
		return NULL;
	}
	return output;
}

/* -------------------------------------------------------------
 * {function: parse_regex_postfix}
 * -------------------------------------------------------------
 * Parses the pattern in {regex} and populates {flags}
 * with parsing information. If an error occurs, the 
 * {error} struct is populated with useful information
 * about the failure.
 * -------------------------------------------------------------
 * {return: FlVector*} Vector of FlCstr that contains the tokens 
 * 	of the parsed regex. The tokens are in a postfix notation
 * -------------------------------------------------------------
 */
FlVector* parse_regex_postfix (FlCstr regex, RegexFlags *flags, FlError **error) 
{
	//error = NULL;
	if (!regex && !regex[0])
	{
		fl_error_set(error, -1, "Empty pattern");
		return NULL;
	}

	#if DEBUG
		printf("regex: /%s/\r\n", regex);
	#endif

	/* Parse, sanitize and get an array with the regex tokens */
	FlVector *tokens = parse_regex(regex, flags, error);
	if (tokens == NULL)
		return NULL;

	int length = strlen(regex);
	FlVector *stack = flm_vector_new(FlCstr, fl_vector_length(tokens));
	FlVector *output = flm_vector_new(FlCstr, fl_vector_length(tokens));

	bool inCharClass = false;
	int currentGroup = 0;
	/* Use the shunting-yard algorithm, with the particularity of doesn't discard the
	 * parentheses, instead use it as a unary operator. This way, we can implement
	 * capturing groups easly walking the resultant tree. */
 	char c;
	FlCstr token;
	FlCstr stackedtoken; // Temp string to use when poping from stack
	while (fl_vector_length(tokens)) {
		fl_vector_shift(tokens, &token);
		c = strlen(token) == 1 ? token[0] : FL_EOS; // FL_EOS makes 'c' fall in default: (escaped char => literal)

		switch (c) {
			case '[':
				// Character [ starts a char class group
				inCharClass = true;
				fl_vector_add(output, &token);
				fl_vector_add(stack, &token);
				break;
			case ']':
				// Character ] closes a char class group
				inCharClass = false;
				// Consume from the stack and send to the output until reach the starting character
				while(fl_vector_pop(stack, &stackedtoken) && stackedtoken[0] != '[')
				{
					fl_vector_add(output, &stackedtoken);
				}
				// Prevent /[]/ and /[^]/
				if ((fl_vector_length(output) == 1 && *flm_vector_get(output, FlCstr, 0) == '[')
					|| (fl_vector_length(output) == 2 && *flm_vector_get(output, FlCstr, 0) == '[' && *flm_vector_get(output, FlCstr, 1) == '^'))
				{
					fl_error_set(error, -1, "Empty character class is not allowed");
					break;
				}
				// parse_regex escapes ] when no [ is found, so this does not need validation like capturing groups
				fl_vector_add(output, &token);
				break;
			case '-':
				// If we ARE NOT in a char class, consume the character - as a literal value
				if (!inCharClass) {
					fl_vector_add(output, &token);
					break;
				}
				// If not we transform a range like [0-9] to [09-]
				FlCstr next;
				fl_vector_shift(tokens, &next);
				fl_vector_add(output, &next);
				fl_vector_add(output, &token);
				break;
			case '(':
				// Consume the parentheses. It is not only used for group operands and change operators
				// precedence, we use it as an operator
				currentGroup++;
				fl_vector_add(output, &token);
				fl_vector_add(stack, &token);
				break;
			case ')':
				// Consume from the stack and send to the output until reach the starting character
				while (fl_vector_pop(stack, &stackedtoken) && stackedtoken[0] != '(')
				{
					fl_vector_add(output, &stackedtoken);
				}
				if (stackedtoken[0] != '(')
				{
					fl_error_set(error, -1, "Invalid regular expression: Found character ')' but starting '(' was not found");
					break;
				}
				currentGroup--;
				fl_vector_add(output, &token);
				break;
			case '*':
			case '+':
			case '?':
			case '&':
			case '|': 
			{
				// When we obtain an operator, we need to check where we will send it (output or stack)
				char o1 = c;
				char o2 = fl_vector_length(stack) == 0 ? FL_EOS : flm_vector_get(stack, FlCstr, fl_vector_length(stack)-1)[0]; // First char

				// If in the stack there's NOT a previous operator, we send the current operator to the stack
				if (o2 == FL_EOS || !regex_operator_exists(o2))
				{
					fl_vector_add(stack, &token);
					break;
				}

				// If there is a previuos operator, we need to check precedence and associativity of both operators
				char p1 = regex_operator_get_precedence(o1); // Precedence of operator 1
				char a1 = regex_operator_get_assoc(o1); // Associativity of operator 1


				while (true) {
					o2 = fl_vector_length(stack) == 0 ? FL_EOS : flm_vector_get(stack, FlCstr, fl_vector_length(stack)-1)[0]; // First char
					
					// Break the loop when we find the end of the stack or a grouping operator
					if (o2 == FL_EOS || !regex_operator_exists(o2))
						break;
					
					char p2 = regex_operator_get_precedence(o2); // Precedence of operator 2
					char a2 = regex_operator_get_assoc(o2); // Associativity of operator 2
					
					// IF assoc. of operator 1 is left and has equal or greater precedence than operator 2
					// 	OR assoc. of operator 1 is right and has greater precedence than operator 2
					// THEN
					// 	we have to remove the operator 2 from the stack and send it to the output and
					// 	finally we add the operator 1 to the stack.
					// ELSE
					// 	we add the operator 1 into the stack (above the operator 2)
					bool needsToPopOp2 = (a1 == 'l' && p1 <= p2) || (a1 == 'r' && p1 < p2);
					if (!needsToPopOp2)
						break;
					
					FlCstr o2s;
					fl_vector_pop(stack, &o2s);
					fl_vector_add(output, &o2s);
				}
				fl_vector_add(stack, &token);
				break;
			}
			default:
				// Literal chars are sended directly to the output
				fl_vector_add(output, &token);
		}
		if (fl_has_error(error))
			break;
	}

	if (!fl_has_error(error))
	{
		// Finally, remove all the pending operators from the stack, and send it to the output
		while (fl_vector_length(stack))
		{
			FlCstr sc;
			fl_vector_pop(stack, &sc);
			fl_vector_add(output, &sc);
		}

		if (inCharClass)
		{
			fl_error_set(error, -1, "Invalid regular expression: missing closing ']'");
		}
		else if (currentGroup != 0)
		{
			fl_error_set(error, -1, "Invalid regular expression: missing closing ')'");
		}
	}

	#if DEBUG
		printf("regex2postfix: /%s/\r\n", fl_cstr_join(output, " "));
	#endif

	fl_vector_delete_ptrs(stack);
	fl_vector_delete_ptrs(tokens);
	return output;
}

/* -------------------------------------------------------------
 * {function: fl_regex_compile}
 * -------------------------------------------------------------
 * Given a certain pattern, this function parses and compiles it
 * into an FlRegex struct that contains a set of NfaStates that
 * represents the NFA for the given regular expression
 * -------------------------------------------------------------
 * {return: FlRegex*} NFA for the given pattern
 * -------------------------------------------------------------
 */
FlRegex* fl_regex_compile (FlCstr pattern, FlError **error)
{
	RegexFlags flags = 0;
	// parse_regex pattern and convert it to postifx notation
	FlVector *tokens = parse_regex_postfix(pattern, &flags, error);
	if (fl_has_error(error))
		return NULL;
	size_t nstates = fl_vector_length(tokens) + 1;

	#define PushState(s) do { *statesp++ = s; *stackp++ = operands_last_index++; } while (0)
	#define PushOperandIndex(val) (*stackp++ = val)
	#define PopOperandIndex() (*--stackp)
	short stack[nstates];
	short operands_last_index = 0, *stackp = stack, leftset, rightset;

	#if DEBUG
	{
		FlCstr postfix = fl_cstr_join(tokens, " ");
		printf("postfix: /%s/\r\n", postfix);
		fl_cstr_delete(postfix);
	}
	#endif

	FlRegex *regex = fl_calloc(sizeof(FlRegex), 1);
	regex->pattern = fl_cstr_dup(pattern);
	regex->flags = flags;
	regex->states = fl_array_new(sizeof(NfaState*), nstates);
	NfaState **statesp = regex->states;

	// Create the initial state and add a concatenation operator between S0 and the rest of the regex
	int stateId = 0;
	NfaState *s = create_nfa_state(stateId++, "S0");
	fl_vector_add_cstr(tokens, "&");
	PushState(s);

	// Variables to handle capturing groups indexes
	short nextcapgroup = 1;
	short lastcg = 0;

	// Current token. Because token can be an escaped sequence, tokenval is used to keep the real value
	// When token = "\[" => tokenval = '['
	FlCstr token;
	char tokenval;

	while(fl_vector_length(tokens))
	{
		// Remove tokens from beginning
		fl_vector_shift(tokens, &token);
		tokenval = strlen(token) == 1 ? token[0] : token[1];

		if (tokenval == '(')
		{
			// {todo: Implement capturing groups}]>
			nextcapgroup++;
		}
		else if (tokenval == ')')
		{
			// {todo: Implement capturing groups}]>
		} 
		// If tokenval is not an operator, add a new state with {token} value
		else if (!fl_array_contains_n(CompileOperators, flm_array_length(CompileOperators), token, strlen(token)))
		{
			s = create_nfa_state(stateId++, token);
			PushState(s);
		}
		else
		{
			switch (tokenval)
			{
				case '&':
					rightset = PopOperandIndex();
					leftset = PopOperandIndex();
					#if DEBUG
						nfa_print_operands("AND", regex->states, leftset, rightset, operands_last_index);
					#endif
					nfa_concat(regex->states, leftset, rightset, rightset, operands_last_index);
					PushOperandIndex(leftset);
					break;
				case '|':
					rightset = PopOperandIndex();
					leftset = PopOperandIndex();
					#if DEBUG
						nfa_print_operands("OR", regex->states, leftset, rightset, operands_last_index);
					#endif
					PushOperandIndex(leftset);
					break;
				case '+':
					leftset = PopOperandIndex();
					if (leftset == 0) // S0 is an invalid regexp
					{
						fl_error_set(error, -1, "Invalid regular expression: There is no operand for '%c'", tokenval);
						break;
					}
					#if DEBUG
						nfa_print_operands("+", regex->states, leftset, operands_last_index, -1);
					#endif
					nfa_repeat(regex->states, leftset, operands_last_index);
					PushOperandIndex(leftset);
					break;
				case '*':
				case '?':
				{
					leftset = PopOperandIndex();
					if (leftset == 0) // S0 is an invalid regexp
					{
						fl_error_set(error, -1, "Invalid regular expression: There is no operand for '%c'", tokenval);
						break;
					}
					#if DEBUG
						nfa_print_operands(token, regex->states, leftset, operands_last_index, -1);
					#endif
					// Add an epsilon transition before LS
					// a? => (S0)--ε-->((1))--a-->((2))
					//
					if (tokenval == '*')
					{
						// Add a transition from 2 to 2 (repeat)
						// a* => (S0)--ε-->((1))--a-->((2))<--.
						//				 	     	    |     |
						//				 	     	    '--a--'						
						nfa_repeat(regex->states, leftset, operands_last_index);
					}

					// Just for clarity, adjust the IDs
					for (int i=leftset; i < operands_last_index; i++)
					{
						regex->states[i]->id++;
					}
					PushOperandIndex(leftset);

					NfaState* es = create_nfa_state(leftset, "Epsilon");
					stateId++;
					STATE_SET_INITIAL(es);
					STATE_SET_FINAL(es);
					PushState(es);
					rightset = PopOperandIndex();
					leftset = PopOperandIndex();
					#if DEBUG
						nfa_print_operands("AND", regex->states, leftset, rightset, operands_last_index);
					#endif
					nfa_concat(regex->states, rightset, operands_last_index, leftset, rightset);
					PushOperandIndex(leftset);
					STATE_SET_FINAL(es);
				
					break;
				}
				case '[':
				{
					FlVector *tmptokens = fl_vector_new(sizeof(FlCstr), 0);
					FlVector *ascii_codes = fl_vector_new(sizeof(int), 0);
					FlVector *display = fl_vector_new(sizeof(FlCstr), 0);
					// Temporary variable to look ahead the next token
					FlCstr nexttok;
					// ASCII codes
					int ord;

					// Look ahead
					fl_vector_shift(tokens, &nexttok);
					if (flm_cstr_equals(nexttok, "^")) 
					{
						fl_vector_add_cstr(display, "^");
						s = create_charclass_nfa_state(stateId++, NULL, true);
					} 
					else 
					{
						if (!flm_cstr_equals(nexttok, "]"))
							fl_vector_add_cstr(tmptokens, nexttok);
						s = create_charclass_nfa_state(stateId++, NULL, false);
					}

					STATE_SET_INITIAL(s);
					STATE_SET_FINAL(s);
					//foreach ($cgroups as $cgroup) {
					//	$s->cgroups[] = new RECapGroup($cgroup, false);
					//}

					if (!flm_cstr_equals(nexttok, "]")) 
					{
						// Consume the tokens until reach the ending ]
						FlCstr tmptoken;
						while (fl_vector_shift(tokens, &tmptoken)) 
						{
							if (flm_cstr_equals(tmptoken, "]"))
								break;
							fl_vector_add(tmptokens, &tmptoken);
						}
						fl_cstr_delete(tmptoken);
					}

					size_t l = fl_vector_length(tmptokens);
					for (size_t i = 0; i < l; i++)
					{
						FlCstr tmptoken = flm_vector_get(tmptokens, FlCstr, i);
						if (flm_cstr_equals(tmptoken, "-")) {
							int endindex;
							int startindex;
							fl_vector_pop(ascii_codes, &endindex);
							fl_vector_pop(ascii_codes, &startindex);
							for (int i=startindex; i <= endindex; i++)
								((NfaStateCharClass*)s)->map[i] = 1;
							
							FlCstr endchar;
							fl_vector_pop(display, &endchar);
							fl_vector_add_cstr(display, "-");
							fl_vector_add(display, &endchar);
						} else {
							if (tmptoken[0] == '\\')
							{
								ord = tmptoken[1];
								flm_vector_addl(ascii_codes, int, ord);
							}
							else
							{
								ord = tmptoken[0];
								flm_vector_addl(ascii_codes, int, ord);
							}
							fl_vector_add_cstr(display, tmptoken);
						}
					}

					while (fl_vector_shift(ascii_codes, &ord))
					{
						((NfaStateCharClass*)s)->map[ord] = 1;
					}

					fl_vector_unshift_cstr(display, "/[");
					fl_vector_add_cstr(display, "]/");
					((NfaStateCharClass*)s)->value = fl_cstr_join(display, "");

					// Add the new charclass state
					PushState(s);

					// Delete nexttok, ascii_codes already mapped, display vector and tmptokens vector
					fl_cstr_delete(nexttok);
					fl_vector_delete(ascii_codes);
					fl_vector_delete_ptrs(display);
					fl_vector_delete_ptrs(tmptokens);
				}
			}

		}
		// Delete consumed token
		fl_cstr_delete(token);

		if (fl_has_error(error))
			break;
	}
	// Sort the regex to retrieve States by its ID like array indexes
	// regex->states[n] is the State->id == n
	qsort(regex->states, fl_array_length(regex->states), sizeof(NfaState*), compare_states);
	#if DEBUG
	print_nfa(regex->states);
	#endif
	
	if (fl_has_error(error))
	{
		fl_regex_delete(regex);
		fl_vector_delete_ptrs(tokens);
		return NULL;
	}
			
	fl_vector_delete(tokens);
	return regex;
}

/* -------------------------------------------------------------
 * {function: delete_nfa}
 * -------------------------------------------------------------
 * Releases the memory used by an {NfaState}
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void delete_nfa(FlByte *statebytes)
{
	if (statebytes == NULL)
		return;
	NfaState *s = *(NfaState**)statebytes;
	if (s == NULL)
		return;

	if (s->value)
		fl_free(s->value);
	if (s->to)
		fl_vector_delete(s->to);
	fl_free(s);
}

/* -------------------------------------------------------------
 * {function: create_nfa_state}
 * -------------------------------------------------------------
 * Creates an NFA state with the provided ID and the value needed
 * to reach the state
 * -------------------------------------------------------------
 * {param: int id} ID to be assigned to the new state
 * {param: FlCstr value} Value needed to reach the new state
 * -------------------------------------------------------------
 * {return: NfaState*} The created NfaState
 * -------------------------------------------------------------
 */
NfaState* create_nfa_state(int id, FlCstr value)
{
	NfaState *s = fl_malloc(sizeof(NfaState));
	s->value = fl_cstr_dup(value);
	s->id = id;
	STATE_SET_INITIAL(s);
	STATE_SET_FINAL(s);
	s->to = fl_vector_new(sizeof(NfaState*), 0);
	s->type = NFA_STATE;
	return s;
}

/* -------------------------------------------------------------
 * {function: create_charclass_nfa_state}
 * -------------------------------------------------------------
 * Creates an NFA state that represents a character class operator
 * -------------------------------------------------------------
 * {param: int id} ID of the state
 * {param: FlCstr value} Representation of the character class (it is NOT used to run the NFA)
 * {param: bool negated} When the character class contains is a negated one
 * -------------------------------------------------------------
 * {return: NfaState*} Created state
 * ------------------------------------------------------------- 
 */
NfaState* create_charclass_nfa_state(int id, FlCstr value, bool negated)
{
	NfaStateCharClass *s = fl_malloc(sizeof(NfaStateCharClass));
	s->value = value != NULL ? fl_cstr_dup(value) : NULL;
	s->id = id;
	STATE_SET_INITIAL(s);
	STATE_SET_FINAL(s);
	s->to = fl_vector_new(sizeof(NfaState*), 0);
	s->negated = negated;
	memset(s->map, 0, sizeof(int)*256);
	s->type = NFA_CHARCLASS_STATE;
	return (NfaState*)s;
}

/* -------------------------------------------------------------
 * {function: nfa_print_operands}
 * -------------------------------------------------------------
 * Show the operands for a given operator while we are creating the NFA.
 * It is used for debug purpouses
 * -------------------------------------------------------------
 * {param: const char* operator} String representation of the operator
 * {param: NfaState* states} Set of states that form the NFA
 * {param: short s} Min index
 * {param: short m} Mid index
 * {param: short e} Max index
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void nfa_print_operands(const char *operator, NfaState **states, short s, short m, short e)
{
	printf("-- %s: Operands\n", operator);
	printf("Left Set = {");
	for (short i=s; i < m; i++)
		printf("%s%s", states[i]->value, i == m-1 ? "" : ", ");
	if (e > -1)
	{
		printf("} | Right Set = {");
		for (short i=m; i < e; i++)
			printf("%s%s", states[i]->value, i == e-1 ? "" : ", ");
	}
	printf("}\n");
	printf("----------------------------\n");
}

/* -------------------------------------------------------------
 * {function: nfa_concat}
 * -------------------------------------------------------------
 * Adds transitions between states. Concatenation is a binary
 * operator that takes a left operand (LS) and a right operand
 * (RS).
 * LS = states[ls] to states[le]
 * RS = states[rs] to states[re]
 * -------------------------------------------------------------
 * {param: NfaState** states} Set of states that form the NFA
 * {param: short ls} LS start index
 * {param: short le} LS end index
 * {param: short rs} RS start index
 * {param: short re} RS end index
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void nfa_concat(NfaState **states, short ls, short le, short rs, short re)
{
	NfaState *left, *right;
	for (short i=ls; i < le; i++)
	{
		left = states[i];
		if (!STATE_IS_FINAL(left))
			continue;
		STATE_UNSET_FINAL(left);
		for (short j=rs; j < re; j++)
		{
			right = states[j];
			if (!STATE_IS_INITIAL(right))
				continue;
			fl_vector_add(left->to, &right);
		}
	}

	for (short j=rs; j < re; j++)
	{
		right = states[j];
		if (!STATE_IS_INITIAL(right))
			continue;
		STATE_UNSET_INITIAL(right);
	}
}

/* -------------------------------------------------------------
 * {function: nfa_repeat}
 * -------------------------------------------------------------
 * Repetition is a unary operator that take a left operand (LS)
 * and adds transitions from LS final states to LS initial ones
 * -------------------------------------------------------------
 * {param: NfaState** states} Set of states that form the NFA
 * {param: short ls} LS start index
 * {param: short le} LS end index
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void nfa_repeat (NfaState **states, short ls, short le)
{
	NfaState *left, *left2;
	for (short i=ls; i < le; i++)
	{
		left = states[i];
		if (!STATE_IS_FINAL(left))
			continue;

		for (short j=ls; j < le; j++)
		{
			left2 = states[j];
			if (!STATE_IS_INITIAL(left2))
				continue;
			fl_vector_add(left->to, &left2);
		}
	}
}

/* -------------------------------------------------------------
 * {function: compare_states}
 * -------------------------------------------------------------
 * Used by qsort to sort the states by ID. The IDs start at 0,
 * shorting them we can access them by 0-based indexes
 * -------------------------------------------------------------
 * {return: int} Negative value if v1 < v2, 0 if equals, positive if v1 > v2
 * -------------------------------------------------------------
 */
int compare_states(const void* v1, const void* v2)
{
	const NfaState *s1 = *(const NfaState**)v1;
	const NfaState *s2 = *(const NfaState**)v2;
	if (s1 == NULL)
		return 10000; 
	else if (s2 == NULL)
		return -10000;
	return s1->id - s2->id;
}

/* -------------------------------------------------------------
 * {function: print_nfa}
 * -------------------------------------------------------------
 * Walks the NFA printing its states
 * -------------------------------------------------------------
 * {param: NfaState** states} Set of states that form the NFA
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void print_nfa (NfaState **states)
{
	bool first = true;
	size_t l = fl_array_length(states);
	NfaState* state;
	for (size_t i = 0; i < l; i++)
	{
		state = states[i];
		if (state == NULL)
			break;
		if (!first)
			printf("\r\n");
		first = false;
		print_nfa_state(state);
	}
	printf("\r\n");
}

/* -------------------------------------------------------------
 * {function: print_nfa_state}
 * -------------------------------------------------------------
 * Prints out a representation of the state
 * -------------------------------------------------------------
 * {param: NfaState* state} State to print
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void print_nfa_state(NfaState *state)
{
	printf(STATE_IS_FINAL(state) ? "((" : " (");
	printf("%d", state->id);
	printf(STATE_IS_FINAL(state) ? "))" : ") ");
	
	printf("|'%s'", state->value);

	if (STATE_IS_FINAL(state) || STATE_IS_INITIAL(state))
	{
		printf("|");
		printf("%s%s", (STATE_IS_INITIAL(state) ? "i":""), (STATE_IS_FINAL(state) ? "f":""));
	}

	if (fl_vector_length(state->to) > 0)
	{
		printf(" => {");
		size_t l = fl_vector_length(state->to);
		for (size_t i = 0; i < l; i++)
		{
			NfaState *dest = flm_vector_get(state->to, NfaState*, i);
			printf("%s->%d", dest->value, dest->id);
			if (i < l-1)
				printf(", ");
		}
		printf("}");
	}
}

/* -------------------------------------------------------------
 * {function: nfa_step}
 * -------------------------------------------------------------
 * Standing in state {state}, uses {value} to make al the 
 * possible transitions to the next step of the NFA
 * -------------------------------------------------------------
 * {param: NfaState* state} Current state
 * {param: CurrentState[] nextstates} Reached states from {state}
 * {param: unsigned char value} Input value for the current step
 * {param: const CurrentState[] currentstates} Already reached states
 * -------------------------------------------------------------
 * {return: NfaStepResult} Contains info of the step result.
 * 	If at least one state was reached, result.anyMatch will be true.
 *	If at least one final state was reached, result.anyFinal will be
 * 	true.
 * -------------------------------------------------------------
 */
NfaStepResult nfa_step (NfaState *state, CurrentState nextstates[], unsigned char value, const CurrentState currentstates[])
{
	NfaStepResult stepresult = {false, false};

	if (state->to == NULL)
		return stepresult;

	size_t l = fl_vector_length(state->to);
	for (size_t i = 0; i < l; i++)
	{
		NfaState *t = flm_vector_get(state->to, NfaState*, i);
	 	// If e-transition reach an state that already is in the next states, avoid it to prevent loops
	 	// We can have e-transitions reaching each one to the other here.
	 	if (nextstates[t->id].id != -1 && nextstates[t->id].e)
	 	{
	 		continue;
	 	}

		// If e-transition reach an state that already is in the current states, avoid it.
		// This result in a cleaner representation of the current step and reduce the time complexity
	 	if (currentstates != NULL && (currentstates[t->id].id != -1 && currentstates[t->id].e))
	 	{
	 		continue;
	 	}

	 	if (can_reach_state(t, value)) 
	 	{
			nextstates[t->id] = (CurrentState){ t->id, false };
	 		stepresult.anyMatch = true;
	 		if (STATE_IS_FINAL(t) || nfa_step(t, nextstates, FL_EOS, NULL).anyFinal)
	 			stepresult.anyFinal = true;
	 	}

	 	if (flm_cstr_equals(t->value, "Epsilon"))
	 	{
	 		nextstates[t->id] = (CurrentState){ t->id, true };
	 		stepresult.anyMatch = true;
	 		if (STATE_IS_FINAL(t) || nfa_step(t, nextstates, FL_EOS, NULL).anyFinal)
	 			stepresult.anyFinal = true;
	 	}
	}

	return stepresult;
}

/* -------------------------------------------------------------
 * {function: can_reach_state}
 * -------------------------------------------------------------
 * Returns true if {state} is reachable with the given input
 * -------------------------------------------------------------
 * {param: NfaState* state} State to try to reach
 * {param: unsigned char value} Input to check to reach {state}
 * -------------------------------------------------------------
 * {return: bool} true if {state} can be reached with {value}
 * -------------------------------------------------------------
 */
bool can_reach_state (NfaState *state, unsigned char value)
{
	if (value == FL_EOS || state == NULL || flm_cstr_equals(state->value, "Epsilon"))
		return false;
	
	// Common states with a single value
	if ((state->value[0] == '\\' && state->value[1] == value) 
		|| (state->value[0] == '.' && value != '\n')
		|| state->value[0] == value)
		return true;

	if (state->type == NFA_STATE)
		return false;
	
	NfaStateCharClass *statecc = (NfaStateCharClass*)state;
	// Characeter classes like [] operator and \d, \w, etc OR Negated characeter classes like [^] operator and \D, \W, etc
	return	(!statecc->negated && statecc->map[value] == 1) || (statecc->negated && statecc->map[value] == 0);
}

/* -------------------------------------------------------------
 * {function: regex_match}
 * -------------------------------------------------------------
 * Check the input text agains the regex and returns true if
 * the input matches the pattern
 * -------------------------------------------------------------
 * {param: FlRegex* regex} Regular expression
 * {param: FlCstr text} Input string to check against pattern
 * -------------------------------------------------------------
 * {return: bool} True if text matches the pattern
 * -------------------------------------------------------------
 */
bool regex_match (FlRegex *regex, FlCstr text)
{
	size_t currentsl = fl_array_length(regex->states);
	CurrentState currents[currentsl];
	memset(currents, -1, sizeof(CurrentState) * currentsl);
	CurrentState nexts[currentsl];
	memset(nexts, -1, sizeof(CurrentState) * currentsl);
	// Previous to run the nfa we've only one state, the initial state
	NfaState *s0 = regex->states[0];//*(NfaState**)fl_vector_get(regex->states, 0);
	currents[s0->id] = (CurrentState){ s0->id, false };

	FlVector *input = fl_cstr_split(text);

	// Run the first step without input to reach all possible states we can reach with e-transitions
	// nfa_step returns an array as result with
	// Index 0 -> True if there is a match (a transition from one of the current states to another)
	// Index 1 -> If one of the reached states is Final, this value is true
	NfaStepResult result;
	NfaStepResult stepresult = nfa_step(s0, currents, FL_EOS, NULL);

	// stepresult.anyMatch will contain the partial result of a nfa_step when a new state is reached
	result.anyMatch = stepresult.anyMatch;
	// stepresult.anyFinal will contain the partial result of a nfa_step when a final state is reached
	result.anyFinal = stepresult.anyFinal;

	// Related to '^' operator
	bool firstinput = true;

	// if (DEBUG_NFA_RUN) {
	// 	echo "= Run NFA", "\r\n", " - Pattern: /{$nfa->pattern}/", "\r\n", " - Input: '{$text}'", "\r\n";
	// 	print_nfa([$currents], false, "= Start", "  |--");
	// }

	char in;
	while(fl_vector_length(input))
	{
		fl_vector_shift(input, &in);
		// Clean stepresult when regex_match has an input
		stepresult = (NfaStepResult){ false, false };
		// Clean result.anyMatch for the new iteration
	 	result.anyMatch = false;

		// nexts will contain all the reached states after a nfa_step
	 	for (int i=0; i < currentsl; i++)
	 	{
	 		if (currents[i].id == -1)
	 			continue;
	 		NfaState *state = regex->states[i]; //*(NfaState**)fl_vector_get(regex->states, i);
	 		stepresult = nfa_step(state, nexts, in, currents);
	 		// If we reached any new state, set result.anyMatch to true
	 		if (!result.anyMatch && stepresult.anyMatch)
	 			result.anyMatch = true;
	
	 		// If we found a final state, set result.anyFinal to true and check if it is possible to break regex_match
	 		if (stepresult.anyFinal)
	 			result.anyFinal = true;
	 	}		

		// If we matched something, update the current set of states
	 	if (result.anyMatch)
	 	{
	 		for (int j=0; j < currentsl; j++)
	 		{
	 			currents[j] = nexts[j];
	 		}
	 		memset(nexts, -1, sizeof(CurrentState)*currentsl);
	 	}

	// 	if (DEBUG_NFA_RUN) {
	// 		if (!stepresult.anyMatch)
	// 			$nexts = $currents;
	// 		print_nfa([$nexts], false, "= After input {$in}", "  |--");
	// 	}

		// When full match is not necessary, short-circuit the execution when match is successful
	 	if (result.anyFinal && !HAS_FLAG_END(regex->flags))
	 		break;

		// If the firstinput match fails when ^ is present break the execution
	 	if (firstinput) {
	 		firstinput = false;
	 		if (!result.anyMatch && HAS_FLAG_START(regex->flags))
	 			break;
	 	}
	}

	// If $ is present and the last match didn't match anything, se the result to false
	if (result.anyFinal && !result.anyMatch && HAS_FLAG_END(regex->flags))
	 	result.anyFinal = false;

	// if (DEBUG_NFA_RUN)
	// {
	// 	if (result.anyFinal)
	// 		echo "Result: Accepted", "\r\n";
	// 	else
	// 		echo "Result: Not accepted", "\r\n";
	// 	echo "-----------------", "\r\n";
	// }
	// return result.anyFinal;
	fl_vector_delete(input);
	return result.anyFinal;
}

/* -------------------------------------------------------------
 * Public API functions
 * -------------------------------------------------------------
 */
bool fl_regex_match(FlRegex *regex, FlCstr text)
{
	return regex_match(regex, text);
}

/* -------------------------------------------------------------
 * {function: fl_regex_delete}
 * -------------------------------------------------------------
 * Free the memory used by an FlRegex struct. To free the memory
 * used by member states, this function uses the handler {delete_nfa}
 * -------------------------------------------------------------
 * {param: FlRegex* regex} target FlRegex struct to cleanup
 * -------------------------------------------------------------
 * {return: void}
 * -------------------------------------------------------------
 */
void fl_regex_delete (FlRegex *regex)
{
	// Delete FlVector *states with delete_nfa handler
	fl_array_delete_h(regex->states, delete_nfa);
	fl_free(regex->pattern);
	fl_free(regex);
}
