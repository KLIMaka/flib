#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <setjmp.h>
#include "memdbg.h"

#include "parse.h"
#include "parselib.h"
#include "extra.h"

#include "opcodes.h"
#include "oplib.h"
#include "opextra.h"

#include "lex.h"

int	compilerErrorTotal;

/*
 * Parser for SSL (Startrek Scripting Language).
 *
 * All variables are "typeless"; their type is defined by how
 * they are used.  All types are promoted to the "highest" type
 * in an expression, where the order is from lowest to highest:
 * int, float, string.
 *
 * So,
 * if you do 2 + "foo", you get back "2foo".
 * if you do 2 + 2.4,   you get 4.4
 * if you do 2 + 4,     you get 6 (hopefully :)
 * if you do 2 + 2.4 + "foo", you get "4.4foo", since
 * expressions are parsed left to right.
 * if you do 2 + (2.4 + "foo") you get "22.4foo", due
 * to the parentheses.
 * etc.
 */
static Program *currentProgram;
static Procedure *currentProcedure;
static InputStream *currentInputStream;
static int procedureStart;
static int startOffset;
static InputStream *includes[32];
static int tois = -1;

extern int outputStr(char *);
extern int warnings;
static int (*outputFunc)(char *) = outputStr;
static void freeVariableList(VariableList *v);
static void freeVariable(Variable *v);
static void parseWhile(Procedure *p, NodeList *n);
static int writeBlock(NodeList *n, int i, OutputStream *f);
int variable(VariableList *v, char **names, int type);

void parseOutputFunc(int (*o)(char *)) { outputFunc = o; }

static int outputStr(char *s) {
   fprintf(stderr, "%s", s);
   return 1;
}

int parseOutput(const char *format, ...) {
   char buf[256];
   int i;
   va_list arg;

   va_start(arg, format);
   i = vsprintf(buf, format, arg);
   va_end(arg);
   outputFunc(buf);
   return i;
}

void parseError(char *format, ...) {
   char buf[256];
   int i;
   va_list arg;

   va_start(arg, format);
   i = vsprintf(buf, format, arg);
   if (currentInputStream->lineno == -1)
      parseOutput("Error during code generation: %s\n", buf);
   else
      parseOutput("%s(%d): Error! %s\n", lexGetFilename(currentInputStream),
             lexGetLineno(currentInputStream), buf);
	va_end(arg);

	compilerErrorTotal++;

   longjmp(currentProgram->env, 1);
}

void parseErrorLine(int line, char *format, ...) {
   char buf[256];
   int i;
   va_list arg;

   va_start(arg, format);
   i = vsprintf(buf, format, arg);
   parseOutput("Error, line %d: %s\n", line, buf);
	va_end(arg);

	compilerErrorTotal++;

   longjmp(currentProgram->env, 1);
}

static void freeVariableList(VariableList *v) {
   int i;

   if (v->variables) {
      for (i=0; i<v->numVariables; ++i)
        freeVariable(v->variables + i);
      free(v->variables);
   }
}

static void freeProcedure(Procedure *p) {
   if (p->namespace) free(p->namespace);
   freeVariableList(&p->variables);
}

static void freeProcedureList(ProcedureList *p) {
   int i;

   if (p->procedures) {
      for (i=0; i<p->numProcedures; ++i)
         freeProcedure(p->procedures + i);
      free(p->procedures);
   }
}

int expectToken(int expectToken) {
   int token;
   token = lex();
   if (token != expectToken) {
      ungetToken();
      return -1;
   }
   return token;
}

static int findName(char *namespace, char *name) {
   char *n;

   if (!namespace) return -1;

   n = namespace + 4;
   while(*(unsigned short *)n != 0xffff) {
      if (stricmp(n+2, name) == 0)
         return n+2 - namespace;
      n += *(unsigned short *)n + 2;
   }
   return -1;
}

static int findString(char *namespace, char *name) {
   char *n;

   if (!namespace) return -1;

   n = namespace + 4;
   while(*(unsigned short *)n != 0xffff) {
      if (strcmp(n+2, name) == 0)
         return n+2 - namespace;
      n += *(unsigned short *)n + 2;
   }
   return -1;
}

static char *getName(int offset, char *namespace) {
   return namespace + offset;
}

/*
 * Add a string to a namespace list.
 * namespaces are defined by <4-byte total len><string><string> ...
 * where a <string> is <2-byte len><string data><one or 2 zero>.
 * lengths do not include the headers; the total len *does*
 * include the headers for each particular string.
 * A length field of 0xffff indicates end of the namespace.
 *
 * To get the string for any name (be that the name of a variable,
 * or a string in a string variable, since I store both as 'names'),
 * you have to have the namespace defining that name.
 */
static int addName(char **namespace, char *name) {
   unsigned short slen = strlen(name);
   long tlen;
   char *n = *namespace;
   char *c;
   int odd = 0;

   slen++;  /* increment one for the null */
   if (slen&1) {/* odd? */
      slen++;
      odd = 1;
   }

   if (!n) {
      n = malloc(4 + 2 + slen + 2);
      *(long *)n = 2 + slen;
      c = n+4;
   }
   else {
      int i;

      tlen = *(long *)n;
      i = findName(n, name);
      if (i != -1)
         return i;

      /* 4 for main header
         tlen for old total length
         2 for this string's header
         slen for this string's length
         2 for ending length
       */
      n = realloc(n, 4 + tlen + 2 + slen + 2);
      *(long *)n = tlen + 2 + slen;
      c = n + 4 + tlen;
   }
   *(unsigned short *)c = slen;
   *(unsigned short *)(c + 2 + slen) = 0xffff;
   *namespace = n;
   strcpy(c+2, name);
   if (odd) *(c+2+slen-1) = 0;
   return c+2 - n;
}

static int addString(char **namespace, char *name) {
   unsigned short slen = strlen(name);
   long tlen;
   char *n = *namespace;
   char *c;
   int odd = 0;

   slen++;  /* increment one for the null */
   if (slen&1) {/* odd? */
      slen++;
      odd = 1;
   }

   if (!n) {
      n = malloc(4 + 2 + slen + 2);
      *(long *)n = 2 + slen;
      c = n+4;
   }
   else {
      int i;

      tlen = *(long *)n;
      i = findString(n, name);
      if (i != -1)
         return i;

      /* 4 for main header
         tlen for old total length
         2 for this string's header
         slen for this string's length
         2 for ending length
       */
      n = realloc(n, 4 + tlen + 2 + slen + 2);
      *(long *)n = tlen + 2 + slen;
      c = n + 4 + tlen;
   }
   *(unsigned short *)c = slen;
   *(unsigned short *)(c + 2 + slen) = 0xffff;
   *namespace = n;
   strcpy(c+2, name);
   if (odd) *(c+2+slen-1) = 0;
   return c+2 - n;
}

static void assignValue(Value *v, LexData *what) {
   switch(what->type) {
      case T_INT:
         v->type = V_INT;
         v->intData = what->intData;
         break;
      case T_FLOAT:
         v->type = V_FLOAT;
         v->floatData = what->floatData;
         break;
      case T_STRING:
         v->type = V_STRING;
         v->stringData = addString(&currentProgram->stringspace, what->stringData);
         break;
   }
}

static void assignVariable(VariableList *v, int which, LexData *what) {
   assignValue(&v->variables[which].value, what);
}

static void referenceVariable(VariableList *v, int which) {
v = v; which = which;
//   v->variables[which].uses++;
}

static void referenceProcedure(ProcedureList *p, int which) {
   p->procedures[which].uses++;
}

static int addVariable(VariableList *var, char **namespace, int type, char *name) {
   Variable *v = var->variables;
   int i;

   if (!v) {
      v = malloc(sizeof(Variable));
      i = 0;
   }
   else {
      for (i=0; i < var->numVariables; ++i)
         if (stricmp(getName(v[i].name, *namespace), name) == 0)
            break;

      if (i == var->numVariables)
         v = realloc(v, sizeof(Variable) * (i+1));
      else {
         parseOutput("Warning, redefinition of variable %s\n", name);
         return i;
      }
   }

   v[i].name = addName(namespace, name);
   v[i].type = type;
   v[i].value.type = V_INT;
   v[i].value.intData = 0;
   v[i].uses = 0;
   var->variables = v;
   var->numVariables++;
   return i;
}

static int defineVariable(VariableList *v, char **namespace, int type) {
   int i;
   if (expectToken(T_SYMBOL) == -1)
      parseError("Expecting symbol.");

   if (findName(*namespace, lexData.stringData) != -1)
      parseError("Redefinition of %s.", lexData.stringData);

   i = addVariable(v, namespace, type, lexData.stringData);

   if (i == -1)
      parseError("Couldn't add variable %s.", lexData.stringData);

   if (expectToken(T_ASSIGN) != -1) {
      if (expectToken(T_CONSTANT) == -1) {
         if (expectToken('-') == -1)
            parseError("Initialization of %s with non-constant.", lexData.stringData);
         if (expectToken(T_CONSTANT) == -1)
            parseError("Initialization of %s with non-constant.", lexData.stringData);
         switch(lexData.type) {
            case T_INT:
               lexData.intData = -lexData.intData;
               break;
            case T_FLOAT:
               lexData.floatData = -lexData.floatData;
               break;
         }
      }
      assignVariable(v, i, &lexData);
   }
   if (expectToken(';') == -1)
      ;
   return 0;
}

static Procedure *findProcedure(ProcedureList *procs, char **namespace, char *name) {
   int i;

   if (!*namespace || !procs || !procs->procedures) return 0;

   for (i=0; i<procs->numProcedures; ++i) {
      if (stricmp(getName(procs->procedures[i].name, *namespace), name) == 0)
         return procs->procedures + i;
   }

   return 0;
}

static int findProcedureIndex(ProcedureList *procs, char **namespace, char *name) {
   int i;

   if (!*namespace || !procs || !procs->procedures) return 0;

   for (i=0; i<procs->numProcedures; ++i) {
      if (stricmp(getName(procs->procedures[i].name, *namespace), name) == 0)
         return i;
   }

   return -1;
}

static Procedure *addProcedure(ProcedureList *procs, char **namespace, char *name) {
   int i;
   Procedure *p;

   if (!procs->procedures) {
      procs->procedures = malloc(sizeof(Procedure));
      procs->numProcedures = 1;
      i = 0;
   }
   else if ((p = findProcedure(procs, namespace, name)) != 0)
      return p;
   else {
      i = procs->numProcedures++;
      procs->procedures = realloc(procs->procedures, sizeof(Procedure)*(i+1));
   }

   procs->procedures[i].name = addName(namespace, name);
   procs->procedures[i].type = 0;
   procs->procedures[i].namespace = 0;
   procs->procedures[i].numArgs = 0;
   procs->procedures[i].uses = 0;
   procs->procedures[i].defined = 0;
   procs->procedures[i].variables.variables = 0;
   procs->procedures[i].variables.numVariables = 0;
   procs->procedures[i].nodes.numNodes = 0;
   procs->procedures[i].nodes.nodes = 0;

   procs->procedures[i].condition.numNodes = 0;
   procs->procedures[i].condition.nodes = 0;

   return procs->procedures + i;
}

static int externProcedure(ProcedureList *pl, char **namespace, int type) {
   Procedure *p;
   int numArgs = 0;
   char *argNames = 0;
   VariableList args;

   args.numVariables = 0;
   args.variables = 0;

   if (expectToken(T_SYMBOL) == -1)
      return 1;

   p = findProcedure(pl, namespace, lexData.stringData);

   if (p) {
      if (p->defined == 2)
         parseError("Redefinition of procedure %s.", lexData.stringData);
   }
   else {
      if (findName(*namespace, lexData.stringData) != -1)
         parseError("%s already defined.", lexData.stringData);

      p = addProcedure(pl, namespace, lexData.stringData);
   }

   if (expectToken('(') != -1) {
      int i;
      i = lex();
      while(i != ')') {
         if (i != T_VARIABLE)
            parseError("'variable' expected.");
         ungetToken();
         variable(&args, &argNames, V_LOCAL);
         i = lex();
         if (i != ')') {
            if (i != ',')
               parseError("',' expected.");
            i = lex();
         }
         numArgs++;
      }
   }

   if (expectToken(';') == -1)
      parseError("Can't define procedures in an import or export block");

   p->numArgs = numArgs;
   p->defined = 1;
   p->type |= type;

   freeVariableList(&args);
   if (argNames) free(argNames);

   return 0;
}

static int externVariable(VariableList *v, char **namespace, int type, int flag) {
   int i;

   if (expectToken(T_SYMBOL) == -1)
      parseError("Expecting symbol.");

   if (findName(*namespace, lexData.stringData) != -1)
      parseError("Redefinition of %s.", lexData.stringData);

   i = addVariable(v, namespace, type, lexData.stringData);

   if (i == -1)
      parseError("Couldn't add variable %s.", lexData.stringData);

   if (expectToken(T_ASSIGN) != -1) {
      if (flag) {
         if (expectToken(T_CONSTANT) == -1)
            parseError("Initialization of %s with non-constant.", lexData.stringData);
         assignVariable(v, i, &lexData);
      }
      else parseError("Can't assign in initialization of imported variables.");
   }
   if (expectToken(';') == -1)
      ;
   return 0;
}

static int findVariableIndex(char *var, VariableList *v, char *namespace) {
   int i;

   for (i=0; i<v->numVariables; ++i) {
      if (stricmp(getName(v->variables[i].name, namespace), var) == 0)
         return i;
   }
   return -1;
}

int import(Program *p, char **names) {
   if (expectToken(T_IMPORT) == -1) return 1;
   do {
      if (expectToken(T_VARIABLE) != -1) {
         if (expectToken(T_BEGIN) != -1) {
            while(expectToken(T_END) == -1)
               if (externVariable(&p->externals, names, V_IMPORT, 0))
                  return 1;
         }
         else if (expectToken(T_SYMBOL) != -1) {
            ungetToken();
            if (externVariable(&p->externals, names, V_IMPORT, 0))
               return 1;
         }
         else parseError("Expected symbol or 'begin' block");
      }
      else if (expectToken(T_PROCEDURE) != -1) {
         if (expectToken(T_BEGIN) != -1) {
            while(expectToken(T_END) == -1) {
               if (externProcedure(&p->procedures, names, P_IMPORT))
                  return 1;
            }
         }
         else if (expectToken(T_SYMBOL) != -1) {
            ungetToken();
            if (externProcedure(&p->procedures, names, P_IMPORT))
               return 1;
         }
         else parseError("Expected symbol or 'begin' block");
      }
      else parseError("Expected 'procedure' or 'variable'");
   } while(expectToken(T_IMPORT) != -1);

   ungetToken();
   return 0;
}

int export(Program *p, char **names) {
   if (expectToken(T_EXPORT) == -1) return 1;
   do {
      if (expectToken(T_VARIABLE) != -1) {
         if (expectToken(T_BEGIN) != -1) {
            while(expectToken(T_END) == -1)
               if (externVariable(&p->externals, names, V_EXPORT, 1))
                  return 1;
         }
         else if (expectToken(T_SYMBOL) != -1) {
            ungetToken();
            if (externVariable(&p->externals, names, V_EXPORT, 1))
               return 1;
         }
         else parseError("Expected symbol or 'begin' block");
      }
      else if (expectToken(T_PROCEDURE) != -1) {
         if (expectToken(T_BEGIN) != -1) {
            while(expectToken(T_END) == -1) {
               if (externProcedure(&p->procedures, names, P_EXPORT))
                  return 1;
            }
         }
         else if (expectToken(T_SYMBOL) != -1) {
            ungetToken();
            if (externProcedure(&p->procedures, names, P_EXPORT))
               return 1;
         }
         else parseError("Expected symbol or 'begin' block");
      }
      else parseError("Expected 'procedure' or 'variable'");
   } while(expectToken(T_EXPORT) != -1);

   ungetToken();
   return 0;
}

static void copyVariables(VariableList *to, char **toNames, VariableList *from, char *fromNames) {
   int i;

   for (i=0; i<from->numVariables; ++i) {
      addVariable(to, toNames, from->variables[i].value.type,
                  getName(from->variables[i].name, fromNames));
   }
}

int variable(VariableList *v, char **names, int type) {
   if (expectToken(T_VARIABLE) == -1) return 1;

   do {
      if (expectToken(T_BEGIN) != -1) {
         while(expectToken(T_END) == -1) {
            if (defineVariable(v, names, type))
               return 1;
         }
      }
      else if (expectToken(T_SYMBOL) != -1) {
         ungetToken();
         if (defineVariable(v, names, type))
            return 1;
      }
      else parseError("Expected symbol or 'begin' block");
   }  while(expectToken(T_VARIABLE) != -1);

   ungetToken();  // put back what was there
   return 0;
}

/*
 * A node is just an instruction.
 * the token field is set to the token returned from the lexical
 * analyzer.  If it's a constant, the value's type is set to the
 * type of variable it is.  If it's a symbol, the type field is
 * set to the type of symbol (global or local, variable or procedure)
 * and the intData field is set to the offset for that variable
 * or procedure.
 */
void emitNode(Procedure *p, NodeList *n, LexData *data) {
   int i;

   if (!n->nodes) {
      n->nodes = malloc(sizeof(Node));
      n->numNodes = 0;
   }
   else
      n->nodes = realloc(n->nodes, sizeof(Node) * (n->numNodes+1));

   i = n->numNodes++;

   n->nodes[i].token = data->token;
   n->nodes[i].lineNum = lexGetLineno(currentInputStream);

   switch(data->token) {
      case T_CONSTANT:
         assignValue(&n->nodes[i].value, data);
         break;
      case T_SYMBOL: {
         int v;
         int type = 0;

//
// search order is:
//  local variables
//  global procedures
//  global variables
//  external variables
//
         v = findVariableIndex(data->stringData, &p->variables, p->namespace);
         type = P_LOCAL;
         if (v == -1) {
            v = findProcedureIndex(&currentProgram->procedures, &currentProgram->namespace, data->stringData);
            if (v == -1) {
               v = findVariableIndex(data->stringData, &currentProgram->variables, currentProgram->namespace);
               type = P_GLOBAL;
               if (v == -1) {
                  v = findVariableIndex(data->stringData, &currentProgram->externals, currentProgram->namespace);
                  if (v == -1)
                     parseError("Undefined symbol %s.", data->stringData);
                  v = findName(currentProgram->namespace, data->stringData);
                  type = P_EXTERN;
                  referenceVariable(&currentProgram->externals, v);
               }
               else referenceVariable(&currentProgram->variables, v);
            }
            else {
               type |= P_PROCEDURE;
               referenceProcedure(&currentProgram->procedures, v);
            }
         }
         else referenceVariable(&p->variables, v);

         n->nodes[i].value.type    = type;
         n->nodes[i].value.intData = v;
         break;
      }
   }
}

void emitInt(Procedure *p, NodeList *nodes, int i) {
   LexData tmp;
   tmp.token = T_CONSTANT;
   tmp.type = T_INT;
   tmp.intData = i;
   emitNode(p, nodes, &tmp);
}

void emitOp(Procedure *p, NodeList *nodes, int token) {
   LexData tmp;
   tmp.token = token;
   emitNode(p, nodes, &tmp);
}

static void logical_expression(Procedure *p, NodeList *nodes);
static void parseFuncArgs(Procedure *p, NodeList *nodes, Procedure *q);

static void factor(Procedure *p, NodeList *nodes) {
   int i;

   i = lex();

	switch(i) {
		case T_FLOOR:
		case T_NOT:
		case T_BWNOT:
      case '-':
         factor(p, nodes);
         if (i == '-') i = T_NEGATE;
         emitOp(p, nodes, i);
         break;

      case T_SYMBOL: {
// local?
         LexData d = lexData;

         if (findVariableIndex(lexData.stringData, &p->variables, p->namespace) != -1) {
            if (expectToken('(') != -1) { // function call
               ungetToken();
               emitOp(p, nodes, T_CALL_FUNC);
               emitNode(p, nodes, &d);
               emitOp(p, nodes, T_START_EVENT);
               parseFuncArgs(p, nodes, 0);
               emitOp(p, nodes, T_END_EVENT);
            }
            else
               emitNode(p, nodes, &d);
         }
         else {
            Procedure *q;
            q = findProcedure(&currentProgram->procedures, &currentProgram->namespace, lexData.stringData);
            if (q) {
               emitOp(p, nodes, T_CALL_FUNC);
               emitNode(p, nodes, &d);
               emitOp(p, nodes, T_START_EVENT);
               parseFuncArgs(p, nodes, q);
               emitOp(p, nodes, T_END_EVENT);
            }
            else if (findVariableIndex(lexData.stringData, &currentProgram->variables, currentProgram->namespace) != -1) {
               if (expectToken('(') != -1) { // function call
                  ungetToken();
                  emitOp(p, nodes, T_CALL_FUNC);
                  emitNode(p, nodes, &d);
                  emitOp(p, nodes, T_START_EVENT);
                  parseFuncArgs(p, nodes, 0);
                  emitOp(p, nodes, T_END_EVENT);
               }
               else
                  emitNode(p, nodes, &d);
            }
            else if (findVariableIndex(lexData.stringData, &currentProgram->externals, currentProgram->namespace) != -1) {
               if (expectToken('(') != -1) { // function call
                  ungetToken();
                  emitOp(p, nodes, T_CALL_FUNC);
                  emitNode(p, nodes, &d);
                  emitOp(p, nodes, T_START_EVENT);
                  parseFuncArgs(p, nodes, 0);
                  emitOp(p, nodes, T_END_EVENT);
               }
               else
                  emitNode(p, nodes, &d);
            }
            else
               parseError("Undefined symbol %s\n", lexData.stringData);
         }
         break;
      }
      case T_CONSTANT: {
         if (lexData.type == T_STRING) { // could be a function call
            LexData d = lexData;
            if (expectToken('(') != -1) {
               ungetToken();
               emitOp(p, nodes, T_CALL_FUNC);
               emitNode(p, nodes, &d);
               emitOp(p, nodes, T_START_EVENT);
               parseFuncArgs(p, nodes, 0);
               emitOp(p, nodes, T_END_EVENT);
            }
            else
               emitNode(p, nodes, &d);
         }
         else
            emitNode(p, nodes, &lexData);
         break;
      }
      case '(':
         logical_expression(p, nodes);
         i = lex();
         if (i != ')') {
            parseError("Mismatched parenthesis.");
            ungetToken();
         }
         break;
		default:
         parseLibExpression(p, i);
         break;
   }
}

static void term_prime(Procedure *p, NodeList *nodes) {
   int i = lex();

   if (i == '*' || i == '/' || i == '%') {
//      term(p, nodes);
      factor(p, nodes);
      emitOp(p, nodes, i);
      term_prime(p, nodes);
   }
   else ungetToken();
}

static void term(Procedure *p, NodeList *nodes) {
	factor(p, nodes);
   term_prime(p, nodes);
}

static void expr_prime(Procedure *p, NodeList *nodes) {
	int i = lex();

   if (i == '+' || i == '-' || i == T_BWAND  || i == T_BWOR || i == T_BWXOR) {
      term(p, nodes);
      emitOp(p, nodes, i);
      expr_prime(p, nodes);
   }
   else
      ungetToken();
}

static void expression(Procedure *p, NodeList *nodes) {
	term(p, nodes);
	expr_prime(p, nodes);
}

static void compare_prime(Procedure *p, NodeList *nodes) {
	int i = lex();

	switch(i) {
		case '>':
		case '<':
		case T_GREATER_EQUAL:
		case T_LESS_EQUAL:
		case T_EQUAL:
		case T_NOT_EQUAL: break;
		default: ungetToken(); return;
	}
	expression(p, nodes);
	emitOp(p, nodes, i);
}

static void compare_expression(Procedure *p, NodeList *nodes) {
	expression(p, nodes);
   compare_prime(p, nodes);
}

static void logical_prime(Procedure *p, NodeList *nodes) {
   int i;

   i = lex();
	if (i == T_AND || i == T_OR) {
      compare_expression(p, nodes);
      emitOp(p, nodes, i);
      logical_prime(p, nodes);
   }
   else ungetToken();
}

static void logical_expression(Procedure *p, NodeList *nodes) {
   compare_expression(p, nodes);
   logical_prime(p, nodes);
}

void parseExpression(Procedure *p, NodeList *nodes) {
   emitOp(p, nodes, T_START_EXPRESSION);

   logical_expression(p, nodes);

   emitOp(p, nodes, T_END_EXPRESSION);
}

static int insideLibFuncArg = 0;

void parseLibArgs(Procedure *p, int n) {
   if (n) {
      if (expectToken('(') == -1) parseError("'(' expected");
      insideLibFuncArg++;
      parseExpGroup(p, &p->nodes, n);
      insideLibFuncArg--;
      if (expectToken(')') == -1) parseError("')' expected");
   }
}

static void parseFuncArgs(Procedure *p, NodeList *nodes, Procedure *q) {
   int i;
   int args = 0;

   i = lex();
   if (i == '(') {
      emitOp(p, nodes, T_START_ARG);
      i = lex();
      if (i != ')') {
         ungetToken();
         do {
            parseExpression(p, nodes);
            args++;
            if (expectToken(')') != -1)
               break;

            if (expectToken(',') == -1)
               parseError("',' expected.");
         } while(1);
      }

      emitOp(p, nodes, T_END_ARG);
   }
   else ungetToken();

   if (q) {
      if (insideLibFuncArg == 0 && q->numArgs != -1 && args != q->numArgs)
         parseError("parseFuncArgs: Wrong number of arguments to procedure %s.",
                    getName(q->name, currentProgram->namespace));
   }
   else
      emitOp(p, nodes, T_CHECK_ARG_COUNT);
}

static void parseEvent(Procedure *p, NodeList *nodes) {
   int i;
   Procedure *other;
   int calloffset = nodes->numNodes;

   if (expectToken(T_SYMBOL) == -1) {
      if (expectToken(T_CONSTANT) != -1 && lexData.type != T_STRING) {
         parseError("Symbol or string expected.");
         return;
      }
   }

   emitNode(p, nodes, &lexData);

   if (lexData.token == T_SYMBOL) {
      other = findProcedure(&currentProgram->procedures, &currentProgram->namespace, lexData.stringData);
      if (!other) { // not a procedure, see if it's a variable
         int v;
         v = findVariableIndex(lexData.stringData, &p->variables, p->namespace);
         if (v == -1) {
            v = findVariableIndex(lexData.stringData, &currentProgram->variables, currentProgram->namespace);
            if (v == -1)
               v = findVariableIndex(lexData.stringData, &currentProgram->externals, currentProgram->namespace);
               if (v == -1)
                  parseError("Undefined procedure %s.", lexData.stringData);
         }
      }
   }
   else
      other = 0;

   emitOp(p, nodes, T_START_EVENT);

   parseFuncArgs(p, nodes, other);

   i = lex();

   switch(i) {
      case T_IN:
      case T_WHEN:
         if (i == T_IN)
            nodes->nodes[calloffset-1].token = T_CALL_AT;
         else
            nodes->nodes[calloffset-1].token = T_CALL_CONDITION;
         emitNode(p, nodes, &lexData);
         parseExpression(p, nodes);
         break;
      default: ungetToken(); break;
   }
   emitOp(p, nodes, T_END_EVENT);
}

void parseIf(Procedure *p, NodeList *nodes);
void parseBlock(Procedure *p);
void parseExpGroup(Procedure *p, NodeList *nodes, int num) {
   if (num > 0) {
      parseExpression(p, nodes);
      num--;
   }
   while(num-- > 0) {
      if (expectToken(',') == -1)
         parseError("',' expected.");
      parseExpression(p, nodes);
   }
}

// Parse a statement from the input stream.

static void parseStatement(Procedure *p) {
   int i;

   i = lex();
   emitOp(p, &p->nodes, T_START_STATEMENT);
   if (i == T_BEGIN) {
      ungetToken();
      parseBlock(p);
   }
   else {
      emitNode(p, &p->nodes, &lexData);
      switch(i) {
         case T_IF:
            parseIf(p, &p->nodes);
            break;
         case T_WHILE:
            parseWhile(p, &p->nodes);
            break;
         default: {
            switch(i) {
               default:
                  parseLib(p, i);
                  break;

               PARSE(DETACH, 0);
               PARSE(EXIT, 0);
               PARSE(SPAWN, 1);
               PARSE(CALLSTART, 1);
               PARSE(EXEC, 1);
               PARSE(FORK, 1);
               PARSE(WAIT, 1);
               PARSE(STARTCRITICAL, 0);
               PARSE(ENDCRITICAL, 0);

               case T_NOOP: break;

               case T_SYMBOL: {
                  int t;

                  t = lex();
                  switch(t) {
                     case T_ASSIGN:
                     case T_ASSIGN_ADD:
                     case T_ASSIGN_SUB:
                     case T_ASSIGN_DIV:
                        emitNode(p, &p->nodes, &lexData);
                        parseExpression(p, &p->nodes);
                        break;
                     case T_ASSIGN_MUL:
                        emitNode(p, &p->nodes, &lexData);
                        parseExpression(p, &p->nodes);
                        break;
                     default:
                        parseError("Assignment operator expected.");
                  }
                  break;
               }
               case T_CALL: parseEvent(p, &p->nodes); break;

               case T_CANCEL:
                  if (expectToken('(') == -1)
                     parseError("'(' expected");
                  if (expectToken(T_SYMBOL) == -1)
                     parseError("Symbol expected");
                  emitNode(p, &p->nodes, &lexData);
                  if (expectToken(')') == -1)
                     parseError("')' expected");
                  break;
               case T_CANCELALL:
                  emitNode(p, &p->nodes, &lexData);
                  break;
               case T_RETURN:
                  if (expectToken(';') == -1)
                     parseExpression(p, &p->nodes);
                  else {
                     ungetToken();
                     emitOp(p, &p->nodes, T_START_EXPRESSION);
                     emitInt(p, &p->nodes, 0);
                     emitOp(p, &p->nodes, T_END_EXPRESSION);
                  }
                  break;
            }
            if (expectToken(';') == -1)
               parseError("expecting ';'.");
            break;
         }
      }
   }
   emitOp(p, &p->nodes, T_END_STATEMENT);
}

static void parseBlock(Procedure *p) {
   int i;

   if (expectToken(T_BEGIN) == -1)
      parseError("expected 'begin'.");

   emitNode(p, &p->nodes, &lexData);  // emit the begin

   variable(&p->variables, &p->namespace, V_LOCAL);

   while((i = lex()) != T_END) {
      if (i == T_EOF)
         parseError("Premature EOF encountered.");
      ungetToken();
      parseStatement(p);
   }
   emitNode(p, &p->nodes, &lexData);  // emit the end
}

static void parseIf(Procedure *p, NodeList *n) {
   parseExpression(p, n);
   if (expectToken(T_THEN) == -1) {
      parseError("Expected 'then'.");
      return;
   }
   emitNode(p, n, &lexData);
   parseStatement(p);
   if (expectToken(T_ELSE) != -1) {
      emitNode(p, n, &lexData);
      parseStatement(p);
   }
}

static void parseWhile(Procedure *p, NodeList *n) {
   parseExpression(p, n);
   if (expectToken(T_DO) == -1) {
      parseError("Expected 'do'.");
      return;
   }
   emitNode(p, n, &lexData);
   parseStatement(p);
}


int procedure(void) {
   Procedure *p;
   int numArgs = 0;
   char *argNames = 0;
   VariableList args;
   int critical = 0;

   args.numVariables = 0;
   args.variables = 0;

   if (expectToken(T_CRITICAL) != -1)
      critical = 1;

   if (expectToken(T_PROCEDURE) == -1)
      return 1;
   if (expectToken(T_SYMBOL) == -1)
      return 1;

   p = findProcedure(&currentProgram->procedures, &currentProgram->namespace, lexData.stringData);
   if (p) {
      if (p->type & P_IMPORT)
         parseError("Can't define imported procedures");

      if (p->defined == 2)
         parseError("Redefinition of procedure %s.", lexData.stringData);
   }
   else {
      if (findName(currentProgram->namespace, lexData.stringData) != -1)
         parseError("%s already defined.", lexData.stringData);

      p = addProcedure(&currentProgram->procedures, &currentProgram->namespace, lexData.stringData);
   }

   if (critical)
      p->type |= P_CRITICAL;

   if (expectToken('(') != -1) {
      int i;
      i = lex();
      while(i != ')') {
         if (i != T_VARIABLE)
            parseError("'variable' expected.");
         ungetToken();
         variable(&args, &argNames, V_LOCAL);
         i = lex();
         if (i != ')') {
            if (i != ',')
               parseError("',' expected.");
            i = lex();
         }
         numArgs++;
      }
   }

   if (p->defined == 1 && p->numArgs != numArgs)
      parseError("Wrong number of arguments to procedure %s\n",
                 getName(p->name, p->namespace));

   if (expectToken(';') != -1) {
      freeVariableList(&args);
      if (argNames) free(argNames);
      p->defined = 1;
      p->numArgs = numArgs;
      return 0;
   }

   copyVariables(&p->variables, &p->namespace, &args, argNames);
   freeVariableList(&args);
   if (argNames) free(argNames);

   if (expectToken(T_IN) != -1) {
      if (expectToken(T_CONSTANT) == -1)
         parseError("Constant expected.");

      p->type |= P_TIMED;
      p->time = lexData.intData;
   }
   else if (expectToken(T_WHEN) != -1) {
      p->type |= P_CONDITIONAL;
      parseExpression(p, &p->condition);
   }

   emitOp(p, &p->nodes, T_BEGIN);
   parseBlock(p);

   emitOp(p, &p->nodes, T_START_STATEMENT);
   emitOp(p, &p->nodes, T_RETURN);
   emitOp(p, &p->nodes, T_START_EXPRESSION);
   emitInt(p, &p->nodes, 0);
   emitOp(p, &p->nodes, T_END_EXPRESSION);
   emitOp(p, &p->nodes, T_END_STATEMENT);
   emitOp(p, &p->nodes, T_END);

   p->defined = 2;
   p->numArgs = numArgs;

   return 0;
}

int top(void) {
   while(1) {
      if (expectToken(T_EOF) != -1) {
         if (tois != -1) {
            fclose(currentInputStream->file);
            free(currentInputStream);
            currentInputStream = includes[tois];
            includes[tois--] = 0;
            continueLex(currentInputStream);
         }
         else break;
      }
      if (variable(&currentProgram->variables, &currentProgram->namespace, V_GLOBAL) &&
          import(currentProgram, &currentProgram->namespace) &&
          export(currentProgram, &currentProgram->namespace) &&
          procedure()) {
         if (expectToken(T_INCLUDE) != -1) {
            InputStream *tmp;
            if (tois == sizeof(includes) / sizeof(includes[0]))
               parseError("Too many includes!");
            includes[++tois] = currentInputStream;
            tmp = malloc(sizeof(InputStream));
            tmp->type = INPUT_FILE;
            lex();
            if (lexData.type != T_STRING)
               parseError("Invalid type given to include");
            strcpy(tmp->name, lexData.stringData);
            tmp->file = fopen(lexData.stringData, "r");
            if (!tmp->file)
               parseError("Couldn't find include file %s\n", lexData.stringData);
            currentInputStream = tmp;
            startLex(tmp);
         }
         else {
            parseError("Expecting top-level statement");
            return 1;
         }
      }
   }
   return 0;
}

static void freeVariable(Variable *v) {
   v = v;
}

static void freeProgram(Program *s) {
   freeProcedureList(&s->procedures);
   freeVariableList(&s->variables);
   if (s->stringspace) free(s->stringspace);
   if (s->namespace) free(s->namespace);
   free(s);
}

void freeCurrentProgram(void) {
   if (currentProgram)
      freeProgram(currentProgram);
}

char *parse(InputStream *stream, char *output) {
   initLex();
   startLex(stream);
   currentProgram = malloc(sizeof(Program));
   currentProgram->procedures.procedures = 0;
   currentProgram->procedures.numProcedures = 0;
   currentProgram->variables.variables = 0;
   currentProgram->variables.numVariables = 0;
   currentProgram->externals.variables = 0;
   currentProgram->externals.numVariables = 0;
   currentProgram->stringspace   = 0;
   currentProgram->namespace     = 0;
   currentInputStream = stream;
// bogus procedure so we never have a zero procedure offset
   addProcedure(&currentProgram->procedures, &currentProgram->namespace, "..............");

   if (setjmp(currentProgram->env)) {
      freeProgram(currentProgram);
      currentProgram = 0;
      lexClose();
      remove( output);
      return 0;
   }

   if (top()) {
      freeProgram(currentProgram);
      currentProgram = 0;
      lexClose();
      remove( output);
      return 0;
   }

   lexClose();
   currentInputStream->lineno = -1;
   return generateCode(currentProgram, output);
}

static void outputClose(OutputStream *f) {
   if (f->type == OUTPUT_FILE) fclose(f->f);
}

static int outputSeek(OutputStream *f, int where, int mode) {
   int absoffset;

   if (f->type == OUTPUT_FILE) return fseek(f->f, where, mode);
   switch(mode) {
      case SEEK_SET: absoffset = where; break;
      case SEEK_CUR: absoffset = f->mem.len + where; break;
   }
   return f->mem.len = where;
}

static int outputTell(OutputStream *f) {
   if (f->type == OUTPUT_FILE)
      return ftell(f->f);
   else
      return f->mem.len;
}

static void writeByte(unsigned char a, OutputStream *f) {
   if (f->type == OUTPUT_FILE)
      fputc(a, f->f);
   else {
      if (f->mem.len == f->mem.maxlen)
         f->mem.buf = realloc(f->mem.buf, f->mem.maxlen*=2);
      f->mem.buf[f->mem.len++] = a;
   }
}

static void writeWord(unsigned short a, OutputStream *f) {
   writeByte((unsigned char)(a >> 8), f);
   writeByte((unsigned char)(a & 0x00ff), f);
}

static void writeLong(unsigned long a, OutputStream *f) {
   writeWord((unsigned short)(a >> 16), f);
   writeWord((unsigned short)(a & 0x0000ffff), f);
}

void writeInt(unsigned long a, OutputStream *f) {
   writeWord(O_INTOP, f);
   writeLong(a, f);
}

void writeFloat(float a, OutputStream *f) {
   writeWord(O_FLOATOP, f);
   writeLong(*(unsigned long *)&a, f);
}

void writeString(unsigned long a, OutputStream *f) {
   writeWord(O_STRINGOP, f);
   writeLong(a, f);
}

void writeOp(unsigned short op, OutputStream *f) {
   writeWord(op, f);
}

static void writeMemory(unsigned char *p, int len, OutputStream *f) {
   while(len--) writeByte(*p++, f);
}

static void writeNamespace(OutputStream *f, char *namespace) {
   if (namespace) {
      int len;
      writeLong(*(unsigned long *)namespace, f);
      namespace += 4;
      while((len = *(unsigned short *)namespace) != 0xffff) {
         writeWord((unsigned short)len, f);
         writeMemory(namespace + 2, len, f);
         namespace += len + 2;
      }
   }
   writeMemory("\xff\xff\xff\xff", 4, f);
}

static void writeProcAddress(NodeList *n, int i, OutputStream *f) {
   switch(n->nodes[i].token) {
      case T_SYMBOL:
         writeInt(n->nodes[i].value.intData, f);
         if (!(n->nodes[i].value.type & P_PROCEDURE)) {
            if (n->nodes[i].value.type & P_LOCAL) {
               writeOp(O_FETCH, f);
               writeOp(O_LOOKUP_STRING_PROC, f);
            }
            else if (n->nodes[i].value.type & P_GLOBAL) {
               writeOp(O_FETCH_GLOBAL, f);
               writeOp(O_LOOKUP_STRING_PROC, f);
            }
            else if (n->nodes[i].value.type & P_EXTERN) {
               writeOp(O_FETCH_EXTERNAL, f);
               writeOp(O_LOOKUP_STRING_PROC, f);
            }
         }
         break;
      case T_CONSTANT:
         if (n->nodes[i].value.type == V_STRING) {
            writeString(n->nodes[i].value.stringData, f);
            writeOp(O_LOOKUP_STRING_PROC, f);
         }
         break;
      default:
        parseError("Internal error, invalid function call address\n");
        break;
   }
}

/*
 * each element in the table is 24 bytes long:
 *    first long == index into namespace for procedure name
 *    second     == type of function (P_TIMED, P_CONDITIONAL, none)
 *    third      == time this proc should go off, if timed
 *    fourth     == ptr to conditional code, if conditional
 *    fifth      == ptr to procedure
 *    sixth      == number of args to procedure
 */
static void writeProcedureTable(OutputStream *f, ProcedureList *p) {
   int i;

   writeLong(p->numProcedures, f);    // write size of table in elements
   for (i=0; i < p->numProcedures; ++i) {
      writeLong(p->procedures[i].name, f);
      writeLong(p->procedures[i].type, f);
      if (p->procedures[i].type & P_TIMED)
         writeLong(p->procedures[i].time, f);
      else
         writeLong(0, f);

      writeLong(0, f);     // expression offset
      writeLong(0, f);     // position of this procedure
      writeLong(p->procedures[i].numArgs, f);
   }
}

static void patchOffset(int where, int with, OutputStream *f) {
   int here = outputTell(f);
   outputSeek(f, where, SEEK_SET);
   writeLong(with, f);
   outputSeek(f, here, SEEK_SET);
}

static void patchProcTableEntry(int where, int which, int elem, int with, OutputStream *f) {
// +4 for table size field
   patchOffset(4 + where + which*4*PROCTABLE_SIZE + elem*4, with, f);
}

static void writeVariable(Variable *v, char *namespace, OutputStream *f) {
namespace = namespace;
//   if (!v->uses && warnings)
//      parseOutput("Warning: variable %s not referenced\n", getName(v->name, namespace));

   switch(v->value.type) {
      case V_INT: writeInt(v->value.intData, f); break;
      case V_FLOAT: writeFloat(v->value.floatData, f); break;
      case V_STRING: writeString(v->value.stringData, f); break;
   }
}

static int writeCallFunc(NodeList *n, int i, OutputStream *f, int *numArgs) {
   int ret = outputTell(f), proc;
   i++;

   if (n->nodes[i].token != T_SYMBOL &&
       (n->nodes[i].token == T_CONSTANT && n->nodes[i].value.type != V_STRING))
      parseError("Internal error, symbol or string expected.");

   writeInt(0, f);       // push return address
   writeOp(O_D_TO_A, f);

   proc = i;

   i++;

   if (n->nodes[i].token != T_START_EVENT)
      parseError("Internal error, no starting event.");

   i++;

   if (n->nodes[i].token == T_START_ARG) {
      int args = 0;
      i++;
      while(n->nodes[i].token != T_END_ARG) {
         i = writeExpression(n, i, f);
         args++;
      }
      i++;
      writeInt(args, f);
      *numArgs = args;
   }
   else {
      *numArgs = 0;
      writeInt(0, f);
   }

   writeProcAddress(n, proc, f);

   if (n->nodes[i].token == T_CHECK_ARG_COUNT) {
      writeOp(O_DUP, f);      // dup the proc address
      writeInt(*numArgs, f);  // write number of args
      writeOp(O_CHECK_ARG_COUNT, f);   // check to make sure right number
      i++;
   }

   writeOp(O_CALL, f);

   patchOffset(ret+OPCODE_SIZE, outputTell(f), f);  // patch return address

   if (n->nodes[i].token != T_END_EVENT)
      parseError("Internal error, no ending event.");
   i++;
   return i;
}

int writeNode(NodeList *n, int i, OutputStream *f) {
   switch(n->nodes[i].token) {
      case T_START_EXPRESSION:
         i = writeExpression(n, i, f);
         break;

      case T_CALL_FUNC: {
         int args;
         i = writeCallFunc(n, i, f, &args);
         break;
      }
      case T_CONSTANT:
         switch(n->nodes[i].value.type) {
            case V_STRING: writeString(n->nodes[i].value.stringData, f); break;
            case V_FLOAT:  writeFloat(n->nodes[i].value.floatData, f); break;
            case V_INT:    writeInt(n->nodes[i].value.intData, f); break;
         }
         i++;
         break;
      case T_SYMBOL:
         if (n->nodes[i].value.type & P_LOCAL) {
            writeInt(n->nodes[i].value.intData, f);
            writeOp(O_FETCH, f);
         }
         else if (n->nodes[i].value.type & P_GLOBAL) {
            writeInt(n->nodes[i].value.intData, f);
            writeOp(O_FETCH_GLOBAL, f);
         }
         else if (n->nodes[i].value.type & P_EXTERN) {
            writeString(n->nodes[i].value.intData, f);
            writeOp(O_FETCH_EXTERNAL, f);
         }
         else parseError("Error, unknown type for symbol %x\n", n->nodes[i].value.type);

         i++;
         break;
      default: {
         switch(n->nodes[i].token) {
			case T_BWNOT: writeOp(O_BWNOT, f); i++; break;
            case T_NEGATE: writeOp(O_NEGATE, f); i++; break;
            case T_EQUAL: writeOp(O_EQUAL, f); i++; break;
				case T_NOT_EQUAL: writeOp(O_NOT_EQUAL, f); i++; break;
            case T_LESS_EQUAL: writeOp(O_LESS_EQUAL, f); i++; break;
            case T_GREATER_EQUAL: writeOp(O_GREATER_EQUAL, f); i++; break;
            case '<': writeOp(O_LESS, f); i++; break;
            case '>': writeOp(O_GREATER, f); i++; break;
            case '+': writeOp(O_ADD, f); i++; break;
            case '-': writeOp(O_SUB, f); i++; break;
            case '*': writeOp(O_MUL, f); i++; break;
            case '/': writeOp(O_DIV, f); i++; break;
            case '%': writeOp(O_MOD, f); i++; break;
            case T_AND: writeOp(O_AND, f); i++; break;
            case T_OR: writeOp(O_OR, f); i++; break;
				case T_BWAND: writeOp(O_BWAND, f); i++; break;
				case T_BWOR: writeOp(O_BWOR, f); i++; break;
				case T_NOT: writeOp(O_NOT, f); i++; break;
				case T_FLOOR: writeOp(O_FLOOR, f); i++; break;
				case T_BWXOR: writeOp(O_BWXOR, f); i++; break;
            default: i = writeLibExpression(n, i, f); break;
         }
         break;
      }
   }
   return i;
}

static void writeExportedVariables(VariableList *v, OutputStream *f) {
   int i;

   for (i=0; i < v->numVariables; ++i)
      if (v->variables[i].type & V_EXPORT) {
         writeString(v->variables[i].name, f);
         writeOp(O_EXPORT_VAR, f);
      }
}

static void writeExportedProcedures(ProcedureList *p, OutputStream *f) {
   int i;

   for (i=0; i < p->numProcedures; ++i)
      if (p->procedures[i].type & P_EXPORT) {
         writeInt(p->procedures[i].numArgs, f);
         writeInt(i, f);
         writeOp(O_EXPORT_PROC, f);
      }
}

static void writeExportedValues(VariableList *v, char *namespace, OutputStream *f) {
   int i;

   for (i=0; i < v->numVariables; ++i)
      if (v->variables[i].type & V_EXPORT) {
         writeVariable(v->variables + i, namespace, f);
         writeString(v->variables[i].name, f);
         writeOp(O_STORE_EXTERNAL, f);
      }
}

static void writeVariables(VariableList *v, char *namespace, int start, OutputStream *f) {
   int i;

   for (i=start; i<v->numVariables; ++i)
      writeVariable(v->variables + i, namespace, f);
}

/*
static void writeClearLocalVariables(VariableList *v, OutputStream *f) {
   int i;

   for (i=0; i<v->numVariables; ++i) {
      writeInt(0, f);
      writeInt(i, f);
      writeOp(O_STORE, f);
   }   
}   
*/

int writeExpressionProc(NodeList *n, int i, OutputStream *f) {
   if (n->nodes[i].token != T_START_EXPRESSION)
      parseError("no starting expression");
   if (n->nodes[i+1].token == T_CALL_FUNC) {
      i+=2;
      writeProcAddress(n, i, f);

      i++;
      if (n->nodes[i].token != T_START_EVENT)
         parseError("No starting event");
      i++;
      if (n->nodes[i].token != T_END_EVENT)
         parseError("Arguments not allowed for procedures as arguments");
      i++;
      if (n->nodes[i].token != T_END_EXPRESSION)
         parseError("No ending expression");
      i++;
   }
   else i = writeExpression(n, i, f);
   return i;
}

int writeNumExpressionProc(NodeList *n, int i, int num, OutputStream *f) {
   while(num--)
      i = writeExpressionProc(n, i, f);

   return i;
}

int writeExpression(NodeList *n, int i, OutputStream *f) {
   if (n->nodes[i++].token != T_START_EXPRESSION)
      parseError("start-expression expected.");

   while(n->nodes[i].token != T_END_EXPRESSION) {
      if (n->nodes[i].token == T_CALL_FUNC) {
         int args;
         int proc = n->nodes[i+1].value.intData;
         int j = i;
         i = writeCallFunc(n, i, f, &args);
         if (n->nodes[i+1].token == T_SYMBOL &&
             (n->nodes[i+1].value.type & P_PROCEDURE))
            if (currentProgram->procedures.procedures[proc].numArgs != args) {
               parseErrorLine(n->nodes[j].lineNum,
                       "Wrong number of arguments to procedure %s.  Expected %d, "
                       "got %d\n",
                       getName(currentProgram->procedures.procedures[proc].name, currentProgram->namespace),
                       currentProgram->procedures.procedures[proc].numArgs,
                       args);
         }
      }
      else
         i = writeNode(n, i, f);
   }

   return i+1;
}

int writeNumExpression(NodeList *n, int i, int num, OutputStream *f) {
   while(num--)
      i = writeExpression(n, i, f);

   return i;
}

static int writeStatement(NodeList *n, int i, OutputStream *f) {
   if (n->nodes[i].token == T_BEGIN)
      return writeBlock(n, i, f);

   if (n->nodes[i].token != T_START_STATEMENT)
      parseError("start-statement expected");

   i++;

   while(n->nodes[i].token != T_END_STATEMENT) {
      switch(n->nodes[i].token) {
         case T_BEGIN: i = writeBlock(n, i, f); break;
         case T_START_STATEMENT: i = writeStatement(n, i, f); break;
         case T_CANCEL:
            i++;
            if (n->nodes[i].token != T_SYMBOL)
               parseError("Internal error, symbol expected.");

            writeProcAddress(n, i, f);
            writeOp(O_CANCEL, f);
            break;

         EXP(CANCELALL, 0);

         case T_CALL_AT: {
            int proc;
            int flag = 0;
            int args = 0;

            i++;
            if (n->nodes[i].token != T_SYMBOL &&
                (n->nodes[i].token == T_CONSTANT && n->nodes[i].value.type != V_STRING))
               parseError("Internal error, symbol expected.");

            proc = i;

            i++;

            if (n->nodes[i].token != T_START_EVENT)
               parseError("Internal error, no starting event.");

            i++;

            if (n->nodes[i].token == T_START_ARG) {
               parseError("Internal error, arguments not currently allowed to events.");
               i++;
               while(n->nodes[i].token != T_END_ARG) {
                  i = writeExpression(n, i, f);
                  args++;
               }
               i++;
               writeInt(args, f);
            }

            if (n->nodes[i].token == T_CHECK_ARG_COUNT) {
               flag = 1;
               i++;
            }

            if (n->nodes[i].token != T_IN)
               parseError("Internal error, no event specifier, got %d", n->nodes[i].token);

            i++;

            if (n->nodes[i].token != T_START_EXPRESSION)
               parseError("Internal error, no starting expression");

            i = writeExpression(n, i, f);

            writeProcAddress(n, proc, f);

            if (flag) {
               writeOp(O_DUP, f);
               writeInt(args, f);
               writeOp(O_CHECK_ARG_COUNT, f);
               i++;
            }

            writeOp(O_CALL_AT, f);
            break;
         }
         case T_CALL_CONDITION: {
            int addr = outputTell(f), proc, cond, flag = 0;

            i++;

            if (n->nodes[i].token != T_SYMBOL &&
                (n->nodes[i].token == T_CONSTANT && n->nodes[i].value.type != V_STRING))
               parseError("Internal error, symbol expected.");

            writeInt(0, f);
            writeOp(O_JMP, f);

            proc = i;  // save procedure node

            i++;

            if (n->nodes[i].token != T_START_EVENT)
               parseError("Internal error, no starting event.");

            i++;

            if (n->nodes[i].token == T_START_ARG)
               parseError("Internal error, arguments not currently allowed to events.");

            if (n->nodes[i].token == T_CHECK_ARG_COUNT) {
               flag = 1;
               i++;
            }

            if (n->nodes[i].token != T_WHEN)
               parseError("Internal error, no event specifier");

            i++;

            if (n->nodes[i].token != T_START_EXPRESSION)
               parseError("Internal error, no starting expression");

            cond = outputTell(f);
            writeOp(O_CRITICAL_START, f);
            i = writeExpression(n, i, f);
            writeOp(O_CRITICAL_DONE, f);
            writeOp(O_STOP_PROG, f);

            patchOffset(addr + OPCODE_SIZE, outputTell(f), f);
            writeInt(cond, f);
            writeProcAddress(n, proc, f);
            if (flag) {
               writeOp(O_DUP, f);
               writeInt(0, f);
               writeOp(O_CHECK_ARG_COUNT, f);
               i++;
            }
            writeOp(O_CALL_CONDITION, f);
            break;
         }
         case T_CALL: {
            int ret = outputTell(f), proc;
            int args = 0;
            i++;

            if (n->nodes[i].token != T_SYMBOL &&
                (n->nodes[i].token == T_CONSTANT && n->nodes[i].value.type != V_STRING))
               parseError("Internal error, symbol expected.");

            writeInt(0, f);       // push return address
            writeOp(O_D_TO_A, f);

            proc = i;

            i++;

            if (n->nodes[i].token != T_START_EVENT)
               parseError("Internal error, no starting event.");

            i++;

            if (n->nodes[i].token == T_START_ARG) {
               i++;
               while(n->nodes[i].token != T_END_ARG) {
                  i = writeExpression(n, i, f);
                  args++;
               }
               i++;
            }
            writeInt(args, f);

            writeProcAddress(n, proc, f);

            if (n->nodes[i].token == T_CHECK_ARG_COUNT) {
               writeOp(O_DUP, f);
               writeInt(args, f);
               writeOp(O_CHECK_ARG_COUNT, f);
               i++;
            }

            writeOp(O_CALL, f);

            patchOffset(ret+OPCODE_SIZE, outputTell(f), f);  // patch return address
            writeOp(O_POP, f);

            if (n->nodes[i].token != T_END_EVENT)
               parseError("Internal error, no ending event.");
            i++;
            break;
         }

         case T_IF: {
            int true, false, j;

            false = outputTell(f);
            writeInt(0, f);
            j = i;
            i = writeExpression(n, i+1, f);
            writeOp(O_IF, f);

            if (n->nodes[i].token != T_THEN)
               parseError("Internal error, 'then' expected.");
            i++;     // skip then
            if (n->nodes[i].token == T_BEGIN) {
               i++;
               while(n->nodes[i].token != T_END)
                  i = writeStatement(n, i, f);
               i++;  // skip end
            }
            else i = writeStatement(n, i, f);

            if (n->nodes[i].token == T_ELSE) {
	       true = outputTell(f);
               writeInt(0, f);
               writeOp(O_JMP, f);

               patchOffset(false+OPCODE_SIZE, outputTell(f), f);

               i++;
               if (n->nodes[i].token == T_BEGIN) {
                  i++;
                  while(n->nodes[i].token != T_END)
                     i = writeStatement(n, i, f);
                  i++;
               }
               else i = writeStatement(n, i, f);
               patchOffset(true+OPCODE_SIZE, outputTell(f), f);
            }
            else {
               unsigned long a = outputTell(f);
               patchOffset(false+OPCODE_SIZE, a, f);
            }

            break;
         }
         case T_WHILE: {
            int false, top;

            false = outputTell(f);
            writeInt(0, f);
            top = outputTell(f);
            i = writeExpression(n, i+1, f);
            writeOp(O_WHILE, f);

            if (n->nodes[i].token != T_DO)
               parseError("Internal error, 'do' expected.");
            i++;     // skip do
            if (n->nodes[i].token == T_BEGIN) {
               i++;
               while(n->nodes[i].token != T_END)
                  i = writeStatement(n, i, f);
               i++;  // skip end
	    }
            else i = writeStatement(n, i, f);

            writeInt(top, f);
            writeOp(O_JMP, f);
            patchOffset(false+OPCODE_SIZE, outputTell(f), f);

            break;
         }
         case T_ASSIGN_ADD:
         case T_ASSIGN_MUL:
         case T_ASSIGN_SUB:
         case T_ASSIGN_DIV: {
            int j = i-1;
            int op;

            switch(n->nodes[i].token) {
               case T_ASSIGN_ADD: op = O_ADD; break;
               case T_ASSIGN_MUL: op = O_MUL; break;
               case T_ASSIGN_SUB: op = O_SUB; break;
               case T_ASSIGN_DIV: op = O_DIV; break;
            }

            writeInt(n->nodes[j].value.intData, f);
            if (n->nodes[j].value.type & P_LOCAL)
               writeOp(O_FETCH, f);
            else if (n->nodes[j].value.type & P_GLOBAL)
               writeOp(O_FETCH_GLOBAL, f);
            else if (n->nodes[j].value.type & P_EXTERN)
               writeOp(O_FETCH_EXTERNAL, f);

            else parseError("Error, unknown type for symbol %x\n", n->nodes[j].value.type);

            i = writeExpression(n, i+1, f);

            writeOp((unsigned short)op, f);

            if (n->nodes[j].value.type & P_LOCAL) {
               writeInt(n->nodes[j].value.intData, f);
               writeOp(O_STORE, f);
            }
            else if (n->nodes[j].value.type & P_GLOBAL) {
               writeInt(n->nodes[j].value.intData, f);
               writeOp(O_STORE_GLOBAL, f);
            }
            else {
               writeString(n->nodes[j].value.intData, f);
               writeOp(O_STORE_EXTERNAL, f);
            }
            break;
         }
         case T_ASSIGN: {
            int j = i-1;
            i = writeExpression(n, i+1, f);
            if (n->nodes[j].value.type & P_LOCAL) {
               writeInt(n->nodes[j].value.intData, f);
               writeOp(O_STORE, f);
            }
            else if (n->nodes[j].value.type & P_GLOBAL) {
               writeInt(n->nodes[j].value.intData, f);
               writeOp(O_STORE_GLOBAL, f);
            }
            else if (n->nodes[j].value.type & P_EXTERN) {
               writeString(n->nodes[j].value.intData, f);
               writeOp(O_STORE_EXTERNAL, f);
            }
            else parseError("Error, unknown type for symbol %x\n", n->nodes[j].value.type);
            break;
         }
         EXP(WAIT, 1);
         EXP(FORK, 1);
         EXP(SPAWN, 1);
         EXP(CALLSTART, 1);
         EXP(EXEC, 1);
         EXP(DETACH, 0);
         EXP(EXIT, 0);
         EXP(STARTCRITICAL, 0);
         EXP(ENDCRITICAL, 0);
         case T_RETURN: {
            int value = 0;

            i++;
            if (n->nodes[i].token == T_START_EXPRESSION) { // return with a value
               value = 1;
               i = writeExpression(n, i, f);
               writeOp(O_D_TO_A, f);
               writeOp(O_SWAPA, f);
            }

            // clear our stack
            writeOp(O_POP_TO_BASE, f);

            // restore previous base address
            writeOp(O_POP_BASE, f);

            if (value) // push any return value
               writeOp(O_A_TO_D, f);

            if (currentProcedure && (currentProcedure->type & P_CRITICAL))
               writeOp(O_CRITICAL_DONE, f);

            writeOp(O_POP_RETURN, f);
            break;
         }
         default: i = writeLibStatement(n, i, f); break;
      }
   }
   return i+1;
}

static int writeBlock(NodeList *n, int i, OutputStream *f) {
   if (n->nodes[i].token != T_BEGIN)
      parseError("begin expected");

   i++;
   while(n->nodes[i].token != T_END)
      i = writeStatement(n, i, f);

   return i+1;
}

static void writeCode(NodeList *n, OutputStream *f) {
   writeBlock(n, 0, f);
}

static void writeProcedure(Procedure *p, int tableOffset, OutputStream *f) {
   if (!(p->type & (P_IMPORT|P_EXPORT)) && p->defined == 1)
      parseError("Error, no code for procedure %s\n", getName(p->name, currentProgram->namespace));

   if (!p->nodes.numNodes) return;

   if (!p->uses && warnings)
      parseOutput("Warning: procedure %s not referenced\n", getName(p->name, currentProgram->namespace));

   if (p->type & P_CONDITIONAL) {
      int here, cond;

      here = outputTell(f);
      writeInt(0, f);  // offset to start of this procedure
      writeOp(O_JMP, f);    // jump to that offset
      cond = outputTell(f);
      writeOp(O_CRITICAL_START, f);
      writeExpression(&p->condition, 0, f);
      writeOp(O_CRITICAL_DONE, f);
      writeOp(O_STOP_PROG, f);
      patchOffset(here + OPCODE_SIZE, outputTell(f), f);
      patchProcTableEntry(tableOffset, 0, 3, cond, f);
   }
   writeOp(O_PUSH_BASE, f);

   writeVariables(&p->variables, p->namespace, p->numArgs, f);

   currentProcedure = p;

   writeCode(&p->nodes, f);

//   writeClearLocalVariables(&p->variables, f);

   writeOp(O_POP_TO_BASE, f);
   writeOp(O_POP_BASE, f);

   if (currentProcedure && (currentProcedure->type & P_CRITICAL))
      writeOp(O_CRITICAL_DONE, f);

   writeOp(O_POP_RETURN, f);
}

static void writeProcedures(Program *prog, int tableOffset, OutputStream *f) {
   int i;
   ProcedureList *p = &prog->procedures;

   for (i=0; i<p->numProcedures; ++i) {
      if (stricmp(getName(p->procedures[i].name, prog->namespace), "start") == 0) {
         if (p->procedures[i].type & P_IMPORT)
            parseError("Procedure 'start' cannot be imported");

         referenceProcedure(p, i);
         startOffset = outputTell(f);
      }

      // procedure at location zero is intentially boguz, and shouldn't
      // be tested
      if (i && !p->procedures[i].defined)
         parseError("Procedure %s prototyped but never defined\n", getName(p->procedures[i].name, prog->namespace));

      if (!(p->procedures[i].type & P_IMPORT)) {
         patchProcTableEntry(tableOffset, 0, 4, outputTell(f), f);
         writeProcedure(p->procedures+i, tableOffset, f);
      }
      tableOffset += 4 * PROCTABLE_SIZE;
   }
}

char *generateCode(Program *p, char *file) {
   OutputStream *f;
   int startAddr;

   startOffset = -1;
   f = malloc(sizeof(OutputStream));
   if (file) {
      f->f = fopen(file, "wb");
      f->type = OUTPUT_FILE;
   }
   else {
      f->mem.buf = malloc(4096);
      f->mem.len = 0;
      f->mem.maxlen = 4096;
      f->type = OUTPUT_MEM;
   }

   if (!f) {
      if (file)
         parseError("Couldn't open %s for write.", file);
      else parseError("Couldn't allocate memory for output.");
   }

   /* startup code pushes the address of an exit instruction onto the */
   /* address stack, then jumps to the entry point of the program.    */
   /* When that function returns, it will pop the address of the exit */
   /* instruction and execute it, exiting the program */
   writeOp(O_CRITICAL_START,f);//2   0
   writeInt(18, f);            //6
   writeOp(O_D_TO_A, f);       //2

   startAddr = outputTell(f);
   writeInt(0, f);             //6   10
   writeOp(O_JMP,f);           //2

   writeOp(O_EXIT_PROG,f);     //2   18 used when the start procedure returns

   writeOp(O_POP, f);               //2  20
   writeOp(O_POP_FLAGS_RETURN, f);  //2

   writeOp(O_POP, f);               //2  24
   writeOp(O_POP_FLAGS_EXIT, f);    //2

   writeOp(O_POP, f);                    //2  28
   writeOp(O_POP_FLAGS_RETURN_EXTERN, f);//2

   writeOp(O_POP, f);                    //2  32
   writeOp(O_POP_FLAGS_EXIT_EXTERN, f);  //2

   // return a value from an external function
   writeOp(O_POP_FLAGS_RETURN_VAL_EXTERN, f); //2  36

   // return a value from a local procedure called from C
   writeOp(O_POP_FLAGS_RETURN_VAL_EXIT, f);   // 38

   // return a value from an exported procedure called from C
   writeOp(O_POP_FLAGS_RETURN_VAL_EXIT_EXTERN, f);  // 40

   // startup code 42 bytes long.  If you add anything above this line that
   // emits code, you have to change the constant length of the startup code
   // in intrpret.c

   // write out our procedure tables
   procedureStart = outputTell(f);
   writeProcedureTable(f, &p->procedures);

   // write out our name and string spaces
   writeNamespace(f, p->namespace);
   writeNamespace(f, p->stringspace);

   // write out globals

   patchOffset(startAddr + OPCODE_SIZE, outputTell(f), f);

   writeOp(O_SET_GLOBAL, f); // 2
   writeVariables(&p->variables, p->namespace, 0, f);
   writeExportedVariables(&p->externals, f);
   writeExportedValues(&p->externals, p->namespace, f);
   writeExportedProcedures(&p->procedures, f);

   writeInt(0, f);  // write out # args to the starting function
   writeOp(O_CRITICAL_DONE, f);
   startAddr = outputTell(f);
   writeInt(18, f);  // push address of exit, in case there is no start
   writeOp(O_JMP, f);

   // write the procedures
   writeProcedures(p, procedureStart, f);

   // patch offset to where the program actually starts
   if (startOffset == -1)
      parseOutput("Warning, no 'start' procedure\n");
   else
      patchOffset(startAddr+OPCODE_SIZE, startOffset, f);

   outputClose(f);

   if (f->type == OUTPUT_MEM) {
      unsigned char *b = f->mem.buf;
      free(f);
      return b;
   }
   else {
      free(f);
      return 0;
   }
}
	