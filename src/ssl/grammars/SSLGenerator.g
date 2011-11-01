tree grammar SSLGenerator;

options {
  language = Java;
  tokenVocab = SSL;
  ASTLabelType = CommonTree;
}

@header {
  package ssl.grammars;
  
  import ssl.generator.symbols.Symbol;
  
  import ssl.errors.SemanticError;
  
  import ssl.generator.symbols.constants.Constant;
  import ssl.generator.symbols.constants.IntConstant;
  import ssl.generator.symbols.constants.FloatConstant;
  import ssl.generator.symbols.constants.StringConstant;
  
  import ssl.generator.symbols.variables.Argument;
  import ssl.generator.symbols.variables.GlobalVariable;
  import ssl.generator.symbols.variables.LocalVariable;
  import ssl.generator.symbols.variables.ExternalVariable;
  
  import ssl.generator.symbols.UndefinedProcedure;
  import ssl.generator.symbols.Procedure;
  import ssl.generator.Script;
  import ssl.generator.OP;
  import ssl.codegen.Node;
  
}

@members {
  public Script script = new Script(input);
  
  public void displayRecognitionError(String[] tokenNames,
                                        RecognitionException e) {
                                        
    MyToken t = (MyToken)e.token;
    if( t == null) {
        super.displayRecognitionError(tokenNames, e);
    } else {
        String msg = t.getSource().replace("\"","") + "(" + t.getLine() + "): Error! " + e.getMessage();
        System.err.println(msg);
        if( e instanceof SemanticError) {
            input.seek(((SemanticError)e).getMark());
        }
    }
  }
  
  public Procedure proc() {
    return script.getProc();
  }
  
}


prog 
    :   (   global_var
        |   proc_decl[Symbol.Type.NONE]
        |   proc_def
        |   import_decl
        |   export_decl
        )*
    ;
    
    
ident returns [String val]
    :   ERROR_POINT                         { input.mark(); }
        IDENT                               { $val = $IDENT.getText(); }
    ;
    
global_var
    :   var_decl                            { script.addSymbol( new GlobalVariable( $var_decl.name, $var_decl.def_val) ); }
    ;
    
var_decl returns [String name, Constant<?> def_val]
    :   ^( VARIABLE ident c=constant? )     { $name = $ident.val; $def_val = c==null? null : $constant.val; }
    ;    
    
proc_decl [Symbol.Type type]
    :   ^( 
            PROCEDURE crit ident            { UndefinedProcedure uproc = new UndefinedProcedure( script.addName($ident.val), type, $crit.val ); }
            decl_args 
          )                                 { uproc.setNumArgs( $decl_args.count );
                                              script.addSymbol( uproc ); }
    ;
    
crit returns [boolean val]
    :   crt=CRITICAL?                       { $val = crt==null?false:true; }
    ;
    
decl_args returns [int count]
    :   { $count = 0;}
       (var_decl {$count++;} )*
    ;
    
proc_def 
    :   ^( 
            PROCEDURE crit ident             { Procedure proc = new Procedure( script.addName($ident.val), script, $crit.val);
                                               script.addSymbol( proc ); 
                                               script.beginProc( proc );  }
            argument*                        { proc().checkArgs(); }
            block 
         )                                   { script.endProc(); }
    ;
    
argument
    :   var_decl                             { proc().addSymbol( new Argument( $var_decl.name )); }
    ;
    
block
    :   ^(BLOCK local_var* statement*)
    ;
    
local_var
    :   var_decl                             { proc().addSymbol( new LocalVariable( $var_decl.name, $var_decl.def_val )); }    
    ;
    
statement
    :   block
    |   if_statement
    |   ^(WHILE expression statement)
    |   ^(CALL fun_call event?)           
    |   ^(RETURN expression)
    |   assignment
    |   fun_call                            
    ;
    
if_statement
    :   ^( IF                               { Node end = proc().getCode().addAfter( new Node() );
                                              proc().getStream().write( OP.INTOP );
                                              proc().getStream().writeLabel( end.get().getLabel() ); }
           expression                       { proc().writeOP( OP.IF ); }
           statement                        { proc().setCode( end ); }
         )
    |   ^( ELSE                             { Node end = proc().getCode().addAfter( new Node() );
                                              Node elsen = end.addAfter( new Node() );
                                              proc().writeOP( OP.INTOP );
                                              proc().getStream().writeLabel( elsen.get().getLabel() ); }
           expression                       { proc().writeOP( OP.IF ); }
           statement                        { proc().writeOP( OP.INTOP );
                                              proc().getStream().writeLabel( end.get().getLabel() );
                                              proc().writeOP( OP.JMP );
                                              proc().setCode( elsen ); }
           statement                        { proc().setCode( end ); }
         )
    ;
    
     
expression
    :   ^('+' expression expression)        { proc().writeOP( OP.ADD ); }
    |   ^('-' expression expression)        { proc().writeOP( OP.SUB ); }
    |   ^('*' expression expression)        { proc().writeOP( OP.MUL ); }
    |   ^('/' expression expression)        { proc().writeOP( OP.DIV ); }
    |   ^('%' expression expression)        { proc().writeOP( OP.MOD ); }
    |   ^(OR expression expression)         { proc().writeOP( OP.OR ); }
    |   ^(AND expression expression)        { proc().writeOP( OP.AND ); }
    |   ^(BWOR expression expression)       { proc().writeOP( OP.BWOR ); }
    |   ^(BWAND expression expression)      { proc().writeOP( OP.BWAND ); }
    |   ^(BWXOR expression expression)      { proc().writeOP( OP.BWXOR ); }
    |   ^('>' expression expression)        { proc().writeOP( OP.GREATER ); }
    |   ^('<' expression expression)        { proc().writeOP( OP.LESS ); }
    |   ^('<=' expression expression)       { proc().writeOP( OP.LESS_EQUAL ); }
    |   ^('>=' expression expression)       { proc().writeOP( OP.GREATER_EQUAL ); }
    |   ^('!=' expression expression)       { proc().writeOP( OP.NOT_EQUAL ); }
    |   ^('==' expression expression)       { proc().writeOP( OP.EQUAL ); }
    |   ^(FLOOR expression)                 { proc().writeOP( OP.FLOOR ); }
    |   ^(NOT expression)                   { proc().writeOP( OP.NOT ); }
    |   ^(BWNOT expression)                 { proc().writeOP( OP.BWNOT ); }
    |   ^(NEGATION expression)              { proc().writeOP( OP.NEGATE ); }
    |   fun_call
    |   symbol                               { proc().writeGet($symbol.val); }
    |   constant                             { proc().writeGet($constant.val); }
    ;
    
assignment
    :   ^(':=' symbol expression)            { proc().writeSet($symbol.val); }         
    ;
    
fun_call
    :   ^(CALL symbol arg_list)              { proc().writeCall($symbol.val, $arg_list.count ); }
    |   ^(CALL constant arg_list)            { proc().writeCall($constant.val, $arg_list.count ); }
    ;
    
symbol returns [Symbol val]
    :   ident                                { $val = script.getSymbol( $ident.val); }
    ;
    
arg_list returns [int count]
    : { count = 0; }
        ( expression {count++;} )*
    ;
    
event
    :   ^(IN expression)
    |   ^(WHEN expression)
    ;
    
import_decl
    :   ^(IMPORT_PROC proc_decl[Symbol.Type.IMPORTED]* ) 
    |   ^(IMPORT_VAR extern_var[Symbol.Type.IMPORTED]* ) 
    ;
    
export_decl
    :   ^(EXPORT_PROC proc_decl[Symbol.Type.EXPORTED]* )
    |   ^(EXPORT_VAR extern_var[Symbol.Type.EXPORTED]* ) 
    ;
    
extern_var[Symbol.Type type]
    :   var_decl                            { script.addSymbol( new ExternalVariable( script.addName($var_decl.name), $var_decl.def_val, type)); } 
    ;


constant returns [Constant<?> val]
    :   DEC_NUMBER                          { $val = new IntConstant( "", new Integer($DEC_NUMBER.getText())); }
    |   FLOAT_NUMBER                        { $val = new FloatConstant( "", new Float($FLOAT_NUMBER.getText())); }
    |   STRING_LITERAL                      { $val = new StringConstant( "", script.addString($STRING_LITERAL.getText())); }
    ;
