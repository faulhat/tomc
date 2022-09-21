lexer grammar CProgramLexer;

fragment DIGIT: [0-9] ;

Name: [a-zA-Z_] [a-zA-Z_0-9]* ;
Int: ( '0x' | '0b' )? DIGIT+ 'l'? ;
Float: ( DIGIT+ '.' DIGIT* | '.' DIGIT+ ) 'f'? ;
String: '"' ( ~'"' | '\\"' ) '"' ;
Char: '\'' ( ~'\'' | '\\\'' ) '\'' ;

StringifyToken: '#' [a-zA-Z_] [a-zA-Z_0-9]* ;
TokenConcat: [a-zA-Z_] [a-zA-Z_0-9]* '##' [a-zA-Z_0-9]+ ;

// Keywords
TYPEDEF: 'typedef' ;
RETURN: 'return' ;
IF: 'if' ;
ELSE: 'else' ;
FOR: 'for' ;
WHILE: 'while' ;
STRUCT: 'struct' ;
UNION: 'union' ;
CONST: 'const' ;

// Symbols
SEMI: ';' ;
COMMA: ',' ;
OSQ: '[' ;
CSQ: ']' ;
OCURL: '{' ;
CCURL: '}' ;
OPAR: '(' ;
CPAR: ')' ;
INC: '++' ;
DEC: '--' ;
DOT: '.' ;
ARROW: '->' ;
STAR: '*' ;
PLUS: '+' ;
MINUS: '-' ;
LOJNOT: '!' ;
BITNOT: '~' ;
BITAND: '&' ;
SIZEOF: 'sizeof' ;
DIV: '/' ;
MOD: '%' ;
LSHIFT: '<<' ;
RSHIFT: '>>' ;
LT: '<' ;
LE: '<=' ;
GT: '>' ;
GE: '>=' ;
EQ: '==' ;
NE: '!=' ;
ASSIGN: '=' ;
INCBY: '+=' ;
DECBY: '-=' ;
MULTBY: '*=' ;
DIVBY: '/=' ;
MODBY: '%=' ;
LSHIFTBY: '<<=' ;
RSHIFTBY: '>>=' ;
ANDBY: '&=' ;
ORBY: '|=' ;
XORBY: '^=' ;
XOR: '^' ;
BITOR: '|' ;
LOJAND: '&&' ;
LOJOR: '||' ;
WONDER: '?' ;
COLON: ':' ;

COMMENT: '/*' .*? '*/' -> skip ;
LCOMMENT: '//' .*? ( [\n\r] | EOF ) -> skip ;

WS: [ \n\t\r] -> skip ;
DEFINE: '#define' -> pushMode(PREPROC) ;
IFDEF: '#ifdef' -> pushMode(PREPROC) ;
IFNDEF: '#ifndef' -> pushMode(PREPROC) ;
UNDEF: '#undef' -> pushMode(PREPROC) ;
INCLUDE: '#include' -> pushMode(PREPROC) ;
ENDIF: '#endif' -> pushMode(PREPROC) ;

mode PREPROC;
IGNORE: '\\' [ \n\t\r] -> skip ;
ENDL: ( '\n' | '\r' | EOF ) -> popMode ;
PRE_String: '"' ( ~'"' | '\\"' ) '"' ;
PRE_Path_Inc: '<' ( ~'>' | '\\>' ) '>' ;
PRE_Name: [a-zA-Z_] [a-zA-Z_0-9]* ;
PRE_Other: .+? ;
