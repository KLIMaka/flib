#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>

extern	int	compilerErrorTotal;

enum { V_INT=1, V_FLOAT, V_STRING };

/*
 * namespaces and stringspaces are encoded with a 2-byte length followed by
 * the name or string.
 * program chunks reference variables by index; if a program encounters
 * a variable index that is greater than the number of locals, this index
 * is referenced as a global variable (after subtracting off the number
 * of local variables).  Global variables can be defined anywhere.
 */

typedef struct {
   int type;   // this type is the type of the variable
   union {
      int intData;
      float floatData;
      int stringData;  /* offset into program's string space */
   };
} Value;

typedef struct {
   int name;   /* offset into program or procedure's namespace */
   int uses;
   Value value;
#define V_LOCAL    1
#define V_GLOBAL   2
#define V_IMPORT   3
#define V_EXPORT   4
   int type;   // this type is where it was declared
} Variable;

typedef struct {
   Variable *variables;
   int numVariables;
} VariableList;

typedef struct {
   int token;
   int lineNum;
   Value value;
} Node;

typedef struct {
   Node *nodes;
   int numNodes;
} NodeList;

typedef struct {
   int  name;          /* offset into main program's namespace    */
   int type;           /* timed, conditional, or system procedure */
#define P_TIMED        0x01
#define P_CONDITIONAL  0x02
#define P_IMPORT       0x04
#define P_EXPORT       0x08
#define P_CRITICAL     0x10
   union {
      int time;
      NodeList condition;
   };
   char *namespace;     /* this program's namespace */
   int numArgs;
   int uses;
   int defined;         // set to 1 if this has been define (and not just declared)
   VariableList variables;
   NodeList nodes;
} Procedure;

typedef struct {
   Procedure *procedures;
   int numProcedures;
} ProcedureList;

typedef struct {
#define OUTPUT_MEM  1
#define OUTPUT_FILE 2
   int type;
   union {
      FILE *f;
      struct {
         char *buf;
         int len, maxlen;
      } mem;
   };
} OutputStream;

#include <setjmp.h>

typedef struct {
   ProcedureList procedures;
   VariableList variables;
   VariableList externals;  // all variables either exported from this module,
                            // or imported to this module
   char *stringspace;   /* this program's string space */
   char *namespace;     /* this program's global namespace */
   jmp_buf env;
} Program;

#include "lex.h"

extern char *parse(InputStream *, char *);
extern void dumpProgram(Program *);
extern char *generateCode(Program *, char *);

extern void parseExpression(Procedure *p, NodeList *nodes);
extern void parseExpGroup(Procedure *p, NodeList *nodes, int num);

extern void emitOp(Procedure *p, NodeList *nodes, int token);
extern void emitInt(Procedure *p, NodeList *nodes, int i);
extern void emitNode(Procedure *p, NodeList *n, LexData *data);

extern int writeNumExpression(NodeList *n, int i, int num, OutputStream *f);
extern int writeExpression(NodeList *n, int i, OutputStream *f);
extern int writeExpressionProc(NodeList *n, int i, OutputStream *f);
extern int writeNumExpressionProc(NodeList *n, int i, int num, OutputStream *f);
extern void writeOp(unsigned short op, OutputStream *f);
extern void writeInt(unsigned long a, OutputStream *f);
extern void writeFloat(float a, OutputStream *f);
extern void writeString(unsigned long a, OutputStream *f);

extern void parseLibArgs(Procedure *, int);
extern void parseError(char *format, ...);
extern int expectToken(int expectToken);

extern void freeCurrentProgram(void);

#define P_GLOBAL    0x80000000
#define P_LOCAL     0x40000000
#define P_PROCEDURE 0x20000000
#define P_EXTERN    0x10000000

#include "opcodes.h"

// write out num expressions with opcode x

#define WEXP(x, num) i++; i = writeNumExpression(n, i, num, f); writeOp(x, f)
#define WEXPPROC(x, num) i++; i = writeNumExpressionProc(n, i, num, f); writeOp(x, f)

// make a case for token T_x, write out opcode O_x and num expressions

// use this one to write an 'statement' type opcode/lib function
#define EXP(x, num) case T_##x: WEXP(O_##x, num); break
// this is like above, but can take procedures as arguments
#define EXPPROC(x, num) case T_##x: WEXPPROC(O_##x, num); break

// use this one to write out a library function/opcode that can be
// used in both a statement and expression context, when that opcode/function
// is being used as a statement (for the expression case, just use EXP/EXPPROC)
#define EXPST(x, num) case T_##x: WEXP(O_##x, num); writeOp(O_POP, f); break
#define EXPSTPROC(x, num) case T_##x: WEXPPROC(O_##x, num); writeOp(O_POP, f); break

// write out opcode O_x for token T_x; assumes the output file handle is 'f'.
#define ST(x) case T_##x: i++; writeOp(O_##x, f); break

// parse N expressions, separated by commas.
// assumes input procedure pointer is 'p', and
// all nodes are going into that procedure's node list.

// Use this one to parse statements
#define PARSE(t, n) case T_##t: parseLibArgs(p, n); break

// use this one to parse expressions
#define PARSEEXP(t, n) case T_##t: \
   emitOp(p, &p->nodes, T_##t);    \
   parseLibArgs(p, n);             \
   break


#endif
