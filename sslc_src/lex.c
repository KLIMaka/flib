#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"
#include "parse.h"
#include "parselib.h"
#include "extra.h"

LexData lexData;
static char *tokens[256+T_END_TOKEN];
static struct {
   LexData c;
   char *name;
} constants[C_END_CONSTANT];

static int ungotToken = -1;
static int lastToken = -1;
static int inited = 0;


/* Old behaviour atoi (begin) */

int old_atoi(const char* p )
{
	int value;
	char sign;

	while(isspace(*p)) {
		++p;
	}

	sign = *p;
	
	if (sign == '+' || sign == '-' ) {
		++p;
	}

	value = 0;
	
	while(isdigit(*p)) {
		value = value * 10 + *p - '0';
		++p;
	}

	if (sign == '-' ) {
		value = - value;
	}

	return( value );
}

/* Old behaviour atoi (end) */

void lexClose(void) { inited = 0; }
void initLex(void) {
   int i;

   if (inited) return;

   inited = 1;
   lastToken = -1;
   ungotToken = -1;

   for (i=0; i<256; ++i) {
      tokens[i] = 0;
   }

   tokens[T_PROCEDURE] = "procedure";
   tokens[T_VARIABLE]  = "variable";
   tokens[T_IMPORT]    = "import";
   tokens[T_EXPORT]    = "export";
   tokens[T_IN]        = "in";
   tokens[T_WHEN]      = "when";
   tokens[T_BEGIN]     = "begin";
   tokens[T_END]       = "end";
   tokens[T_EXIT]      = "exit";
   tokens[T_DETACH]    = "detach";
   tokens[T_FORK]      = "fork";
   tokens[T_SPAWN]     = "spawn";
   tokens[T_CALLSTART] = "callstart";
   tokens[T_EXEC]      = "exec";

   tokens[T_NOOP]      = "noop";
   tokens[T_WAIT]      = "wait";     // wait specified # milliseconds
   tokens[T_CALL]      = "call";     // call a function
   tokens[T_CANCEL]    = "cancel";   // cancel pending events for function
   tokens[T_CANCELALL] = "cancelall";
   tokens[T_IF]        = "if";
   tokens[T_THEN]      = "then";
   tokens[T_ELSE]      = "else";
   tokens[T_END]       = "end";
   tokens[T_EQUAL]     = "==";
   tokens[T_NOT_EQUAL] = "!=";
   tokens[T_LESS_EQUAL]= "<=";
   tokens[T_GREATER_EQUAL] = ">=";
   tokens[T_ASSIGN]    = ":=";
   tokens[T_AND]       = "and";
   tokens[T_OR]        = "or";
	tokens[T_BWAND]     = "bwand";
	tokens[T_BWOR]      = "bwor";
   tokens[T_BWNOT]     = "bwnot";
   tokens[T_BWXOR]     = "bwxor";
	tokens[T_NOT]       = "not";
	tokens[T_FLOOR]	  = "floor";
   tokens[T_RETURN]    = "return";
   tokens[T_DO]        = "do";
   tokens[T_WHILE]     = "while";
   tokens[T_INCLUDE]   = "include";
   tokens[T_STARTCRITICAL] = "startcritical";
   tokens[T_ENDCRITICAL]   = "endcritical";
   tokens[T_CRITICAL]  = "critical";

   lexAddConstant(C_TRUE, "true", T_INT, 1);
   lexAddConstant(C_FALSE, "false", T_INT, 0);
   initLibExtras();
}

static int lookupConstant(char *name) {
   int i;

   for (i=0; i<C_END_CONSTANT; ++i) {
      if (constants[i].name && stricmp(constants[i].name, name) == 0)
         return i;
   }
   return -1;
}

void lexAddConstant(int c, char *name, int type, unsigned long val) {
   constants[c].name = name;
   constants[c].c.type = type;
   switch(type) {
      case T_INT:    constants[c].c.intData = val; break;
      case T_FLOAT:  constants[c].c.floatData = *(float *)&val; break;
      case T_STRING: constants[c].c.stringData = *(char **)&val; break;
      default: printf("Unknown constant type %x\n", type); exit(1);
   }
}

void lexAddToken(int token, char *name) {
   tokens[token] = name;
}

static int lookupToken(char *string) {
   int i;

   for (i=256; i<T_END_TOKEN; ++i)
      if (tokens[i] && stricmp(string, tokens[i]) == 0)
         break;

   return i == T_END_TOKEN ? -1 : i;
}

/*
 * tokens are white-space delimited.
 * all single-character tokens are defined by their character;
 * token a is 'a', token b is 'b', etc.
 * Only end-of-line comments are supported; '#' is the start of an
 * end-of-line comment.
 */

static InputStream *stream;

char *lexGetFilename(InputStream *s) { return  s ? s->name : 0; }
/*
 * read a character from the current input stream
 */
static int getChar(void) {
   int ret = -1;

   if (!stream) return -1;
   switch(stream->type) {
      case INPUT_FILE:
         ret = (stream->lastChar = fgetc(stream->file));
         break;
      case INPUT_STRING:
         if (stream->s.curpos >= stream->s.string + stream->s.len) {
            stream->s.readEOF = 1;
            ret = (stream->lastChar = EOF);
         }
         else
            ret = (stream->lastChar = *stream->s.curpos++);
         break;
   }

   if (ret == '\n') stream->lineno++;
   return ret;
}

static void ungetChar(void) {
   if (!stream) return;
   if (stream->lastChar == '\n') stream->lineno--;
   switch(stream->type) {
      case INPUT_FILE:
         ungetc(stream->lastChar, stream->file);
         break;
      case INPUT_STRING:
         if (stream->s.readEOF)
            stream->s.readEOF = 0;
         else if (stream->s.curpos != stream->s.string)
            stream->s.curpos--;
         break;
   }
   stream->lastChar = -1;
}

static int expect(int what, int hit, int miss) {
   int c = getChar();
   if (c == what) return hit;
   else { ungetChar(); return miss; }
}

static int validSymbolChar(int c) {
   return (c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z') ||
          (c >= '0' && c <= '9') ||
           c == '_';
}

#define GOT_DECIMAL 0x01
#define GOT_MINUS   0x02
#define GOT_PLUS    0x04

static int validNumberChar(int c) {
   return (c >= '0' && c <= '9') || c == '.';
}

static int validHexNumberChar(int c) {
   return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

static void parseNumber(void) {
   int c;
   char buf[80];
   int i = 0;
   int floatFlag = 0;
   int hexFlag = 0;

   do {
      c = getChar();
      buf[i++] = c;

	  if (c == '.')
         floatFlag = 1;
   } while(validNumberChar(c));

   buf[--i] = 0;
   ungetChar();

   if (floatFlag) {
      lexData.type = T_FLOAT;
      lexData.floatData = (float)atof(buf);
   }
   else {
      lexData.type = T_INT;
      lexData.intData = old_atoi(buf);
   }
}

static void parseCharConstant(void) {
   int c;

   c = getChar();

   if (c != '\'')
      return;

   c = getChar();

   if (c == '\\') {
      c = getChar();
      switch(c) {
         case 'a': c = '\a'; break;
         case 'b': c = '\b'; break;
         case 'f': c = '\f'; break;
         case 'n': c = '\n'; break;
         case 'r': c = '\r'; break;
         case 't': c = '\t'; break;
         case 'v': c = '\t'; break;
         case '0': {
            int d;

            d = getChar();
            if (!isdigit(d)) parseError("Illegally formatted octal constant\n");
            d = d - '0';
            if (d > 7)
               parseError("Illegally formatted octal constant\n");
            c = d;

            d = getChar();
            if (!isdigit(d)) parseError("Illegally formatted octal constant\n");
            d = d - '0';
            if (d > 7)
               parseError("Illegally formatted octal constant\n");
            c = (c * 8) + d;

            d = getChar();
            if (d == '\'')
               ungetChar();
            else {
               if (!isdigit(d)) parseError("Illegally formatted octal constant\n");
               d = d - '0';
               if (d > 7)
                  parseError("Illegally formatted octal constant\n");
               c = (c * 8) + d;
            }
            break;
         }
         default:
            parseError("Illegally formatted escaped character\n");
            break;
      }
   }
   lexData.type = T_INT;
   lexData.intData = c;

   c = getChar();
   if (c != '\'')
      parseError("Illegally formated character constant\n");
}

static void parseString(char *buf)
{
	int c;
	int i = 0;

	while(1) {
		c = getChar();

		if (c == EOF) {
			break;
		}

		if (c == '"') {   // got end of string, find next line
			do {
				c = getChar();
			} while(c == ' ' || c == '\t' || c == '\n' || c == '\v');

			if (c == '"')  // two strings back to back
				continue;
			else {         // single string
				ungetChar();
				break;
			}
		}

		if (c == '\\') {  // got escape sequence
			c = getChar();

			switch(c) {
				case 'a':
					c = '\a';
					break;

				case 'b':
					c = '\b';
					break;

				case 'f':
					c = '\f';
					break;

				case 'n':
					c = '\n';
					break;

				case 'r':
					c = '\r';
					break;

				case 't':
					c = '\t';
					break;

				case 'v':
					c = '\t';
					break;

/*				case '0': {
					int d;

					d = getChar();

					if (!isdigit(d))
						parseError("Illegally formatted octal constant\n");

					d = d - '0';

					if (d > 7)
						parseError("Illegally formatted octal constant\n");

					c = d;
					d = getChar();

					if (!isdigit(d))
						parseError("Illegally formatted octal constant\n");

					d = d - '0';

					if (d > 7)
						parseError("Illegally formatted octal constant\n");

					c = (c * 8) + d;
					d = getChar();

					if (isdigit(d)) {
						d = d - '0';

						if (d > 7)
							parseError("Illegally formatted octal constant\n");

						c = (c * 8) + d;
					}
					else
						ungetChar();

					break;
				}   
*/
			}
		}

		buf[i++] = c;
	}

	buf[i] = 0;
	lexData.type = T_STRING;
	lexData.stringData = buf;
}

void continueLex(InputStream *s) {
   stream = s;
}

void startLex(InputStream *s) {
   stream = s;
   if (s->type == INPUT_STRING) {
      s->s.curpos = s->s.string;
      s->s.readEOF = 0;
   }
   s->lineno = 1;
}

int lexGetLineno(InputStream *s) {
   return s ? s->lineno : -1;
}

/*
 * read a symbol from the input stream
 */
int lex(void) {
	int c, i;
	static char buf[2][1024];
	static int which;
	int ret;


	if (ungotToken != -1) {
		ret = ungotToken;
		ungotToken = -1;
		return ret;
	}

/* read past spaces */
top:
	c = getChar();

	switch(c) {
		case EOF:
			ret = T_EOF;
			break;

		case '/': {
			int d;

			d = getChar();

			if (d == '*') {
				while(1) {
					d = getChar();

					while(d != '*') {   // got slash-star, now look for star-slash
						if (d == EOF) {
							ungetChar();
							goto top;
						}

						d = getChar();
					}

					d = getChar();   // found star, is next char a slash?

					if (d == '/')    // yup, start parsing over
						goto top;
					else if (d == '*')
						ungetChar();

					// else just continue
				}
			}
			else if (d != '/') {
				if (d == '=')
					ret = T_ASSIGN_DIV;
				else {
					ungetChar();
					ret = '/';
				}

				break;
			}
			/*FALLTHRU*/
		}

		case '#': {  // parse compiler directives
			do {
				c = getChar();
			} while(c != EOF && c != '\n' && !validSymbolChar(c));

			if (validSymbolChar(c)) {
				int i = 0, j;

				buf[which][0] = c;

				while(1) {
					c = getChar();

					if (!validSymbolChar(c))
						break;

					buf[which][i+1] = c;
					i++;
				}

				buf[which][i+1] = 0;

				if (stricmp(buf[which], "line") == 0) {// line directive
					do {
						c = getChar();
					} while(c != EOF && c != '\n' && !isdigit(c));


					if (c == EOF) {
						ungetChar();
					}
					else if (c != '\n') {
						ungetChar();
						parseNumber();
						j = lexData.intData;

						do {
							c = getChar();
						} while(c != EOF && c != '\n' && c != '"');

						if (c == '"') {
							stream->lineno = j - 1;
							parseString(buf[which]);

							if (strlen(buf[which]) >= sizeof(stream->name)) {
								char* d = buf[which] + strlen(buf[which]) - sizeof(stream->name) + 1;
								strcpy(stream->name, d);
							}
							else
								strcpy(stream->name, buf[which]);
						}
						else if (c == EOF)
							ungetChar();
					}
				}
				else if (c == EOF) {
					ungetChar();
				}
				else if (c != '\n') {
					do {
						c = getChar();
					} while(c != '\n' && c != EOF) ;

					if (c == EOF)
						ungetChar();
				}
			}
			else if (c == EOF) {
				ungetChar();
			}
			else if (c != '\n') {
				do {
					c = getChar();
				} while(c != '\n' && c != EOF) ;

				if (c == EOF)
					ungetChar();
			}
		}

		case '\t':
		case ' ':
		case '\n':
		case '\v':
			goto top;
			break;

		case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9': case '0':
		case '.':
			ungetChar();
			parseNumber();
			ret = T_CONSTANT;
			break;

		case '\'':
			ungetChar();
			parseCharConstant();
			ret = T_CONSTANT;
			break;

		case '"':
			parseString(buf[which]);
			ret = T_CONSTANT;
			break;

#define EXPECT(x, y, z) case x: ret = expect(y, z, x); break;
		EXPECT(':', '=', T_ASSIGN);
		EXPECT('=', '=', T_EQUAL);
		EXPECT('>', '=', T_GREATER_EQUAL);
		EXPECT('<', '=', T_LESS_EQUAL);
		EXPECT('!', '=', T_NOT_EQUAL);

		EXPECT('+', '=', T_ASSIGN_ADD);
		EXPECT('-', '=', T_ASSIGN_SUB);
		EXPECT('*', '=', T_ASSIGN_MUL);

		case '{': case '}':
		case '(': case ')':
		case ';': case ',': case '%':
			ret = c;
			break;

		default:
			i = 0;

			do {
				buf[which][i++] = c;
				c = getChar();
			} while(validSymbolChar(c));

			ungetChar();
			buf[which][i] = 0;
			ret = lookupToken(buf[which]);

			if (ret == -1) {
				ret = lookupConstant(buf[which]);

				if (ret == -1) {
					lexData.stringData = buf[which];
					ret = T_SYMBOL;
				}
				else {
					lexData.type = constants[ret].c.type;

					switch(lexData.type) {
						case T_INT:
							lexData.intData = constants[ret].c.intData;
							break;

						case T_FLOAT:
							lexData.floatData = constants[ret].c.floatData;
							break;

						case T_STRING:
							lexData.stringData = constants[ret].c.stringData;
							break;
					}

					ret = T_CONSTANT;
				}
			}

			break;
	}

	which = !which;
	lexData.token = ret;
	return lastToken = ret;
}

void ungetToken(void) { ungotToken = lastToken; }
