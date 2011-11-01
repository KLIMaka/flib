grammar SSL;

options {
  language = Java;
  output = AST;
  TokenLabelType = MyToken;
}

tokens {
  //VAR_DEF;
  //VAR_DEF_CONST;
  
  IMPORT_VAR;
  IMPORT_PROC;
  
  EXPORT_VAR;
  EXPORT_PROC;
  
  BLOCK;
  
  NEGATION;
  
  ERROR_POINT;
}

@header {
  package ssl.grammars;
}

@lexer::header {
  package ssl.grammars;
}

@lexer::members {
  private String m_source;
  
  public void setSource( String src ) {
    m_source = src;
  }
  
  public String getSource() {
    return m_source;
  }
  
  public Token emit() { 
    MyToken token = new MyToken(input, state.type, 
      state.channel, state.tokenStartCharIndex, getCharIndex()-1); 
    token.setLine(state.tokenStartLine); 
    token.setText(state.text); 
    token.setCharPositionInLine(state.tokenStartCharPositionInLine); 
    token.setSource(getSource());

    emit(token); 
    return token; 
  } 
}

@members {

    private int m_errors = 0;

   public void displayRecognitionError(String[] tokenNames,
                                        RecognitionException e) {
    String msg1 = getErrorMessage(e, tokenNames);
    
    String msg = ((MyToken)e.token).getSource().replace("\"","")+"("+e.token.getLine()+"): Error! " + msg1;
    System.out.println( msg );
    m_errors++;
  }
  
    public int getErrors() {
        return m_errors;
    }
}

all : program;

program
    :   
        (   variable_def
        |   procedure_decl
        |   procedure_def
        |   export_def
        |   import_def
        )*

    ;

variable_def
    :   VARIABLE
        (   var_def ';'?                                    -> var_def
        |   BEGIN (var_def ';'? )* END                      -> var_def*
        )
    ;

var_def
    :   ident (':=' constant)?                              -> ^(VARIABLE ident constant? ) 
    ;

variable_list
    :   variable_def (',' variable_def)*                    -> variable_def+
    ;
    
procedure_hdr
    :     CRITICAL? PROCEDURE   procedure_signature         -> CRITICAL?  procedure_signature
    ;
    
procedure_signature
    :   ident ('(' variable_list?  ')')?                    -> ident variable_list?
    ;

procedure_decl
    :   procedure_hdr  ';'                                  -> ^(PROCEDURE procedure_hdr)
    ;
    
procedure_def
    :   procedure_hdr
        ( IN  constant)?
        ( WHEN )?
        block                                               -> ^(PROCEDURE procedure_hdr block)
    ;
    
block
    :   BEGIN
            variable_def*
            statement*
        END                                                 -> ^(BLOCK variable_def* statement*)
    ;
    
statement
    :   block
    |   if_statement
    |   while_statement
    |   assignment_statement ';'!
    |   call_statement ';'!
    |   return_statement ';'!
    |   function_call ';'!
    ;
    
if_statement 
    :   IF expression 
        THEN s1=statement 
        (options {greedy=true;} : ELSE s2=statement)?       
            ->  {s2==null}? ^(IF expression $s1 )
            ->              ^(ELSE expression $s1 $s2 )
    ;
    
while_statement 
    :   WHILE expression DO 
        statement                                           -> ^(WHILE expression statement)
    ;
    
assignment_statement
    :   ident ':='^ expression
    |   ident '+=' expression                               -> ^(':=' ident ^('+' ident expression))
    |   ident '-=' expression                               -> ^(':=' ident ^('-' ident expression))
    |   ident '*=' expression                               -> ^(':=' ident ^('*' ident expression))
    |   ident '/=' expression                               -> ^(':=' ident ^('/' ident expression))
    |   ident '%=' expression                               -> ^(':=' ident ^('%' ident expression))
    ;
    
call_statement
    :   CALL function_call event?                           -> ^(CALL function_call event? ) 
    ;
    
event
    :   (IN | WHEN)^ expression
    ; 
    
return_statement
    :   RETURN expression?                                  -> ^(RETURN expression? )
    ;   
    
expression
    :   compare_expression 
        ((AND | OR)^ compare_expression)*
    ;
    
    
compare_expression
    :   expr 
        (('<' | '>' | '<=' | '>=' | '!=' | '==')^ expr)? 
    ;
    
    
expr
    :   term 
        (('-' | '+' | BWAND | BWOR | BWXOR )^ term)*
    ;
    
term
    :   factor 
        (('*' | '/' | '%')^ factor)*
    ;
    
factor
    :   (FLOOR | NOT | BWNOT | negation)^ factor  
    |   function_call 
    |   ident 
    |   constant 
    |   '('! expression ')'!
    ;
    
negation
    : '-' -> NEGATION 
    ;
    
function_call
    :   ident '(' argument_list? ')'                    -> ^(CALL ident argument_list?)
    |   constant '(' argument_list? ')'                 -> ^(CALL constant argument_list?)
    ;

argument_list
    :   expression (',' expression)*                    -> expression+
    ;

extern_procedure_decl
    :   PROCEDURE procedure_signature ';'               -> ^(PROCEDURE procedure_signature)
    |   (PROCEDURE BEGIN
            (procedure_signature ';')*
        END)                                            -> ^(PROCEDURE procedure_signature)*
    ;
    
import_def
    :   IMPORT
    (   extern_procedure_decl                           -> ^(IMPORT_PROC extern_procedure_decl)
    |   variable_def                                    -> ^(IMPORT_VAR variable_def)
    ); 
    
    
export_def
    :   EXPORT
    (   extern_procedure_decl                           -> ^(EXPORT_PROC extern_procedure_decl)
    |   variable_def                                    -> ^(EXPORT_VAR variable_def)
    );

ident
    :   IDENT                                           -> ERROR_POINT IDENT
    ;

constant 
    :   integer_literal
    |   float_literal
    |   string_literal
    |   cahr_literal
    ;
    
integer_literal
    :   DEC_NUMBER
    |   HEX_NUMBER
    ;
    
float_literal
    :   FLOAT_NUMBER
    ;
    
string_literal
    :   STRING_LITERAL
    ;

cahr_literal
    :   CAHR_LITERAL
    ;


fragment ALPHA : 'a'..'z' | 'A'..'Z' ;
fragment DIGIT : '0'..'9' ;
fragment HEXDIGIT : '0'..'9' | 'a'..'f' | 'A'..'F' ;
fragment NEWLINE : '\r' | '\n' ;

// Fragments for case insensitive math
fragment A:('a'|'A');
fragment B:('b'|'B');
fragment C:('c'|'C');
fragment D:('d'|'D');
fragment E:('e'|'E');
fragment F:('f'|'F');
fragment G:('g'|'G');
fragment H:('h'|'H');
fragment I:('i'|'I');
fragment J:('j'|'J');
fragment K:('k'|'K');
fragment L:('l'|'L');
fragment M:('m'|'M');
fragment N:('n'|'N');
fragment O:('o'|'O');
fragment P:('p'|'P');
fragment Q:('q'|'Q');
fragment R:('r'|'R');
fragment S:('s'|'S');
fragment T:('t'|'T');
fragment U:('u'|'U');
fragment V:('v'|'V');
fragment W:('w'|'W');
fragment X:('x'|'X');
fragment Y:('y'|'Y');
fragment Z:('z'|'Z');

// Keywords
PROCEDURE : P R O C E D U R E;
VARIABLE : V A R I A B L E;
IMPORT : I M P O R T;
EXPORT : E X P O R T;
IN : I N;
WHEN : W H E N;
BEGIN : B E G I N;
END : E N D;
CALL : C A L L;
CRITICAL : C R I T I C A L;
IF : I F;
THEN : T H E N;
ELSE : E L S E;
WHILE : W H I L E;
DO : D O;
AND : A N D;
OR : O R;
BWAND : B W A N D;
BWOR : B W O R;
BWXOR : B W X O R;
FLOOR : F L O O R;
NOT : N O T;
BWNOT : B W N O T;
RETURN : R E T U R N; 

IDENT : (ALPHA | '_') (ALPHA | DIGIT | '_')* ;
DEC_NUMBER : DIGIT+ ; 
HEX_NUMBER : '0x' HEXDIGIT+ ;

FLOAT_NUMBER 
    :   DIGIT+ ('.' DIGIT+)?
    |   '.' DIGIT
    ;
    
STRING_LITERAL
    :
    '\"'    { StringBuilder b = new StringBuilder(); }
        (   '\\' '\"'           { b.appendCodePoint( '\"' ); }
        |   '\\' '\\'           { b.appendCodePoint( '\\' ); }
        |   '\\' 'n'            { b.appendCodePoint( '\n' ); }
        |   '\\' 't'            { b.appendCodePoint( '\t' ); }
        |   c=~( '\"' | '\\')   { b.appendCodePoint( c ); }
        )*
    '\"'    { setText( b.toString() ); }
    ;
    
CAHR_LITERAL
    :   '\'' . '\'' { setText( getText().substring(1,2) ); }
    ;

WS : (NEWLINE | ' ' | '\f' | '\t')+ { $channel = HIDDEN; } ;

LINE : '#line ' d1=DEC_NUMBER ' ' STRING_LITERAL ' ' d2=DEC_NUMBER {
    $channel = HIDDEN; 
    input.setLine( Integer.parseInt(d1.getText())-1 );
    setSource($STRING_LITERAL.getText().replace("\"","").replace("\\\\","\\"));
    };